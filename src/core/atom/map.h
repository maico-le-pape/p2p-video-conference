/*
 *   Copyright 2012 Epyx Team
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *       http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 */
/**
 * @file map.h
 * @brief Thread-safe map
 */

#ifndef EPYX_ATOM_MAP_H
#define EPYX_ATOM_MAP_H

#include <boost/noncopyable.hpp>
#include <map>
#include <mutex>

#ifndef EPYX_ATOM_MAP_DEBUG
/**
 * @brief atom::Map debug
 */
#define EPYX_ATOM_MAP_DEBUG 0
#endif

namespace Epyx
{
    namespace atom
    {
        /**
         * @class Map
         * @brief Thread-safe STL map
         */
        template <typename TKey, typename TVal> class Map : private boost::noncopyable
        {
        public:
            /**
             * @brief iterator
             */
            typedef typename std::map<TKey, TVal>::iterator iterator;
            /**
             * @brief constant iterator
             */
            typedef typename std::map<TKey, TVal>::const_iterator const_iterator;

            /**
             * @brief Constructor
             */
            Map();

            /**
             * @brief Insert an element
             * @param key
             * @param value
             */
            void set(TKey key, TVal value);

            /**
             * @brief Insert an element when the map is already locked
             * @param key
             * @param value
             */
            void setLocked(TKey key, TVal value);

            /**
             * @brief Get an element and lock the map to do something with
             * @param key
             * @param defval Default value
             * @return map[key] if it exists, defval otherwise
             *
             * @note Call endUnlock() to unlock the map
             */
            TVal getAndLock(TKey key, TVal defval);

            /**
             * @brief Get an element
             * @param key
             * @param defval Default value
             * @return map[key] if it exists, defval otherwise
             */
            TVal get(TKey key, TVal defval);

            /**
             * @brief Erase an element
             * @param key
             * @return true if there was an element which is now deleted
             */
            bool unset(TKey key);

            /**
             * @brief Get and unset
             * @param key
             * @param defval default return value
             * @return unsetted value
             *
             * This may be useful to call a destrcuor on a registered value
             */
            TVal getUnset(TKey key, TVal defval);

            /**
             * @brief Clear the map
             * @param isLocked true if the map is already locked
             */
            void clear(bool isLocked = false);

            /**
             * @brief Clear the map and disallow any change
             */
            void clearForEver();

            /**
             * @brief Is it empty ?
             * @return true if it is empty
             */
            bool empty();

            /**
             * @brief Get the number of items
             * @return count
             */
            long unsigned int size();

            /**
             * @brief Lock map and begin iteration
             * @return iterator
             */
            iterator beginLock();

            /**
             * @brief Tell wether iterator is ended
             * @param it
             * @return true on and
             */
            bool isEnd(const const_iterator& it);

            /**
             * @brief End iteration and unlock map
             */
            void endUnlock();

        private:
            bool readOnly;
            std::map<TKey, TVal> map;
            std::mutex mut;
        };
    }
}

#include "map-detail.h"

#endif /* EPYX_ATOM_MAP_H */

