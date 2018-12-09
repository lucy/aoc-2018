#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

struct p { uint32_t next, prev; };

uint32_t insert(struct p *a, uint32_t y, uint32_t x) {
	uint32_t z = a[x].next;
	a[z].prev = a[x].next = y;
	a[y].prev = x;
	a[y].next = z;
	return y;
}

uint32_t delete(struct p *a, uint32_t y) {
	uint32_t x = a[y].prev, z = a[y].next;
	a[z].prev = x;
	a[x].next = z;
	return z;
}

uint64_t run(uint32_t p, uint32_t h) {
	uint64_t *scores = calloc(p, sizeof(*scores));
	struct p *a = malloc((h+1) * sizeof(*a));
	uint32_t l = 0;
	a[l].prev = a[l].next = l;
	for (uint32_t n = 1; n <= h; n++) {
		if (n%23 == 0) {
			for (uint32_t i = 0; i < 7; i++) l = a[l].prev;
			scores[n%p] += n + l;
			l = delete(a, l);
		} else {
			l = insert(a, n, a[l].next);
		}
	}
	uint64_t max = 0;
	for (uint32_t i = 0; i < p; i++)
		if (scores[i] > max)
			max = scores[i];
	free(a);
	free(scores);
	return max;
}

#define format "%" SCNu32 " players; last marble is worth %" SCNu32 " points"

int main(void) {
	uint32_t p, h;
	scanf(format, &p, &h);
	printf("%" PRIu64 "\n", run(p, h));
	printf("%" PRIu64 "\n", run(p, h*100));
}
