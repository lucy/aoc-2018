#include <limits.h>
#include <stdio.h>

#define d 300

static void find_max_square(int (*t)[d+1], int from, int to, 
	                        int *sm, int *xm, int *ym) {
	int m = INT_MIN;
	for (int s = from; s <= to; s++)
		for (int x = 1; x <= d-s; x++)
			for (int y = 1; y <= d-s; y++) {
				int p = t[x+s][y+s] - t[x][y+s] - t[x+s][y] + t[x][y];
				if (p > m) *sm = s, *xm = x, *ym = y, m = p;
			}
}

int main(void) {
	int in, t[(d+1)][(d+1)] = {0};
	scanf("%d", &in);
	for (int x = 1; x <= d; x++) for (int y = 1; y <= d; y++)
		t[x][y] = (((in+10)*y+in)*(x+10)/100%10-5) +
			t[(x-1)][y] + t[x][y-1] - t[(x-1)][y-1];
	int xm, ym, sm;
	find_max_square(t, 3, 3, &sm, &xm, &ym);
	printf("%d,%d\n", xm+1, ym+1);
	find_max_square(t, 1, d, &sm, &xm, &ym);
	printf("%d,%d,%d\n", xm+1, ym+1, sm);
}
