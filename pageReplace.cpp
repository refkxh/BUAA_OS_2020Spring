#include "pageReplace.h"
#include <list>
#include <map>
#include <set>

const unsigned int PAGE_SHIFT = 12;
const unsigned int N_PHY_PAGE = 64;

#define GET_PAGE(addr) (((unsigned long) (addr)) >> PAGE_SHIFT)

std::list<int> freeList;  // physical index
std::map<long, std::pair<long, int>> frames;  // <page number, <last referenced time, physical index>>
std::set<std::pair<long, long>> lastUsed;  // <last referenced time, page number>

void pageReplace(long *physic_memory, long nwAdd) {
    static long clock = 0;
    if (clock == 0) {
        for (int i = 0; i < N_PHY_PAGE; i++) {
            freeList.push_back(i);
        }
    }
    long pageNum = GET_PAGE(nwAdd);
    if (frames.count(pageNum)) {
        frames[pageNum].first = clock;
        lastUsed.insert(std::pair<long, long>(clock++, pageNum));
        return;
    }
    else if (freeList.empty()) {
        while (true) {
            auto victim = lastUsed.begin();
            if (frames.count(victim->second) && victim->first == frames[victim->second].first) {
                physic_memory[frames[victim->second].second] = 0;
                freeList.push_back(frames[victim->second].second);
                frames.erase(victim->second);
                lastUsed.erase(victim);
                break;
            }
            lastUsed.erase(victim);
        }
    }
    long index = freeList.front();
    freeList.pop_front();
    frames[pageNum] = std::pair<long, int>(clock, index);
    lastUsed.insert(std::pair<long, long>(clock++, pageNum));
    physic_memory[index] = pageNum;
}

