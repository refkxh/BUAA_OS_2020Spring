#pragma G++ optimize(3)

#include "pageReplace.h"

const unsigned int PAGE_SHIFT = 12;
const unsigned int N_PHY_PAGE = 64;
const unsigned int N_PAGE = 32 << 10;
const int INF = 1e9 + 7;

#define GET_PAGE(addr) (((unsigned long) (addr)) >> PAGE_SHIFT)

long lastUsed[N_PHY_PAGE];  // <physical index, last referenced time>

int frames[N_PAGE];  // <page number, physical index>

void pageReplace(long *physic_memory, long nwAdd) {
    static long clock = 0;
    static int top = 0;
    long pageNum = GET_PAGE(nwAdd);
    if (frames[pageNum] > 0) {
        lastUsed[frames[pageNum]] = clock++;
        return;
    }
    int victim;
    if (top >= N_PHY_PAGE) {
        int earliest = INF;
        for (int i = 0; i < N_PHY_PAGE; i++) {
            if (lastUsed[i] < earliest) {
                earliest = lastUsed[i];
                victim = i;
            }
        }
        frames[physic_memory[victim]] = 0;
    }
    else {
        victim = top++;
    }
    frames[pageNum] = victim;
    lastUsed[victim] = clock++;
    physic_memory[victim] = pageNum;
}

