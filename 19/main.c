#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char *names[] = {
	"addr", "addi", "mulr", "muli", "banr", "bani", "borr", "bori",
	"setr", "seti", "gtir", "gtri", "gtrr", "eqir", "eqri", "eqrr",
};

static inline int run(int op, int a, int b, int r[6]) {
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
	case 0xa: return a > r[b];
	case 0xb: return r[a] > b;
	case 0xc: return r[a] > r[b];
	case 0xd: return a == r[b];
	case 0xe: return r[a] == b;
	case 0xf: return r[a] == r[b];
	}
	abort();
}

struct ins { int op, a, b, c; };

int main(void) {
	int ipr, op, a, b, c, len = 0;
	struct ins insns[1000];
	char opn[5];
	if (scanf("#ip %d\n", &ipr) != 1) return 1;
	while (scanf("%4s %d %d %d\n", opn, &a, &b, &c) == 4) {
		for (op = 0; op < 0x10 && strcmp(names[op], opn); op++);
		insns[len++] = (struct ins) { .op = op, .a = a, .b = b, .c = c };
	}
	int p2 = 0;
	int r[6] = {p2,0,0,0,0,0};
	while (0 <= r[ipr] && r[ipr] < len) {
		struct ins ins = insns[r[ipr]];
		r[ins.c] = run(ins.op, ins.a, ins.b, r);
		if (p2) printf("%d\n", r[1]);
		r[ipr]++;
	}
	printf("%d\n", r[0]);
	/* part 2: set p2 1, google "factors of a number",
	 * input register 1, add them */
}
