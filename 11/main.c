#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define d 300

static inline int power(int x, int y, int s) {
	return ((x+10)*y+s)*(x+10)/100%10-5;
}

static inline int sum_square(int *t, int x, int y, int s) {
	return t[(x+s)*d+y+s] - t[x*d+y+s] - t[(x+s)*d+y] + t[x*d+y];
} 

static int find_max_square(int *t, int s, int m, int *sm, int *xm, int *ym) {
	for (int p, x = 0; x < d-s; x++) for (int y = 0; y < d-s; y++)
		if ((p = sum_square(t, x, y, s)) > m)
			*sm = s, *xm = x, *ym = y, m = p;
	return m;
}

int main(void) {
	int in, *t = calloc((d+1)*(d+1), sizeof(int));
	scanf("%d", &in);
	for (int x = 1; x < d; x++) for (int y = 1; y < d; y++)
		t[x*d+y] = power(x, y, in) + t[(x-1)*d+y] + t[x*d+y-1] - t[(x-1)*d+y-1];
	int xm, ym, sm = 3;
	int m = find_max_square(t, 3, INT_MIN, &sm, &xm, &ym);
	printf("%d,%d\n", xm+1, ym+1);
	for (int s = 1; s < d; s++)
		m = find_max_square(t, s, m, &sm, &xm, &ym);
	printf("%d,%d,%d\n", xm+1, ym+1, sm);
}
