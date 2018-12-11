#include <limits.h>
#include <stdio.h>

#define power(x, y, s) ((x+10)*y+s)*(x+10)/100%10-5;

int max_square(int serial, int size, int max, int *xmax, int *ymax) {
	for (int x1 = 1; x1 <= 300 - size + 1; x1++) {
		for (int y1 = 1; y1 <= 300 - size + 1; y1++) {
			int p = 0;
			for (int x = x1; x < x1 + size; x++)
				for (int y = y1; y < y1 + size; y++)
					p += power(x, y, serial);
			if (p > max) max = p, *xmax = x1, *ymax = y1;
		}
	}
	return max;
}

int main(void) {
	int serial, xmax, ymax, smax;
	scanf("%d", &serial);
	int max = max_square(serial, 3, INT_MIN, &xmax, &ymax);
	printf("%d,%d\n", xmax, ymax);
	for (int s = 1; s <= 300; s++) {
		int p = max_square(serial, s, max, &xmax, &ymax);
		if (p > max) smax = s, max = p;
	}
	printf("%d,%d,%d\n", xmax, ymax, smax);
}
