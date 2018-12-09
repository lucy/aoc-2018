#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

struct p { int64_t v; int prev, next; };

int insert(struct p *a, int y, int x, int64_t v) {
	int z = a[x].next;
	a[x].next = a[z].prev = y;
	a[y] = (struct p) { v, x, z };
	return y;
}

int delete(struct p *a, int y) {
	int z = a[y].next;
	a[a[y].prev].next = z;
	a[z].prev = a[y].prev;
	return z;
}

void run(int p, int64_t h) {
	int64_t *scores = calloc(p, sizeof(*scores));
	struct p *a = malloc((h+1) * sizeof(*a));
	int l = 0;
	a[l] = (struct p) { 0, l, l };
	for (int n = 1; n <= h; n++) {
		if (n%23 != 0) {
			l = insert(a, n, a[l].next, n);
		} else {
			for (int i = 0; i < 7; i++) l = a[l].prev;
			scores[n%p] += n + a[l].v;
			l = delete(a, l);
		}
	}
	int64_t max = 0;
	for (int i = 0; i < p; i++)
		if (scores[i] > max)
			max = scores[i];
	printf("%" PRId64 "\n", max);
	free(a);
	free(scores);
}

int main(void) {
	int p, h;
	scanf("%d players; last marble is worth %d points", &p, &h);
	run(p, h);
	run(p, (int64_t)h*100);
}
