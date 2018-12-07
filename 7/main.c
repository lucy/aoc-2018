#include <stdio.h>
#include <stdint.h>

#define format "Step %c must be finished before step %c can begin.\n"
#define len 26

void p1(uint32_t a, uint32_t deps[len]) {
	while (a)
		for (int i = 0; i < len; i++)
			if (((a>>i)&1U) && !(deps[i]&a)) {
				printf("%c", i+'A');
				a &= ~(1UL << i);
				break;
			}
	printf("\n");
}

void p2(uint32_t a, uint32_t deps[len]) {
	int work[len], workers = 5, t = 0;
	for (int i = 0; i < len; i++) work[i] = -1;
	while (a) {
		for (int i = 0; i < len && workers; i++)
			if (((a>>i)&1U) && !(deps[i]&a) && work[i] == -1)
				workers--, work[i] = i+1+60;  
		int w = 0;
		for (int i = 0; i < len; i++) {
			if (work[i] == -1) continue;
			if (--work[i] == 0) workers++, a &= ~(1UL << i);
			w++;
		}
		if (w) t++;
	}
	printf("%d\n", t);
}

int main(void) {
	uint32_t deps[len] = {0}, a = 0;
	unsigned char x, y;
	while (scanf(format, &y, &x) == 2)
		a |= 1UL<<(x-'A') | (deps[x-'A'] |= 1UL<<(y-'A'));
	p1(a, deps);
	p2(a, deps);
}
