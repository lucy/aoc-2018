#include <stdio.h>

struct ins { int op; int a, b, c; };

int run(int op, int a, int b, int r[4]) {
	switch (op) {
	case 0x0: return r[a] + r[b];
	case 0x1: return r[a] + b;
	case 0x2: return r[a] * r[b];
	case 0x3: return r[a] * b;
	case 0x4: return r[a] & r[b];
	case 0x5: return r[a] & b;
	case 0x6: return r[a] | r[b];
	case 0x7: return r[a] | b;
	case 0x8: return r[a];
	case 0x9: return a;
	case 0xa: return r[a] > r[b];
	case 0xb: return a > r[b];
	case 0xc: return r[a] > b;
	case 0xd: return r[a] == r[b];
	case 0xe: return a == r[b];
	case 0xf: return r[a] == b;
	}
	return -1;
}

#define op_max (0xf + 1)

int main(void) {
	int opset[op_max][op_max] = {0}, p1 = 0;
	for (;;) {
		int op, a, b, c;
		int ri[4], ro[4], p = 0;
		int n = scanf(
			"Before: [%d, %d, %d, %d]\n"
			"%d %d %d %d\n"
			"After: [%d, %d, %d, %d]\n",
			&ri[0], &ri[1], &ri[2], &ri[3],
			&op, &a, &b, &c,
			&ro[0], &ro[1], &ro[2], &ro[3]);
		if (n != 12) break;
		for (int x = 0; x < op_max; x++)
			if (run(x, a, b, ri) == ro[c]) p++;
			else opset[op][x] = 1;
		if (p >= 3) p1++;
	}
	int ops[op_max] = {0};
	for (int n = 0; n < op_max;) {
		for (int x = 0; x < op_max; x++) {
			int g = 0, opx = 0;
			for (int op = 0; op < op_max; op++)
				if (!opset[x][op]) g++, opx = op;
			if (g != 1) continue;
			for (int y = 0; y < op_max; y++)
				opset[y][opx] = 1;
			ops[x] = opx;
			n++;
		}
	}
	int reg[4] = {0};
	for (;;) {
		int op, a, b, c;
		int n = scanf("%d %d %d %d\n", &op, &a, &b, &c);
		if (n != 4) break;
		reg[c] = run(ops[op], a, b, reg);
	}
	printf("%d\n", p1);
	printf("%d\n", reg[0]);
}
