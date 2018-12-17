#include <stdio.h>

int main(void) {
	char scan[2000][2000] = {0};
	int miny = 2000, maxy = 0;
	int minx = 2000, maxx = 0;
	for (;;) {
		int y1, y2, x1, x2;
		if (scanf("y=%d, x=%d..%d\n", &y1, &x1, &x2) == 3) y2 = y1; else
		if (scanf("x=%d, y=%d..%d\n", &x1, &y1, &y2) == 3) x2 = x1; else break;
		for (int x = x1; x <= x2; x++)
			for (int y = y1; y <= y2; y++)
				scan[y][x] = '#';
		if (y1 < miny) miny = y1;
		if (y2 > maxy) maxy = y2;
		if (x1 < minx) minx = x1;
		if (x2 > maxx) maxx = x2;
	}
	scan[0][500] = '|';
	/* probably enough */
	int stack[5000], i = 0;
	stack[i++] = 500;
	stack[i++] = 0;
	while (i) {
		int y = stack[--i];
		int x = stack[--i];
		if (scan[y][x] != '|') continue;
		if (scan[y+1][x] == 0) {
			scan[y+1][x] = '|';
			stack[i++] = x;
			stack[i++] = y+1;
		}
		if (scan[y+1][x] != '#' && scan[y+1][x] != '~') continue;
		int x1 = x, x2 = x;
		while (scan[y][x1-1] != '#') {
			scan[y][--x1] = '|';
			if (scan[y+1][x1] != '#' && scan[y+1][x1] != '~') {
				stack[i++] = x1;
				stack[i++] = y;
				break;
			}
		}
		while (scan[y][x2+1] != '#') {
			scan[y][++x2] = '|';
			if (scan[y+1][x2] != '#' && scan[y+1][x2] != '~') {
				stack[i++] = x2;
				stack[i++] = y;
				break;
			}
		}
		if (scan[y][x1-1] != '#' || scan[y][x2+1] != '#') continue;
		for (; x1 <= x2; x1++) {
			scan[y][x1] = '~';
			if (scan[y-1][x1] == '|') {
				stack[i++] = x1;
				stack[i++] = y-1;
			}
		}
	}
	/*
	int c = 1;
	while (c) {
		c = 0;
		for (int y = 0; y <= maxy+10; y++) {
			for (int x = 0; x <= maxx+20; x++) {
				if (scan[y][x] != '|') continue;
				if (scan[y+1][x] == 0) c++, scan[y+1][x] = '|';
				if (scan[y+1][x] != '#' && scan[y+1][x] != '~') continue;
				if (scan[y][x+1] == 0) c++, scan[y][x+1] = '|';
				if (scan[y][x-1] == 0) c++, scan[y][x-1] = '|';
				int x1 = x, x2 = x;
				while (scan[y][x1-1] == '|') x1--;
				while (scan[y][x2+1] == '|') x2++;
				if (scan[y][x1-1] == '#' && scan[y][x2+1] == '#')
					for (; x1 <= x2; x1++) c++, scan[y][x1] = '~';
			}
		}
	}
	*/
	int still = 0, flow = 0;
	for (int y = miny; y <= maxy; y++) {
		for (int x = minx-1; x <= maxx+1; x++) {
			still += scan[y][x] == '~';
			flow  += scan[y][x] == '|';
			//int c = scan[y][x];
			//if (c == 0) printf(".");
			//else printf("%c", c);
		}
		//printf("\n");
	}
	printf("%d\n", still+flow);
	printf("%d\n", still);
	return 0;
	return 0;
}
