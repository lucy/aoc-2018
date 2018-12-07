#define _POSIX_SOURCE
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

int react(char *p, char *buf, char skip) {
	char *s = buf;
	for (; *p; p++) {
		char a = *p;
		if ((a | 0x20) == skip) continue;
		if (s > buf && (a ^ s[-1]) == 32) { s--; continue; }
		*s++ = a;
	}
	*s = 0;
	return s - buf;
}

int main(void) {
	ssize_t len;
	char *p = 0;
	size_t cap = 0;
	if ((len = getline(&p, &cap, stdin)) == -1) return 1;
	if (p[len-1] == '\n') p[--len] = 0;
	char *buf = malloc(len);
	printf("%d\n", react(p, buf, 0));
	int min = INT_MAX, n;
	for (char a = 'a'; a <= 'z'; a++)
		if ((n = react(buf, p, a)) < min)
			min = n;
	printf("%d\n", min);
}
