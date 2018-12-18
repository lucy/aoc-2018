#include <stdio.h>

#define d 50

int main(void) {
	char ga[d][d], aa[d][d];
	char (*a)[d] = ga;
	char (*g)[d] = aa;
	int c = 0;
	int i = 0;
	int j = 0;
	while ((c = getc(stdin)) != -1) {
		if (c == '\n') { i++; j = 0; continue; }
		g[i][j++] = c;
	}
#define get(x, y) (((x) >= d || (y) >= d || (x) < 0 || (y) < 0) ? 0 : g[y][x])
	for (int i = 1; i <= 600; i++) {
		char (*tmp)[d] = g;
		g = a; a = tmp;
		for (int y = 0; y < d; y++) {
			for (int x = 0; x < d; x++) {
				int adj[8] = { get(x-1, y-1), get(x+1, y-1), get(x-1, y+1), get(x+1, y+1),
					get(x, y-1), get(x+1, y), get(x-1, y), get(x, y+1), };
				int t = 0, l = 0;
				for (int i = 0; i < 8; i++) { if (adj[i] == '|') t++; if (adj[i] == '#') l++; }
				if (g[y][x] == '.') { if (t >= 3) { a[y][x] = '|'; } else { a[y][x] = '.'; } }
				if (g[y][x] == '|') { if (l >= 3) { a[y][x] = '#'; } else { a[y][x] = '|'; } }
				if (g[y][x] == '#') { if (l >= 1 && t >= 1) { a[y][x] = '#'; } else { a[y][x] = '.'; } }
			}
		}
		int t = 0, l = 0;
		for (int y = 0; y < d; y++) {
			for (int x = 0; x < d; x++) {
				if (g[y][x] == '#') l++;
				if (g[y][x] == '|') t++;
			}
		}
		printf("%d: %d\n", i-2, t*l);
	}
	for (int y = 0; y < d; y++) {
		for (int x = 0; x < d; x++) {
			printf("%c", g[y][x]);
		}
		printf("\n");
	}
	int t = 0, l = 0;
	for (int y = 0; y < d; y++) {
		for (int x = 0; x < d; x++) {
			if (g[y][x] == '#') l++;
			if (g[y][x] == '|') t++;
		}
	}
	printf("%d: %d\n", i, t*l);
	/* part 2 done with calc.exe */
}
