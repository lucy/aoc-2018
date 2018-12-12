#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define size 512

int main(void) {
	char *raw_state = malloc(128);
	scanf("initial state: %127s\n\n", raw_state);
	char subs[0xff] = {0}, f[6], t;
	while (scanf("%5s => %c\n", f, &t) == 2) {
		unsigned char x = 0;
		for (int i = 0; i < 5; i++)
			x = x << 1 | (f[i] == '#');
		subs[x] = t == '#';
	}
	char *c = malloc(size), *l = calloc(size, 1);
	for (size_t i = 0; i < strlen(raw_state); i++)
		l[i] = raw_state[i] == '#';
	long long lsum = 0;
	long long off = 0;
	for (size_t g = 1; ; g++) {
		size_t j = 0;
		while(!l[j]) j++;
		unsigned char s = 0;
		for (size_t i = j; i < size; i++) {
			s = (s << 1 | l[i]) & 31;
			c[i-j] = subs[s];
		}
		off -= 2 - j;
		char *tmp = l;
		l = c, c = tmp;
		long long sum = 0;
		for (size_t i = 0; i < size; i++)
			if (l[i]) sum += i+off;
		if (g == 20) printf("%lld\n", sum);
		if (memcmp(l, c, size) == 0) {
			printf("%lld\n", (50000000000-g)*(sum-lsum)+sum);
			break;
		}
		lsum = sum;
	}
}
