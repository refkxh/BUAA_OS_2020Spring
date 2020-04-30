#include "lib.h"

void umain() {
	int m = 0;
	while (m < 5) {
		m = ipc_recv(NULL, 0, NULL);
		m++;
		ipc_send(0x1001, m, 0, PTE_V);
	}
}
