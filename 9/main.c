#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

struct p { uint32_t next, prev; };

uint64_t run(uint32_t p, uint32_t h) {
	uint64_t *scores = calloc(p, sizeof(*scores));
	struct p *a = malloc((h+1) * sizeof(*a));
	uint32_t l = 0;
	a[l].prev = l, a[l].next = l;
	for (uint32_t n = 1; n <= h; n++) {
		if (n%23 != 0) {
			uint32_t x = a[l].next;
			uint32_t z = a[x].next;
			a[z].prev = n, a[x].next = n;
			a[n].next = z, a[n].prev = x;
			l = n;
		} else {
			for (uint32_t i = 0; i < 7; i++) l = a[l].prev;
			scores[n%p] += n + l;
			uint32_t x = a[l].prev;
			uint32_t z = a[l].next;
			a[z].prev = x, a[x].next = z;
			l = z;
		}
	}
	uint64_t max = 0;
	for (uint32_t i = 0; i < p; i++)
		if (scores[i] > max) max = scores[i];
	free(a);
	free(scores);
	return max;
}

int main(void) {
	long p, h;
	scanf("%ld players; last marble is worth %ld points", &p, &h);
	printf("%" PRIu64 "\n", run(p, h));
	printf("%" PRIu64 "\n", run(p, h*100));
}
