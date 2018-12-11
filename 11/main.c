#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define d 300
#define p(x, y, s) ((((x)+10)*(y)+s)*((x)+10)/100%10-5)

static inline int q(int *t, int x, int y, int s) {
	return t[(x+s)*d+(y+s)] - t[(x  )*d+(y+s)] -
	       t[(x+s)*d+(y  )] + t[(x  )*d+(y  )];
} 

static int f(int *t, int s, int p, int *xm, int *ym) {
	for (int m, x = 0; x < d-s; x++) for (int y = 0; y < d-s; y++)
		if ((m = q(t, x, y, s)) > p) *xm = x, *ym = y, p = m;
	return p;
}

int main(void) {
	int serial, *t = calloc((d+1)*(d+1), sizeof(int));
	scanf("%d", &serial);
	for (int x = 1; x < d; x++) for (int y = 1; y < d; y++)
		t[x*d+y] = p(x, y, serial) + t[(x-1)*d+y] + t[x*d+y-1] - t[(x-1)*d+y-1];
	int xm, ym, sm = 3, m = f(t, sm, INT_MIN, &xm, &ym);
	printf("%d,%d\n", xm+1, ym+1);
	for (int p, s = 1; s < d; s++)
		if ((p = f(t, s, m, &xm, &ym)) > m)
			sm = s, m = p;
	printf("%d,%d,%d\n", xm+1, ym+1, sm);
}
