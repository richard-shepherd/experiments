#include "UVLoopThread.h"
#include "Logger.h"
#include "Utils.h"
#include "UVUtils.h"
using namespace MessagingMesh;

// TODO: delete the pointers (or use make_unique???)

// Constructor.
UVLoopThread::UVLoopThread(const std::string& threadName) :
    m_threadName(threadName),
    m_pLoop(nullptr),
    m_pMarshalledEventsSignal(nullptr)
{
    Logger::info("Creating thread: " + m_threadName);

    // We create a mutex for the marshalled events vector...
    m_pMarshalledEventsMutex = new uv_mutex_t;
    uv_mutex_init(m_pMarshalledEventsMutex);

    // We create the thread...
    uv_thread_create(
        &m_threadHandle, 
        [](void* args)
        {
            auto self = (UVLoopThread*)args;
            self->threadEntryPoint();
        },
        this);
}

// Destructor.
UVLoopThread::~UVLoopThread()
{
    // We marshall an event to the loop to tell it to stop...
    Logger::info("Signalling thread to stop: " + m_threadName);
    marshallEvent(
        [](uv_loop_t* pLoop)
        {
            uv_stop(pLoop);
        });

    // We wait for the thread to end...
    Logger::info("Waiting for thread to stop: " + m_threadName);
    uv_thread_join(&m_threadHandle);
    Logger::info("Thread stopped: " + m_threadName);
}

// Thread entry point.
void UVLoopThread::threadEntryPoint()
{
    try
    {
        // We set the thread's name...
        uv_thread_setname(m_threadName.c_str());

        // We create the uv loop and tell it that it is associated with 
        // this UVLoopThread object...
        m_pLoop = new uv_loop_t;
        m_pLoop->data = this;
        uv_loop_init(m_pLoop);

        // We listen for marshalled events...
        m_pMarshalledEventsSignal = new uv_async_t;
        uv_async_init(
            m_pLoop,
            m_pMarshalledEventsSignal,
            [](uv_async_t* pHandle)
            {
                auto self = (UVLoopThread*)pHandle->loop->data;
                self->processMarshalledEvents();
            });

        // We signal the event in case there are already marshalled events...
        uv_async_send(m_pMarshalledEventsSignal);

        // We run the loop...
        Logger::info("Running event loop for thread: " + m_threadName);
        uv_run(m_pLoop, UV_RUN_DEFAULT);
    }
    catch (const std::exception& ex)
    {
        auto message = Utils::format("UVLoopThread::threadEntryPoint - %s", ex.what());
        Logger::error(message);
    }
}

// Marshalls an 'event' to the uv loop we are managing. This event (function)
// will be called from within the event loop.
void UVLoopThread::marshallEvent(MarshalledEvent marshalledEvent)
{
    // We add the event to the collection of marshalled events.
    // This is done in a short-lived lock as the collection of events can
    // be accessed from the event thread.
    {
        UVUtils::Lock lock(m_pMarshalledEventsMutex);
        m_marshalledEvents.push_back(marshalledEvent);
    }

    // We signal to the event loop that there is a new event.
    // Note: We check that the loop and signal have been set up. If not, we
    //       cannot signal, but the event has still been added to the queue.
    if (m_pMarshalledEventsSignal)
    {
        uv_async_send(m_pMarshalledEventsSignal);
    }
}

// Processes marshalled events.
void UVLoopThread::processMarshalledEvents()
{
    try
    {
        // We get the marshalled events and clear the original collection.
        // This is done in a short-lived lock as the events collection can
        // be added to from other threads.
        VecMarshalledEvents marshalledEvents;
        {
            UVUtils::Lock lock(m_pMarshalledEventsMutex);
            marshalledEvents = m_marshalledEvents;
            m_marshalledEvents.clear();
        }

        // We process the events...
        for (auto marshalledEvent : marshalledEvents)
        {
            marshalledEvent(m_pLoop);
        }
    }
    catch (const std::exception& ex)
    {
        auto message = Utils::format("UVLoopThread::processMarshalledEvents - %s", ex.what());
        Logger::error(message);
    }
}
