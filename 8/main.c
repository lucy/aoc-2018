#include <stdio.h>
#include <stdlib.h>

int p(int *r1, int *r2, int *l, int ii) {
	int c = l[ii++], m = l[ii++], *cc = c ? calloc(c, sizeof(int)) : 0;
	for (int i = 0; i < c; i++) ii = p(r1, &cc[i], l, ii);
	for (int i = 0, j; i < m; i++) {
		j = l[ii++], *r1 += j;
		if (!c) *r2 += j;
		else if (j && j <= c) *r2 += cc[j-1];
	}
	free(cc);
	return ii;
}

int main(void) {
	int l[1<<15] = {0}, len = 0, r1 = 0, r2 = 0;
	while (scanf("%d", &l[len++]) == 1);
	p(&r1, &r2, l, 0);
	printf("%d\n%d\n", r1, r2);
}
