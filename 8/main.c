#include <stdio.h>
#include <stdlib.h>

int p1(int *r, int *l, int ii) {
	int c = l[ii++], m = l[ii++];
	for (int i = 0; i < c; i++) ii = p1(r, l, ii);
	for (int i = 0; i < m; i++) *r += l[ii++];
	return ii;
}

int p2(int *r, int *l, int ii) {
	int c = l[ii++], m = l[ii++];
	if (c == 0) {
		for (int i = 0; i < m; i++) *r += l[ii++];
		return ii;
	}
	int *cc = calloc(c, sizeof(int)), j;
	for (int i = 0; i < c; i++) ii = p2(&cc[i], l, ii);
	for (int i = 0; i < m; i++) if ((j = l[ii++]) && j <= c) *r += cc[j-1];
	free(cc);
	return ii;
}

int main(void) {
	int l[1<<15] = {0}, len = 0, r1 = 0, r2 = 0;
	while (scanf("%d", &l[len++]) == 1);
	p1(&r1, l, 0);
	p2(&r2, l, 0);
	printf("%d\n%d\n", r1, r2);
}
