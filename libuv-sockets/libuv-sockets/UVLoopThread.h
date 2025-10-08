#pragma once
#include <string>
#include <vector>
#include <functional>
#include "uv.h"

namespace MessagingMesh
{
    /// <summary>
    /// Creates a thread which runs a libuv event loop.
    /// 
    /// You can marshall events to the loop which will be picked up
    /// and run on the loop's thread.
    /// </summary>
    class UVLoopThread
    {
    // Public types...
    public:
        // An 'event' which can be marshalled to the event loop we are managing.
        typedef std::function<void(uv_loop_t*)> MarshalledEvent;

    // Public methods...
    public:
        // Constructor.
        UVLoopThread(const std::string& threadName);

        // Destructor.
        ~UVLoopThread();

        // Marshalls an 'event' to the uv loop we are managing. This event (function)
        // will be called from within the event loop.
        void marshallEvent(MarshalledEvent marshalledEvent);

    // Private functions...
    private:
        // Thread entry point.
        void threadEntryPoint();

        // Processes marshalled events.
        void processMarshalledEvents();

    // Private data...
    private:
        // The thread name...
        std::string m_threadName;

        // Thread handle.
        uv_thread_t m_threadHandle;

        // UV message loop running on the thread.
        uv_loop_t* m_pLoop;

        // Signal sent to the event loop when there are new marshalled events.
        uv_async_t* m_pMarshalledEventsSignal;

        // Vector of marshalled events and a lock for it.
        typedef std::vector<MarshalledEvent> VecMarshalledEvents;
        VecMarshalledEvents m_marshalledEvents;
        uv_mutex_t* m_pMarshalledEventsMutex;
    };

} // namespace

