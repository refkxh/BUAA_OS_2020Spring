#pragma GCC optimize(2)

#include "pageReplace.h"

#include <map>

const unsigned int PAGE_SHIFT = 12;
const unsigned int N_PHY_PAGE = 64;
const int INF = 1e9 + 7;

#define GET_PAGE(addr) (((unsigned long) (addr)) >> PAGE_SHIFT)

int top = 0;
int freeStack[N_PHY_PAGE];  // physical index

long lastUsed[N_PHY_PAGE]; // <physical index, last referenced time>

std::map<long, int> frames;  // <page number, physical index>

void pageReplace(long *physic_memory, long nwAdd) {
    static long clock = 0;
    if (clock == 0) {
        for (int i = 0; i < N_PHY_PAGE; i++) {
            freeStack[top++] = i;
        }
    }
    long pageNum = GET_PAGE(nwAdd);
    if (frames.count(pageNum)) {
        lastUsed[frames[pageNum]] = clock++;
        return;
    }
    if (top == 0) {
        int victim;
        int earliest = INF;
        for (int i = 0; i < N_PHY_PAGE; i++) {
            if (lastUsed[i] < earliest) {
                earliest = lastUsed[i];
                victim = i;
            }
        }
        frames.erase(physic_memory[victim]);
        physic_memory[victim] = 0;
        freeStack[top++] = victim;
    }
    long index = freeStack[--top];
    frames[pageNum] = index;
    lastUsed[index] = clock++;
    physic_memory[index] = pageNum;
}

