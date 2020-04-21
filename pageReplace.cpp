#pragma G++ optimize(2)

#include "pageReplace.h"

#include <climits>

#define PAGE_SHIFT 12
#define N_PHY_PAGE 64
#define N_PAGE 32768
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
        int earliest = INT_MAX;
        for (int i = 0; i < N_PHY_PAGE; i++) {
            if (lastUsed[i] < earliest) {
                earliest = lastUsed[i];
                victim = i;
            }
        }
        frames[physic_memory[victim]] = 0;
    }
    else victim = top++;
    frames[pageNum] = victim;
    lastUsed[victim] = clock++;
    physic_memory[victim] = pageNum;
}

