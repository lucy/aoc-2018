#define _POSIX_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define N 1
#define W (1 << 1)

int main(void) {
	size_t buflen = 0;
	char *s = 0;
	ssize_t len = getline(&s, &buflen, stdin);
	if (len == -1) return 1;
	int st[5000], si = 0;
	int xmin = 0, xmax = 0, ymin = 0, ymax = 0;
	for (int x = 0, y = 0, i = 0; i < len; i++) {
		char c = s[i];
		if (c == '^' || c == '$' || c == '\n') continue;
		if (c == '(') { st[si++] = x; st[si++] = y; continue; }
		if (c == '|') { y = st[si-1]; x = st[si-2]; continue; }
		if (c == ')') { si -= 2; continue; }
		y += (c == 'S') - (c == 'N');
		x += (c == 'E') - (c == 'W');
		if (x > xmax) xmax = x; else if (x < xmin) xmin = x;
		if (y > ymax) ymax = y; else if (y < ymin) ymin = y;
	}
	int w = xmax - xmin + 1, h = ymax - ymin + 1;
	unsigned char *doors = calloc((w+1)*(h+1), 1);
	for (int x = -xmin, y = -ymin, i = 0; i < len; i++) {
		char c = s[i];
		if (c == '^' || c == '$' || c == '\n') continue;
		if (c == '(') { st[si++] = x; st[si++] = y; continue; }
		if (c == '|') { y = st[si-1]; x = st[si-2]; continue; }
		if (c == ')') { si -= 2; continue; }
		int yv = (c == 'S') - (c == 'N');
		int xv = (c == 'E') - (c == 'W');
		doors[(y+(yv>0))*w+(x+(xv>0))] |= (yv ? N : W);
		x += xv, y += yv;
	}
	int *distances = calloc(w*h, sizeof(*distances));
	for (int i = 0; i < w*h; i++) distances[i] = INT_MAX;
	distances[-ymin*h-xmin] = 0;
	int xvs[4] = {0, -1, 1, 0}, yvs[4] = {-1, 0, 0, 1};
	st[si++] = -xmin, st[si++] = -ymin;
	while (si) {
		int y = st[--si], x = st[--si], d = distances[y*h+x];
		for (int i = 0; i < 4; i++) {
			int x1 = x+xvs[i], y1 = y+yvs[i];
			int yo = yvs[i] > 0, xo = xvs[i] > 0;
			int m = yvs[i] ? N : W;
			if (!(doors[(y+yo)*w+(x+xo)] & m)) continue;
			if (d + 1 < distances[y1*h+x1])
				distances[y1*h+x1] = d + 1,
				st[si++] = x1, st[si++] = y1;
		}
	}
	int m = 0, c = 0;
	for (int i = 0; i < w*h; i++) {
		int d = distances[i];
		if (d == INT_MAX) continue;
		if (d > m) m = d;
		if (d >= 1000) c++;
	}
	printf("%d\n%d\n", m, c);
}
