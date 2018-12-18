#include <stdio.h>

int main(void) {
	char scan[2000][2000] = {0}, c;
	int ymin = 2000, ymax = 0, xmin = 2000, xmax = 0;
	for (;;) {
		int y1, y2, x1, x2;
		if (scanf("y=%d, x=%d..%d\n", &y1, &x1, &x2) == 3) y2 = y1; else
		if (scanf("x=%d, y=%d..%d\n", &x1, &y1, &y2) == 3) x2 = x1; else break;
		for (int x = x1; x <= x2; x++) for (int y = y1; y <= y2; y++)
			scan[y][x] = '#';
		if (y1 < ymin) ymin = y1;
		if (y2 > ymax) ymax = y2;
		if (x1 < xmin) xmin = x1;
		if (x2 > xmax) xmax = x2;
	}
	int s[5000], i = 0;
	/* push '|' at 0,500 */
	scan[0][500] = '|'; s[i++] = 500, s[i++] = 0;
	while (i) {
		int y = s[--i], x = s[--i], x1 = x, x2 = x;
		/* if down is 0 set it to '|' and push */
		if (scan[y+1][x] == 0) scan[y+1][x] = '|', s[i++] = x, s[i++] = y+1;
		/* if unsupported, skip (doesn't have '#'/'~' below) */
		if (c = scan[y+1][x], c != '#' && c != '~') continue;
		/* fill '|'s left until '#' */
		for (; scan[y][x1-1] != '#'; scan[y][--x1] = '|')
			if (c = scan[y+1][x1], c != '#' && c != '~') {
				/* push and break if unsupported */
				s[i++] = x1, s[i++] = y; break;
			}
		/* fill '|'s right until '#' */
		for (; scan[y][x2+1] != '#'; scan[y][++x2] = '|')
			if (c = scan[y+1][x2], c != '#' && c != '~') {
				/* push and break if unsupported */
				s[i++] = x2, s[i++] = y; break;
			}
		if (scan[y][x1-1] != '#' || scan[y][x2+1] != '#') continue;
		/* if '|' row is touching '#'s on both sides, turn into '~' */
		for (; x1 <= x2; scan[y][x1++] = '~')
			/* push all '|'s above */
			if (scan[y-1][x1] == '|') s[i++] = x1, s[i++] = y-1;
	}
	int still = 0, flow = 0;
	for (int y = ymin; y <= ymax; y++) for (int x = xmin-1; x <= xmax+1; x++)
		c = scan[y][x], still += c == '~', flow  += c == '|';
	printf("%d\n%d\n", still+flow, still);
}
