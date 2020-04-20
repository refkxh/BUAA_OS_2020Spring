#pragma GCC optimize(2)

#include "pageReplace.h"

#include <map>
#include <set>
#include <stack>

const unsigned int PAGE_SHIFT = 12;
const unsigned int N_PHY_PAGE = 64;

#define GET_PAGE(addr) (((unsigned long) (addr)) >> PAGE_SHIFT)

std::stack<int> freeStack;  // physical index
std::map<long, std::pair<long, int>> frames;  // <page number, <last referenced time, physical index>>
std::set<std::pair<long, long>> lastUsed;  // <last referenced time, page number>

void pageReplace(long *physic_memory, long nwAdd) {
    static long clock = 0;
    if (clock == 0) {
        for (int i = 0; i < N_PHY_PAGE; i++) {
            freeStack.push(i);
        }
    }
    long pageNum = GET_PAGE(nwAdd);
    if (frames.count(pageNum)) {
        lastUsed.erase(std::pair<long, long>(frames[pageNum].first, pageNum));
        frames[pageNum].first = clock;
        lastUsed.insert(std::pair<long, long>(clock++, pageNum));
        return;
    }
    if (freeStack.empty()) {
        auto victim = lastUsed.begin();
        physic_memory[frames[victim->second].second] = 0;
        freeStack.push(frames[victim->second].second);
        frames.erase(victim->second);
        lastUsed.erase(victim);
    }
    long index = freeStack.top();
    freeStack.pop();
    frames[pageNum] = std::pair<long, int>(clock, index);
    lastUsed.insert(std::pair<long, long>(clock++, pageNum));
    physic_memory[index] = pageNum;
}

