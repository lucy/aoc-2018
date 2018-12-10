#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define format "position=<%d, %d> velocity=<%d, %d>\n"
#define letter_height 9

struct p { int x, y, xv, yv; };

#define min(x, y) ((x)<(y)?(x):(y))
#define max(x, y) ((x)>(y)?(x):(y))
#define px(p, t) (p.x+p.xv*t)
#define py(p, t) (p.y+p.yv*t)

int main(void) {
	int cap = 32, len = 0;
	struct p *l = malloc(cap*sizeof(*l));
	struct p a;
	while (scanf(format, &a.x, &a.y, &a.xv, &a.yv) == 4) {
		if (len >= cap) l = realloc(l, (cap *= 2) * sizeof(*l));
		l[len++] = a;
	}
	int t = 0;
	while (1) {
		int min = INT_MAX, max = 0;
		for (int i = 0; i < len; i++) {
			max = max(max, py(l[i], t));
			min = min(min, py(l[i], t));
		}
		if ((max - min) == letter_height) break;
		t++;
	}
	int wmin = INT_MAX, wmax = 0;
	int hmin = INT_MAX, hmax = 0;
	for (int i = 0; i < len; i++) {
		wmin = min(wmin, px(l[i], t));
		wmax = max(wmax, px(l[i], t));
		hmin = min(hmin, py(l[i], t));
		hmax = max(hmax, py(l[i], t));
	}
	int w = wmax - wmin + 1;
	int h = hmax - hmin + 1;
	char *grid = calloc(w * h, 1);
	for (int i = 0; i < len; i++) {
		int x = px(l[i], t) - wmin;
		int y = py(l[i], t) - hmin;
		grid[x*h+y] = 1;
	}
	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++)
			putc(grid[x*h+y] ? '#' : '.', stdout);
		putc('\n', stdout);
	}
	printf("%d\n", t);
}
