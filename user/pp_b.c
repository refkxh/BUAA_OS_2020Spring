#include "lib.h"

void umain() {
	int m = 0;
	while (m < 5) {
		ipc_send_double(0x800, 0x1802, m, 0, PTE_V);
		ipc_recv(NULL, 0, NULL);
		m = ipc_recv(NULL, 0, NULL);
		m++;
	}
}
