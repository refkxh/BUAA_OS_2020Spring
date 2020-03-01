#include <stdio.h>
#include <string.h>
char s[10];
int isPalindrome(int num) {
	int i;
	sprintf(s, "%d", num);
	for (i = 0; i < strlen(s) / 2; i++) {
		if (s[i] != s[strlen(s) - i - 1]) return 0;
	}
	return 1;
}
int main() {
	int n;
	scanf("%d", &n);
	if(isPalindrome(n)) {
		printf("Y");
	}
	else {
		printf("N");
	}
	return 0;
}
