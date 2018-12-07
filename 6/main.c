#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

struct c { int x, y, n; };

int dist(int x, int y, struct c c) { return abs(x - c.x) + abs(y - c.y); }

int main(void) {
	size_t len = 0, cap = 32;
	struct c *l = malloc(cap*sizeof(*l)), c;
	while(scanf("%d, %d\n", &c.x, &c.y) == 2) {
		if (len >= cap) l = realloc(l, (cap*=2)*sizeof(*l));
		l[len++] = c;
	}

	int w = 0, h = 0;
	for (size_t i = 0; i < len; i++) {
		if (l[i].x > w) w = l[i].x;
		if (l[i].y > h) h = l[i].y;
	}

	for (int x = 0; x <= w; x++)
		for (int y = 0; y <= h; y++) {
			int min = INT_MAX, m = 0, n = 0, t;
			for (size_t i = 0; i < len; i++)
				if ((t = dist(x, y, l[i])) < min)
					min = t, m = i, n = 1;
				else if (t == min)
					n++;
			if (n != 1) continue;
			if (x == 0 || y == 0 || x == w || y == h) l[m].n = -1;
			if (l[m].n != -1) l[m].n++;
		}

	int max = 0, mi;
	for (size_t i = 1; i < len; i++)
		if (l[i].n > max) { max = l[i].n; mi = i; }
	printf("%d %d,%d\n", max, l[mi].x, l[mi].y);

	int n = 0;
	for (int x = 0; x <= w; x++)
		for (int y = 0; y <= h; y++) {
			int a = 0;
			for (size_t i = 0; i < len; i++)
				a += dist(x, y, l[i]);
			if (a < 10000) n++;
		}
	printf("%d\n", n);
}
