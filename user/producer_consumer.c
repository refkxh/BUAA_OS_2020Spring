#include "lib.h"

u_int produced = 0;
u_int consumed = 0;

void umain() {
	u_int mutex = syscall_init_PV_var(1);
	u_int empty = syscall_init_PV_var(10);
	u_int full = syscall_init_PV_var(0);
	u_int pid = fork();
	while (1) {
		if (pid) {
			syscall_P(empty);
			syscall_P(mutex);
			writef("producing %dth item.\n", ++produced);
			writef("now empty is %d and full is %d\n", syscall_check_PV_value(empty), syscall_check_PV_value(full));
			syscall_V(mutex);
			syscall_V(full);
			if (produced >= 100) {
				if (consumed >= 100) {
					syscall_release_PV_var(mutex);
					syscall_release_PV_var(produced);
					syscall_release_PV_var(consumed);
				}
				return;
			}
		}
		else {
			syscall_P(full);
			syscall_P(mutex);
			writef("consuming %dth item.\n", ++consumed);
			writef("now empty is %d and full is %d\n", syscall_check_PV_value(empty), syscall_check_PV_value(full));
			syscall_V(mutex);
			syscall_V(empty);
			if (consumed >= 100) {
				if (produced >= 100) {
					syscall_release_PV_var(mutex);
					syscall_release_PV_var(produced);
					syscall_release_PV_var(consumed);
				}
				return;
			}
		}
	}
}
