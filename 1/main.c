#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
struct b { int64_t v; struct b *l; struct b *r; };
int tinsert(struct b **b, int64_t v) {
	while (1) {
		if (!*b) {
			*b = malloc(sizeof(**b));
			**b = (struct b) { v, 0, 0 };
			return 0;
		}
		if ((*b)->v == v) return 1;
		if ((*b)->v < v) b = &(*b)->l;
		else b = &(*b)->r;
	}
}
int tsearch(struct b *b, int64_t v) {
	while (1) {
		if (!b) return 0;
		if (b->v == v) return 1;
		if (b->v < v) b = b->l;
		else b = b->r;
	}
}
int main(void) {
	size_t cap = 32, len = 0;
	int64_t *l = malloc(cap*sizeof(*l));
	int x = 0;
	while (scanf("%d", &x) == 1) {
		if (len >= cap) l = realloc(l, (cap*=2)*sizeof(*l));
		l[len++] = (int64_t)x;
	};
	int64_t freq = 0;
	for (size_t i = 0; i < len; i++) freq += l[i];
	printf("%ld\n", freq);
	freq = 0;
	struct b *b = 0;
	while (1) {
		for (size_t i = 0; i < len; i++) {
			freq += l[i];
			if (tinsert(&b, freq)) goto L;
		}
	}
L:
	printf("%ld\n", freq);
}
