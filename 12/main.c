#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define size 2048

int main(void) {
	char *raw_state = malloc(128);
	scanf("initial state: %127s\n\n", raw_state);
	char subs[0xff] = {0}, f[6], t;
	while (scanf("%5s => %c\n", f, &t) == 2) {
		unsigned char x = 0;
		for (int i = 0; i < 5; i++)
			x |= (f[i] == '#') << i;
		subs[x] = t == '#';
	}
	char *c = calloc(size, 1), *l = calloc(size, 1);
	for (size_t i = 0; i < strlen(raw_state); i++)
		l[i+size/2] = raw_state[i] == '#';
	long long lsum = 0;
	for (size_t g = 1; ; g++) {
		for (size_t i = 2; i < size-2; i++)
			c[i] = subs[l[i-2]|l[i-1]<<1|l[i]<<2|l[i+1]<<3|l[i+2]<<4];
		char *tmp = l;
		l = c, c = tmp;
		memset(c, 0, size);
		long long sum = 0;
		for (size_t i = 0; i < size; i++)
			if (l[i])sum += i-size/2;
		if (g == 20) printf("%lld\n", sum);
		if (g == 500) {
			printf("%lld\n", (50000000000-g)*(sum-lsum)+sum);
			break;
		}
		lsum = sum;
	}
}
