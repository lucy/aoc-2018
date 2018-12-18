#define _POSIX_SOURCE
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

struct find_result { int x, y, found; };

struct q { struct n *first, *last; };

struct n { int x, y, d; struct n *prev, *next; };

void qappend(struct q *q, int x, int y, int d) {
	struct n *n = calloc(1, sizeof(struct n));
	n->x = x; n->y = y; n->d = d;
	n->prev = q->last;
	if (q->last) q->last->next = n;
	q->last = n;
	if (!q->first) q->first = n;
}

void qpop(struct q *q, int *x, int *y, int *d) {
	struct n *n = q->first;
	*x = n->x;
	*y = n->y;
	*d = n->d;
	if (q->first == q->last) {
		q->last = q->first->next;
	}
	q->first = q->first->next;
	free(n);
}

int qempty(struct q *q) {
	return !q->first;
}

int testx[4] = {0, -1, 1, 0};
int testy[4] = {-1, 0, 0, 1};

struct find_result find(char enemy, int xo, int yo, int h, int w, char **map) {
	int seen[100][100] = {0};
	int meta[100][100][3] = {0};
	struct q q = {0};
	qappend(&q, xo, yo, 0);
	while (!qempty(&q)) {
		int x, y, d;
		qpop(&q, &x, &y, &d);
		for (int i = 0; i < 4; i++) {
			int nx = x + testx[i];
			int ny = y + testy[i];
			if (map[ny][nx] != '.') continue;
			int d1 = meta[ny][nx][0];
			if (!d1 || d1 > d + 1) {
				meta[ny][nx][0] = d + 1;
				meta[ny][nx][1] = x;
				meta[ny][nx][2] = y;
			}
			int n = 0;
			for (struct n *p = q.first; p; p = p->next) {
				if (p->x == nx && p->y == ny && p->d == d + 1)
					n++;
			}
			if (!seen[ny][nx] && n == 0)
				qappend(&q, nx, ny, d + 1);
		}
		seen[y][x] = 1;
	}
	int dmin = INT_MAX, xmin = INT_MAX, ymin = INT_MAX;
	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			int d1 = meta[y][x][0];
			if (!d1) continue;
			if (map[y][x] != '.') continue;
			for (int i = 0; i < 4; i++) {
				int nx = x + testx[i];
				int ny = y + testy[i];
				if (map[ny][nx] == enemy)
					if (dmin > d1) {
						dmin = d1;
						xmin = x;
						ymin = y;
					}
			}
		}
	}
	if (dmin == INT_MAX)
		return (struct find_result) { 0, 0, 0 };
	while (meta[ymin][xmin][0] > 1) {
		int x, y;
		x = meta[ymin][xmin][1];
		y = meta[ymin][xmin][2];
		xmin = x;
		ymin = y;
	}
	return (struct find_result) { xmin, ymin, 1 };
}

int debug_print(char **map, int **hp, int w, int h) {
	for (int y = 0; y < h; y++) {
		printf("%s", map[y]);
	}
	/*
	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			if (hp[y][x] != INT_MAX) {
				printf("%c (%d)\n", map[y][x], hp[y][x]);
			}
		}
	}
	*/
}

int run(char **inmap, int h, int w, int p2) {
	int hw = h;
	int **hp = calloc(100,sizeof(*hp));
	char **map = calloc(100,sizeof(*map));
	for (int y = 0; y < h; y++) {
		hp[y] = calloc(100,sizeof(**hp));
		map[y] = strdup(inmap[y]);
		for (int x = 0; x < w; x++) {
			hp[y][x] = INT_MAX;
			if (map[y][x] == 'G' || map[y][x] == 'E')
				hp[y][x] = 200;
		}
	}

	int es = 0;
	if (p2)
		for (int y0 = 0; y0 < h; y0++)
			for (int x0 = 0; x0 < w; x0++)
				if (map[y0][x0] == 'E') es++;

	int gen = 0;
	while (1) {
		//printf("after %d rounds(s):\n", gen);
		//debug_print(map, hp, h, w);
		int done[100][100] = {0};
		for (int y = 0; y < h; y++) {
			for (int x = 0; x < w; x++) {
				if (done[y][x]) continue;
				char unit = map[y][x];
				if (unit != 'G' && unit != 'E') continue;
				int g = 0, e = 0;
				for (int y0 = 0; y0 < h; y0++) {
					for (int x0 = 0; x0 < w; x0++) {
						char unit = map[y0][x0];
						if (unit == 'E') e++;
						if (unit == 'G') g++;
					}
				}
				if (p2 && e < es) {
					return 0;
				}
				if (g == 0 || e == 0) goto L;
				int xe = 0, ye = 0, h = INT_MAX;
				for (int i = 0; i < 4; i++) {
					int xt = x+testx[i];
					int yt = y+testy[i];
					if (hp[yt][xt] < h && map[yt][xt] != unit) {
						xe = xt, ye = yt, h = hp[yt][xt];
					}
				}

				if (h == INT_MAX) {
					struct find_result r = find(unit == 'G' ? 'E' : 'G', x, y, hw, w, map);
					if (!r.found) continue;
					hp[r.y][r.x] = hp[y][x];
					hp[y][x] = INT_MAX;
					map[r.y][r.x] = map[y][x];
					done[r.y][r.x] = 1;
					map[y][x] = '.';
					xe = 0, ye = 0, h = INT_MAX;
					for (int i = 0; i < 4; i++) {
						int xt = r.x+testx[i];
						int yt = r.y+testy[i];
						if (hp[yt][xt] < h && map[yt][xt] != unit)
							xe = xt, ye = yt, h = hp[yt][xt];
					}
				}

				if (h == INT_MAX) continue;
				if (unit == 'E') h -= p2 ? p2 : 3;
				else h -= 3;
				if (h < 0) {
					hp[ye][xe] = INT_MAX;
					map[ye][xe] = '.';
				} else {
					hp[ye][xe] = h;
				}
			}
		}
		gen++;
	}
L:;
	int total_hp = 0;
	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			if (hp[y][x] != INT_MAX) {
				total_hp += hp[y][x];
			}
		}
		free(map[y]);
		free(hp[y]);
	}
	free(map);
	free(hp);
	printf("%d full rounds, %d total hp, result: %d\n", gen, total_hp, gen*total_hp);
	return 1;
}


int main(void) {
	char **map = calloc(100,sizeof(*map));
	int h = 0;
	int w = 0;
	while (1) {
		char *buf = 0;
		size_t len = 0;
		ssize_t n = getline(&buf, &len, stdin);
		if (n == -1) break;
		map[h++] = buf;
		w = n-1;
	}
	run(map, h, w, 0);
	int i = 4;
	for (; !run(map, h, w, i); i++);
	printf("%d\n", i);
}
