#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>

struct p { int64_t v; struct p *prev, *next; };

struct p *insert(struct p *l, int64_t v) {
	struct p *y = calloc(1, sizeof(*y));
	struct p *x = l->next;
	struct p *z = l->next->next;
	x->next = y;
	y->next = z;
	y->prev = x;
	z->prev = y;
	y->v = v;
	return y;
}
//        1            2            3
//  <-prev next-><-prev next-><-prev next->
//
// 1->next = 3
// 3->prev = 1
//
// 1->next = 2
// 2->prev = 1
// 3->prev = 2
// 2->next = 3
struct p *delete(struct p *l, int64_t *v) {
	for (int i = 0; i < 7; i++) l = l->prev;
	struct p *x = l->prev;
	struct p *y = l;
	struct p *z = l->next;
	x->next = z;
	z->prev = y;
	//printf("deleting %d\n", y->v);
	*v += y->v;
	free(y);
	return z;
}

void print(struct p *l) {
	struct p *p = l;
	do printf(" %ld", p->v);
	while ((p = p->next) != l);
	printf("\n");
}

int64_t run(int p, int64_t h) {
	int64_t *scores = calloc(p, sizeof(*scores));
	struct p *l = calloc(1, sizeof(*l));
	//struct p *init = l;
	l->prev = l->next = l;
	for (int64_t n = 1; n <= h; )
		for (int i = 0; i < p; i++, n++) {
			//printf("[%d] ", i+1);
			if (n%23 == 0) {
				scores[i] += n;
				l = delete(l, &scores[i]);
			} else {
				l = insert(l, n);
			}
			//print(init);
		}
	int64_t max = 0;
	int maxi = 0;
	for (int i = 0; i < p; i++) {
		if (scores[i] > max) {
			max = scores[i];
			maxi = i;
		}
	}
	printf("%d won with %" PRId64 "\n", maxi+1, max);
}

int main(void) {
	int p, h;
	scanf("%d players; last marble is worth %d points", &p, &h);
	run(p, h);
	run(p, (int64_t)h*100);
}

