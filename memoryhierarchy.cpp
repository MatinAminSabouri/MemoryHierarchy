#include "memoryhierarchy.h"

Cache::Cache(int size, int blockSize, ReplacementPolicy policy)
    : size(size), blockSize(blockSize), policy(policy) {}

bool Cache::accessMemory(int address) {
    int blockAddress = address / blockSize;
    if (cache.find(blockAddress) != cache.end()) {
        // Hit
        hits++;
        if (policy == LRU) {
            accessOrder.erase(std::remove(accessOrder.begin(), accessOrder.end(), blockAddress), accessOrder.end());
            accessOrder.push_back(blockAddress);
        }
        return true;
    } else {
        // Miss
        misses++;
        replace(blockAddress);
        return false;
    }
}

void Cache::replace(int address) {
    if (cache.size() >= size / blockSize) {
        int evictAddress;
        if (policy == FIFO || policy == LRU) {
            evictAddress = accessOrder.front();
            accessOrder.pop_front();
        } else if (policy == RR) {
            evictAddress = cache.begin()->first;
        }
        cache.erase(evictAddress);
    }
    cache[address] = address;
    accessOrder.push_back(address);
}

MemoryHierarchy::MemoryHierarchy(int l1Size, int l2Size, int l3Size, int blockSize)
    : l1Cache(l1Size, blockSize, Cache::LRU), l2Cache(l2Size, blockSize, Cache::FIFO), l3Cache(l3Size, blockSize, Cache::RR) {}

bool MemoryHierarchy::accessMemory(int address) {
    if (l1Cache.accessMemory(address)) return true;
    if (l2Cache.accessMemory(address)) return true;
    return l3Cache.accessMemory(address);
}
