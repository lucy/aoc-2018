#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

int main(void) {
	char *s = malloc(1024);
	scanf("initial state: %1023s\n\n", s);
	printf("%s\n", s);
	int cap = 32, len = 0;
	char **subs = malloc(cap * sizeof(*subs));
	char f[6], t;
	while (scanf("%5s => %c\n", f, &t) == 2) {
		if (len >= cap) subs = realloc(subs, (cap *= 2) * sizeof(*subs));
		subs[len] = malloc(6);
		memcpy(subs[len], f, 5);
		subs[len++][5] = t;
	}
	char cgen[4096*8];
	char lgen[4096*8];
	memset(lgen, '.', sizeof(lgen));
	memset(cgen, '.', sizeof(cgen));
	memcpy(lgen+2048*8, s, strlen(s));
	long lsum = 0;
	for (size_t z = 1; ; z++) {
		for (size_t i = 2; i < sizeof(cgen)-2; i++) {
			for (size_t j = 0; j < (size_t)len; j++) {
				char *m = subs[j];
				if (lgen[i-2] == m[0] && lgen[i-1] == m[1] && lgen[i] == m[2] &&
					lgen[i+1] == m[3] && lgen[i+2] == m[4]) {
					cgen[i] = m[5];
					break;
				}
			}
		}
		memcpy(lgen, cgen, sizeof(cgen));
		memset(cgen, '.', sizeof(cgen));
		long sum = 0;
		for (size_t i = 0; i < sizeof(cgen); i++) {
			if (lgen[i] == '#') {
				sum += i-2048*8;
			}
		}
		if (z == 20) printf("%ld\n", sum);
		if (z == 1000) {
			printf("%ld\n", (50000000000-z)*(sum-lsum)+sum);
			break;
		}
		lsum = sum;
	}
}
