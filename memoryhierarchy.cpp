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
        if (policy == FIFO) {
            evictAddress = accessOrder.front();
            accessOrder.pop_front();
        } else if (policy == LRU) {
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

MemoryHierarchy::MemoryHierarchy(int l1Size, int l2Size, int l3Size, int blockSize, Cache::ReplacementPolicy policy)
    : l1Cache(l1Size, blockSize, policy), l2Cache(l2Size, blockSize, policy), l3Cache(l3Size, blockSize, policy) {}

bool MemoryHierarchy::accessMemory(int address) {
    auto start = std::chrono::high_resolution_clock::now();

    if (l1Cache.accessMemory(address)) {
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::micro> elapsed = end - start;
        totalAccessTime += elapsed.count();
        numAccesses++;
        return true;
    }

    if (l2Cache.accessMemory(address)) {
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::micro> elapsed = end - start;
        totalAccessTime += elapsed.count();
        numAccesses++;
        return true;
    }

    if (l3Cache.accessMemory(address)) {
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::micro> elapsed = end - start;
        totalAccessTime += elapsed.count();
        numAccesses++;
        return true;
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::micro> elapsed = end - start;
    totalAccessTime += elapsed.count();
    numAccesses++;
    return false;
}
