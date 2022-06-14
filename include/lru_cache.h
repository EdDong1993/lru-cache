#pragma once

#include <list>
#include <mutex>
#include <string>
#include <unordered_map>

template <typename T>
struct void_deletor_t {
    void operator()(T &t) {
    }
};

template <typename key, typename value, typename deletor = void_deletor_t<value>>
class lru_cache {
public:
    using key_value = std::pair<key, value>;

    lru_cache(size_t max_size = 1024) : m_max_size(max_size) {
    }

    virtual ~lru_cache() {
        for (auto &pair : m_item_list) {
            delete_value(pair.second);
        }
    }

    void put(const key &k, const value &v) {
        std::lock_guard<std::mutex> lock(m_mutex);
        auto it = m_item_map.find(k);
        if (it != m_item_map.end()) {
            delete_value(it->second->second);
            m_item_list.erase(it->second);
            m_item_map.erase(it);
        }
        m_item_list.push_front(std::make_pair(k, v));
        m_item_map.insert(std::make_pair(k, m_item_list.begin()));
        clean();
    }

    bool get(const key &k, value &v) {
        std::lock_guard<std::mutex> lock(m_mutex);
        auto it = m_item_map.find(k);
        if (it == m_item_map.end()) {
            return false;
        } else {
            m_item_list.splice(m_item_list.begin(), m_item_list, it->second);
            v = it->second->second;
            return true;
        }
    }

    bool exist(const key &k) {
        std::lock_guard<std::mutex> lock(m_mutex);
        return (m_item_map.count(k) > 0);
    }

    void set_max_size(uint32_t max_size) {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_max_size = max_size;
        clean();
    }

    void clear() {
        std::lock_guard<std::mutex> lock(m_mutex);
        while (m_item_map.size() > 0) {
            auto last_it = m_item_list.end();
            last_it--;
            m_item_map.erase(last_it->first);
            delete_value(last_it->second);
            m_item_list.pop_back();
        }
    }

    size_t size() {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_item_map.size();
    }

private:
    inline void delete_value(value &v) {
        deletor del;
        del(v);
    }

    void clean() {
        while (m_item_map.size() > m_max_size) {
            auto last_it = m_item_list.end();
            last_it--;
            m_item_map.erase(last_it->first);
            delete_value(last_it->second);
            m_item_list.pop_back();
        }
    }

    std::list<key_value> m_item_list;
    std::unordered_map<key, decltype(m_item_list.begin())> m_item_map;
    size_t m_max_size;
    std::mutex m_mutex;
};
