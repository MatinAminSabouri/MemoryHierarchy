#ifndef MEMORYHIERARCHY_H
#define MEMORYHIERARCHY_H

#include <vector>
#include <unordered_map>
#include <deque>
#include <algorithm>
#include <chrono>

class Cache {
public:
    enum ReplacementPolicy {
        LRU,
        FIFO,
        RR
    };

    Cache(int size, int blockSize, ReplacementPolicy policy);
    bool accessMemory(int address);

    int getHits() const { return hits; }
    int getMisses() const { return misses; }

private:
    int size;
    int blockSize;
    ReplacementPolicy policy;
    std::unordered_map<int, int> cache;
    std::deque<int> accessOrder;
    int hits = 0;
    int misses = 0;

    void replace(int address);
};

class MemoryHierarchy {
public:
    MemoryHierarchy(int l1Size, int l2Size, int l3Size, int blockSize, Cache::ReplacementPolicy policy);
    bool accessMemory(int address);

    int getL1Hits() const { return l1Cache.getHits(); }
    int getL1Misses() const { return l1Cache.getMisses(); }
    int getL2Hits() const { return l2Cache.getHits(); }
    int getL2Misses() const { return l2Cache.getMisses(); }
    int getL3Hits() const { return l3Cache.getHits(); }
    int getL3Misses() const { return l3Cache.getMisses(); }

    double getAverageAccessTime() const { return totalAccessTime / numAccesses; }
    double getL1HitRate() const { return static_cast<double>(l1Cache.getHits()) / numAccesses; }
    double getL2HitRate() const { return static_cast<double>(l2Cache.getHits()) / numAccesses; }
    double getL3HitRate() const { return static_cast<double>(l3Cache.getHits()) / numAccesses; }

private:
    Cache l1Cache;
    Cache l2Cache;
    Cache l3Cache;
    double totalAccessTime = 0;
    int numAccesses = 0;
};

#endif // MEMORYHIERARCHY_H
