#ifndef LRUCACHE_LRUCACHE_H
#define LRUCACHE_LRUCACHE_H

#include <map>
#include <string>
#include <iostream>

template<class T, class V>
class LRUCache
{
    typedef struct Entry
    {
        Entry *prev;
        Entry *next;
        T key;
        V value;
    } Entry;
    typedef std::map<T, Entry *> EntryMap;
public:
    explicit LRUCache(int size) : current_size_(0), cache_size_(size) {}

    int Get(const T &key, V &value);

    int Put(const T &key, const V &value);

    void Print();

    void Clear();

private:
    void Remove(const T &key);

    void RemoveTail();

    void MoveToHead(Entry *node);

private:
    int cache_size_;
    EntryMap nodes_;
    int current_size_;
    Entry *head_;
    Entry *tail_;
};

template<class T, class V>
int LRUCache<T, V>::Get(const T &key, V &value) {
    typename EntryMap::iterator iter = nodes_.find(key);
    if (iter == nodes_.end()) {
        return -1;
    }
    MoveToHead(iter->second);
    value = iter->second->value;
    return 0;
}

template<class T, class V>
int LRUCache<T, V>::Put(const T &key, const V &value) {
    typename EntryMap::iterator iter = nodes_.find(key);
    if (iter == nodes_.end()) {
        if (current_size_ >= cache_size_) {
            nodes_.erase(nodes_.find(tail_->key));
            RemoveTail();
        } else {
            current_size_++;
        }
        Entry *node = new Entry();
        node->key = key;
        node->value = value;
        node->prev = NULL;
        node->next = NULL;
        MoveToHead(node);
        nodes_[key] = node;
    }
    return 0;
}

template<class T, class V>
void LRUCache<T, V>::Print() {
    std::cout << "head key: " << head_->key << ", tail key: " << tail_->key << std::endl;
    if (head_) {
        Entry *node = head_;
        while (node) {
            std::cout << "key: " << node->key << ", value: " << node->value << std::endl;
            node = node->next;
        }
    } else {
        std::cout << "null" << std::endl;
    }
}

template<class T, class V>
void LRUCache<T, V>::Clear() {
    head_ = NULL;
    tail_ = NULL;
    current_size_ = 0;
    for (typename EntryMap::iterator iter = nodes_.begin(); iter != nodes_.end(); iter++) {
        Entry *node = iter->second;
        if (node) {
            delete node;
            node = NULL;
        }
    }
    nodes_.clear();
}

template<class T, class V>
void LRUCache<T, V>::Remove(const T &key) {
    typename EntryMap::iterator iter = nodes_.find(key);
    if (iter != nodes_.end()) {
        Entry *node = iter->second;
        if (node->prev) {
            node->prev->next = node->next;
        }
        if (node->next) {
            node->next->prev = node->prev;
        }
        if (node == tail_) {
            tail_ = node->prev;
        }
        if (node == head_) {
            head_ = node->next;
        }
        nodes_.erase(iter);
        delete node;
    }
}

template<class T, class V>
void LRUCache<T, V>::RemoveTail() {
    if (tail_) {
        if (tail_->prev) {
            tail_->prev->next = NULL;
        } else {
            head_ = NULL;
        }
        tail_ = tail_->prev;
    }
}

template<class T, class V>
void LRUCache<T, V>::MoveToHead(Entry *node) {
    assert(node);
    if (node == head_) {
        return;
    }
    if (node->prev) {
        node->prev->next = node->next;
    }
    if (node->next) {
        node->next->prev = node->prev;
    }
    if (node == tail_) {
        tail_ = node->prev;
    }
    if (head_) {
        node->next = head_;
        head_->prev = node;
    }
    head_ = node;
    node->prev = NULL;
    if (tail_ == NULL) {
        tail_ = head_;
    }
}

#endif //LRUCACHE_LRUCACHE_H
