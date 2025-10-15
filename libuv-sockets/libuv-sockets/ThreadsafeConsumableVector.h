#pragma once
#include <vector>
#include <set>
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
    template<typename ItemType, typename UniqueKeyType = int>
    class ThreadsafeConsumableVector
    {
    // Public types...
    public:
        // Vector of ItemType.
        typedef std::vector<ItemType> VecItemType;

        // Shared pointer for VecItemType.
        typedef std::shared_ptr<VecItemType> VecItemTypePtr;

    // Public methods...
    public:
        // Constructor.
        ThreadsafeConsumableVector()
        {
            // We create the items vector...
            m_items = std::make_shared<VecItemType>();

            // We create the mutex...
            m_mutex = std::make_unique<uv_mutex_t>();
            uv_mutex_init(m_mutex.get());
        }

        // Adds an item to the vector.
        void add(ItemType& item)
        {
            UVUtils::Lock lock(m_mutex);
            m_items->push_back(item);
        }

        // Adds an item to the vector if the key has not already been registered.
        // Returns true if the item was added, false if not.
        bool addUnique(const UniqueKeyType& key, ItemType& item)
        {
            UVUtils::Lock lock(m_mutex);
            if (m_uniqueKeys.count(key))
            {
                // The name has already been registered...
                return false;
            }
            else
            {
                // The name has not been registered, so we add the item...
                m_items->push_back(item);
                m_uniqueKeys.insert(key);
                return true;
            }
        }

        // Gets the current contents of the vector, and clears the data being held.
        VecItemTypePtr getItems()
        {
            UVUtils::Lock lock(m_mutex);
            auto result = m_items;
            m_items = std::make_shared<VecItemType>();
            m_uniqueKeys.clear();
            return result;
        }

    // Private data...
    private:
        // Vector of items.
        VecItemTypePtr m_items;

        // Set of keys for registered unique items.
        typedef std::set<UniqueKeyType> UniqueKeySet;
        UniqueKeySet m_uniqueKeys;

        // Mutex.
        std::unique_ptr<uv_mutex_t> m_mutex;
    };

} // namespace

