#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define format "position=<%d, %d> velocity=<%d, %d>\n"
#define letter_height 9

struct p { int x, y, xv, yv; };
struct b { int x, y, w, h; };

#define min(x, y) ((x)<(y)?(x):(y))
#define max(x, y) ((x)>(y)?(x):(y))
#define px(p, t) (p.x+p.xv*t)
#define py(p, t) (p.y+p.yv*t)

struct b bbx(struct p *l, size_t len, int t) {
	int xmin = INT_MAX, xmax = 0;
	int ymin = INT_MAX, ymax = 0;
	for (size_t i = 0; i < len; i++) {
		xmin = min(xmin, px(l[i], t));
		xmax = max(xmax, px(l[i], t));
		ymin = min(ymin, py(l[i], t));
		ymax = max(ymax, py(l[i], t));
	}
	return (struct b) { xmin, ymin, xmax-xmin, ymax-ymin };
}

int ph(struct p *l, size_t len, int t) {
	int ymin = INT_MAX, ymax = 0;
	for (size_t i = 0; i < len; i++) {
		ymin = min(ymin, py(l[i], t));
		ymax = max(ymax, py(l[i], t));
	}
	return ymax - ymin;
}

int main(void) {
	size_t cap = 32, len = 0;
	struct p *l = malloc(cap*sizeof(*l));
	struct p a;
	while (scanf(format, &a.x, &a.y, &a.xv, &a.yv) == 4) {
		if (len >= cap) l = realloc(l, (cap *= 2) * sizeof(*l));
		l[len++] = a;
	}
	int t0 = 0, t1 = 1;
	int f = ph(l, len, t0);
	while (f > ph(l, len, t1)) t1*=2;
	while (t0 != t1) {
		int t = (t0+t1)/2;
		int m = ph(l, len, t);
		int r = ph(l, len, t+1);
		if (r > m)
			t1 = t;
		else
			t0 = t+1;
	}
	struct b b = bbx(l, len, t0);
	int w = b.w + 1, h = b.h + 1;
	char *grid = calloc(w * h, 1);
	for (size_t i = 0; i < len; i++) {
		int x = px(l[i], t1) - b.x;
		int y = py(l[i], t1) - b.y;
		grid[x*h+y] = 1;
	}
	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++)
			putc(grid[x*h+y] ? '#' : '.', stdout);
		putc('\n', stdout);
	}
	printf("%d\n", t0);
}
