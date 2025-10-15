#pragma once
#include <string>
#include <functional>
#include "uv.h"
#include "ThreadsafeConsumableVector.h"

namespace MessagingMesh
{
    /// <summary>
    /// Creates a thread which runs a libuv event loop.
    /// 
    /// You can marshall events to the loop which will be picked up
    /// and run on the loop's thread.
    /// </summary>
    class UVLoop
    {
    // Public types...
    public:
        // An 'event' which can be marshalled to the event loop we are managing.
        typedef std::function<void(uv_loop_t*)> MarshalledEvent;

        // Enum for unique marshalled events. (Events where we only marshall
        // one at any one time for a given key.)
        enum class UniqueEventKey
        {
            SOCKET_QUEUE_WRITE
        };

    // Public methods...
    public:
        // Constructor.
        UVLoop(const std::string& threadName);

        // Destructor.
        ~UVLoop();

        // Gets the UV loop.
        uv_loop_t* getUVLoop() const { return m_loop.get();  }

        // Marshalls an event to the UV loop we are managing. This event (function)
        // will be called from within the event loop.
        void marshallEvent(MarshalledEvent marshalledEvent);

        // Marshalls an event to the UV loop we are managing. 
        // Only one event for the given key will be marshalled until events
        // are processed in the UV loop thread.
        void marshallUniqueEvent(const UniqueEventKey& key, MarshalledEvent marshalledEvent);

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
        std::unique_ptr<uv_loop_t> m_loop;

        // Signal sent to the event loop when there are new marshalled events.
        std::unique_ptr<uv_async_t> m_marshalledEventsSignal;

        // Vector of marshalled events and a lock for it.
        ThreadsafeConsumableVector<MarshalledEvent, UniqueEventKey> m_marshalledEvents;
    };

} // namespace

