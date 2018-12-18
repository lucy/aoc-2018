#define _POSIX_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define STATE_L1 0
#define STATE_S2 1
#define STATE_R3 2

struct cart { char c; char xv, yv; char state, done; };

int main(void) {
	size_t buflen = 0;
	int len = 0, cap = 1024, w = 0, h = 0;
	char *buf = 0, *map = malloc(cap*(sizeof(*map)));
	while (1) {
		ssize_t n = getline(&buf, &buflen, stdin);
		if (n == -1) break;
		while (len+n >= cap) map = realloc(map, (cap *= 2) * sizeof(*map));
		memcpy(map+len, buf, n);
		len += n, w = n;
		if (map[len-1] == '\n') map[--len] = 0, w--;
		h++;
	}
	free(buf);

	struct cart *carts = calloc(w*h, sizeof(*carts));
	for (int y = 0; y < h; y++) for (int x = 0; x < w; x++) {
		char c = map[y*w+x];
		struct cart *cart = &carts[y*w+x];
		//if (c == '<') cart->xv = -1;
		//if (c == '>') cart->xv = +1;
		//if (c == '^') cart->yv = -1;
		//if (c == 'v') cart->yv = +1;
		if (c == '<' || c == '>' || c == '^' || c == 'v')
			cart->c = c;
	}

	int f = 0;
	for (int i = 0; ; i++) {
		for (int y = 0; y < h; y++) {
			for (int x = 0; x < w; x++) {
				char t = map[y*w+x];
				struct cart *cart = &carts[y*w+x];
				char c = cart->c;
				char s = cart->state;
				int x1 = x, y1 = y;

				if (t == ' ') {
					//printf(" ");
					continue;
				}

				
				if (c == 0 || cart->done) {
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

				struct cart *cart2 = &carts[y1*w+x1];
				if (cart2->c && !f) {
					printf("%d,%d\n", x1, y1);
					f = 1;
				}
				if (cart2->c) {
					printf("boom %d,%d @ %d\n", x1, y1, i);
					cart2->c = 0;
					cart2->state = 0;
				} else {
					cart2->c = c;
					cart2->state = s;
				}
				cart->c = 0;
				cart->state = 0;
				cart2->done = 1;
			}
		}
		int n = 0, x1, y1;
		for (int y = 0; y < h; y++)
			for (int x = 0; x < w; x++)
				if (carts[y*w+x].c)
					n++, x1 = x, y1 = y;
		if (n == 1) {
			printf("%d,%d\n", x1, y1);
			break;
		}
		for (int y = 0; y < h; y++) {
			for (int x = 0; x < w; x++) {
				carts[y*w+x].done = 0;
			}
		}
	}
}
