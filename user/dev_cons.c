#include "lib.h"

int ugetStr(char *buff) {
	int len = 0;
	while (true) {
		char ch;
		syscall_read_dev(&ch, 0x10000000, 1);
		if (ch == 0) continue;
		syscall_write_dev(&ch, 0x10000000, 1);
		buff[len++] = ch;
		if (ch == '\r') {
			buff[len] = '\0';
			return len;
		}
	}
}

static void u_myoutput(void *arg, const char *s, int l)
{
	int i;

	// special termination call
	if ((l == 1) && (s[0] == '\0')) {
		return;
	}

	for (i = 0; i < l; i++) {
		syscall_write_dev(&s[i], 0x10000000, 1);

		if (s[i] == '\n') {
			syscall_write_dev('\n', 0x10000000, 1);
		}
	}
}

void uwritef(char *fmt, ...){
	va_list ap;
	va_start(ap, fmt);
	user_lp_Print(u_myoutput, 0, fmt, ap);
	va_end(ap);
}
