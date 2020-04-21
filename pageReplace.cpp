#pragma G++ optimize(3)

#include "pageReplace.h"

const unsigned int PAGE_SHIFT = 12;
const unsigned int N_PHY_PAGE = 64;
const unsigned int N_PAGE = 32 << 10;
const int INF = 1e9 + 7;

#define GET_PAGE(addr) (((unsigned long) (addr)) >> PAGE_SHIFT)

int top = 64;
int freeStack[N_PHY_PAGE] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
                             16, 17, 18, 19,20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,
                             32, 33, 34, 35, 36, 37, 38, 39,40, 41, 42, 43, 44, 45, 46, 47,
                             48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59,60, 61, 62, 63};

long lastUsed[N_PHY_PAGE];  // <physical index, last referenced time>

int frames[N_PAGE];  // <page number, physical index>

void pageReplace(long *physic_memory, long nwAdd) {
    static long clock = 0;
    long pageNum = GET_PAGE(nwAdd);
    if (frames[pageNum] > 0) {
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
        frames[physic_memory[victim]] = 0;
        physic_memory[victim] = 0;
        freeStack[top++] = victim;
    }
    long index = freeStack[--top];
    frames[pageNum] = index;
    lastUsed[index] = clock++;
    physic_memory[index] = pageNum;
}

