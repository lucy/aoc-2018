#include <stdlib.h>
#include <stdio.h>

struct t { int mlen, clen, *m; struct t *c; };

int tree(struct t *n, int *input, int ii, int len) {
	if (ii >= len) return 0;
	n->clen = input[ii++], n->mlen = input[ii++];
	n->c = calloc(n->clen, sizeof(*n->c));
	n->m = calloc(n->mlen, sizeof(*n->m));
	for (int i = 0; i < n->clen; i++) ii = tree(&n->c[i], input, ii, len);
	for (int i = 0; i < n->mlen; i++) n->m[i] = input[ii++];
	return ii;
}

int p1(struct t *n) {
	int v = 0;
	for (int i = 0; i < n->clen; i++) v += p1(&n->c[i]);
	for (int i = 0; i < n->mlen; i++) v += n->m[i];
	return v;
}

int p2(struct t *n) {
	int v = 0;
	for (int i = 0; i < n->mlen; i++)
		if (!n->clen) v += n->m[i];
		else if (n->m[i] && n->m[i] <= n->clen)
			v += p2(&n->c[n->m[i]-1]);
	return v;
}

int main(void) {
	int l[1<<15] /* there are less than 32k values */, len = 0;
	while (scanf("%d", &l[len]) == 1) len++;
	struct t n = {0};
	tree(&n, l, 0, len); /* constructing the tree makes p1 and 2 much
	                        simpler */
	printf("%d\n%d\n", p1(&n), p2(&n));
}
