#pragma GCC optimize (2)

#include "pageReplace.h"

#define PAGE_SHIFT 12
#define N_PHY_PAGE 64
#define N_PAGE 32768
#define GET_PAGE(addr)  ((addr) >> PAGE_SHIFT)

bool lastUsed[N_PHY_PAGE];

int frames[N_PAGE];  // <page number, physical index>

void pageReplace(long *physic_memory, long nwAdd) {
    static int top = 0;
    static int ptr = 0;
    long pageNum = GET_PAGE(nwAdd);
    if (frames[pageNum] > 0) {
        lastUsed[frames[pageNum]] = true;
        if (pageNum + 1 < N_PAGE) {
            while (lastUsed[ptr]) {
                lastUsed[ptr] = false;
                ptr = (ptr + 1) & (N_PHY_PAGE - 1);
            }
            frames[physic_memory[ptr]] = 0;
            frames[pageNum + 1] = ptr;
            physic_memory[ptr] = pageNum + 1;
        }
        return;
    }
    if (top >= N_PHY_PAGE) {
        while (lastUsed[ptr]) {
            lastUsed[ptr] = false;
            ptr = (ptr + 1) & (N_PHY_PAGE - 1);
        }
        frames[physic_memory[ptr]] = 0;
        lastUsed[ptr] = true;
        frames[pageNum] = ptr;
        physic_memory[ptr] = pageNum;
    }
    else {
        frames[pageNum] = top;
        physic_memory[top++] = pageNum;
    }
}

