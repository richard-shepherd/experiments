#pragma once
#include <string>
#include <functional>
#include "uv.h"
#include "SharedPointers.h"
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
        // Creates a Socket instance to be managed by the uv loop specified.
        static UVLoopPtr create(const std::string& name) { return UVLoopPtr(new UVLoop(name)); }

        // Destructor.
        ~UVLoop();

        // Gets the loop's name.
        const std::string& getName() const { return m_name; }

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
        // Constructor.
        // NOTE: The constructor is private. Use UVLoop::create() to create an instance.
        UVLoop(const std::string& name);

        // Thread entry point.
        void threadEntryPoint();

        // Processes marshalled events.
        void processMarshalledEvents();

    // Private data...
    private:
        // The loop name. This will also be set as the name of the thread running the loop.
        std::string m_name;

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

