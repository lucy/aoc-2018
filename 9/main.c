#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>

struct p { int64_t v; struct p *prev, *next; };

static struct p *insert(struct p *y, struct p *x, int64_t v) {
	struct p *z = x->next;
	x->next = z->prev = y;
	*y = (struct p) { v, x, z };
	return y;
}

static struct p *delete(struct p *y) {
	struct p *z = y->next;
	y->prev->next = z;
	z->prev = y->prev;
	return z;
}

static void run(int p, int64_t h) {
	int64_t *scores = calloc(p, sizeof(*scores));
	struct p *a = malloc((h+1) * sizeof(*a));
	int n = 0;
	struct p *l = &a[n++];
	*l = (struct p) { 0, l, l };
	for (; n <= h; n++) {
		if (n%23 != 0) {
			l = insert(&a[n], l->next, n);
		} else {
			for (int i = 0; i < 7; i++) l = l->prev;
			scores[n%p] += n + l->v;
			l = delete(l);
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
