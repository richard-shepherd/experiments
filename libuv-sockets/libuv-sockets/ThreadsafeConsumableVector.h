#pragma once
#include <vector>
#include <memory>
#include "uv.h"
#include "UVUtils.h"

namespace MessagingMesh
{
    /// <summary>
    /// Allows you to add data from producer threads and retrieve data
    /// from a consumer thread.
    /// 
    /// When you retrieve data you are given a copy of all the data currently 
    /// available, and the internal data is cleared.
    /// </summary>
    template<typename T>
    class ThreadsafeConsumableVector
    {
    // Public types...
    public:
        // Vector of T.
        typedef std::vector<T> VecT;

        // Shared pointer for std::vector<T>.
        typedef std::shared_ptr<VecT> VecTPtr;

    // Public methods...
    public:
        // Constructor.
        ThreadsafeConsumableVector()
        {
            // We create the items vector...
            m_items = std::make_shared<VecT>();

            // We create the mutex...
            m_mutex = std::make_unique<uv_mutex_t>();
            uv_mutex_init(m_mutex.get());
        }

        // Adds an item to the vector.
        void add(T& item)
        {
            UVUtils::Lock lock(m_mutex);
            m_items->push_back(item);
        }

        // Gets the current contents of the vector, and clears the data being held.
        VecTPtr get()
        {
            UVUtils::Lock lock(m_mutex);
            auto result = m_items;
            m_items = std::make_shared<VecT>();
            return result;
        }

    // Private data...
    private:
        // Vector of items and a lock for it.
        VecTPtr m_items;
        std::unique_ptr<uv_mutex_t> m_mutex;
    };

} // namespace

