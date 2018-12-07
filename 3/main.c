#include <stdio.h>
#include <stdlib.h>

struct claim { int x, y, w, h; };

int main(void) {
	size_t len = 0, cap = 32;
	struct claim c, *l = malloc(cap*sizeof(*l));
	int w = 1000, h = 1000;
	while (scanf("#%*d @ %d,%d: %dx%d\n", &c.x, &c.y, &c.w, &c.h) == 4) {
		if (len >= cap) l = realloc(l, (cap*=2)*sizeof(*l));
		if (c.x+c.w > w || c.y+c.h > h) exit(1);
		if (c.x+c.w >= w || c.y+c.h >= h)
			printf("%d %d\n", c.x+c.w, c.y+c.h);
		l[len++] = c;
	}

	int *fabric = calloc(w*h, sizeof(*fabric));
	for (size_t i = 0; i < len; i++) {
		c = l[i];
		for (int x = c.x; x < c.x+c.w; x++)
			for (int y = c.y; y < c.y+c.h; y++)
				fabric[x+w*y]++;
	}

	int count = 0;
	for (size_t i = 0; i < (size_t)(w*h); i++)
		if (fabric[i] > 1) count++;
	printf("%d\n", count);

	for (size_t i = 0; i < len; i++) {
		c = l[i];
		for (int x = c.x; x < c.x+c.w; x++)
			for (int y = c.y; y < c.y+c.h; y++)
				if (fabric[x+w*y] > 1) goto L;
		printf("%d\n", (int)i+1);
		break;
L: ;
	}
}
