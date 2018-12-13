#define _POSIX_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define STATE_L1 1
#define STATE_S2 2
#define STATE_R3 3

int main(void) {
	int len = 0, cap = 32;
	char **g = malloc(cap*(sizeof(*g)));
	ssize_t n;
	size_t llen = 0;
	char *l = 0;
	while ((n = getline(&l, &llen, stdin)) != -1) {
		if (len >= cap) g = realloc(g, (cap *= 2) * sizeof(*g));
		if (l[n-1] == '\n') l[--n] = 0;
		g[len++] = strdup(l);
		l = 0;
		llen = 0;
	}
	for (int i = 0; i < len; i++) {
		printf("f %s\n", g[i]);
	}
	int w = strlen(g[0]);
	int h = len;
	char **carts = calloc(sizeof(*carts)*h, 1);
	char **done = calloc(sizeof(*carts)*h, 1);
	char **state = calloc(sizeof(*state)*h, 1);
	for (int y = 0; y < h; y++) {
		carts[y] = calloc(sizeof(**carts)*w, 1);
		done[y] = calloc(sizeof(**carts)*w, 1);
		state[y] = calloc(sizeof(**state)*w, 1);
		for (int x = 0; x < w; x++) {
			if (g[y][x] == '>' || g[y][x] == '<' || g[y][x] == 'v' || g[y][x] == '^') {
				carts[y][x] = g[y][x];
				state[y][x] = STATE_L1;
			}
		}
	}
	int f = 0;
	for (int i = 0; ; i++) {
		for (int y = 0; y < h; y++) {
			for (int x = 0; x < w; x++) {
				char t = g[y][x];
				char c = carts[y][x];
				char s = state[y][x];
				int x1 = x, y1 = y;

				if (t == ' ') {
					//printf(" ");
					continue;
				}

				if (c == 0 || done[y][x]) {
					//printf("%c", t);
					continue;
				}

				//printf("%c", c);
				if (t == '+') {
					if (s == STATE_L1) {
						if (c == '<') c = 'v';
						else if (c == 'v') c = '>';
						else if (c == '>') c = '^';
						else if (c == '^') c = '<';
						s = STATE_S2;
					} else if (s == STATE_S2) {
						s = STATE_R3;
					} else if (s == STATE_R3) {
						if (c == '<') c = '^';
						else if (c == 'v') c = '<';
						else if (c == '>') c = 'v';
						else if (c == '^') c = '>';
						s = STATE_L1;
					}
				}
				if (t == '\\') {
					if (c == '^') c = '<';
					else if (c == '>') c = 'v';
					else if (c == 'v') c = '>';
					else if (c == '<') c = '^';
				}

				if (t == '/') {
					if (c == '^') c = '>';
					else if (c == '<') c = 'v';
					else if (c == '>') c = '^';
					else if (c == 'v') c = '<';
				}

				if (c == '^') y1--;
				if (c == '<') x1--;
				if (c == 'v') y1++;
				if (c == '>') x1++;
				if (carts[y1][x1] && !f) {
					printf("%d,%d\n", x1, y1);
					f = 1;
				}
				if (carts[y1][x1]) {
					carts[y1][x1] = 0;
					state[y1][x1] = 0;
				} else {
					carts[y1][x1] = c;
					state[y1][x1] = s;
				}
				carts[y][x] = 0;
				state[y][x] = 0;
				done[y1][x1] = 1;
			}
		}
		int n = 0, x1, y1;
		for (int y = 0; y < h; y++)
			for (int x = 0; x < w; x++)
				if (carts[y][x])
					n++, x1 = x, y1 = y;
		if (n == 1) {
			printf("%d,%d\n", x1, y1);
			break;
		}
		for (int y = 0; y < h; y++) {
			for (int x = 0; x < w; x++) {
				done[y][x] = 0;
			}
		}
	}
}
