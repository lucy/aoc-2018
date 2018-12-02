#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct bt { int64_t v; struct bt *l; struct bt *r; };

int bt_insert(struct bt **b, int64_t v) {
	while (1) {
		if (!*b) {
			*b = malloc(sizeof(**b));
			**b = (struct bt) { .v = v, .l = 0, .r = 0 };
			return 0;
		}
		if ((*b)->v == v) return 1;
		if ((*b)->v > v) b = &(*b)->r;
		else if ((*b)->v < v) b = &(*b)->l;
	}
} 

int bt_search(struct bt *b, int64_t v) {
	while (1) {
		if (!b) return 0;
		if (b->v == v) return 1;
		if (b->v > v) b = b->r;
		else if (b->v < v) b = b->r;
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
	struct bt *b = 0;
	while (1) {
		for (size_t i = 0; i < len; i++) {
			freq += l[i];
			if (bt_insert(&b, freq)) goto L;
		}
	}
L:
	printf("%ld\n", freq);
}
