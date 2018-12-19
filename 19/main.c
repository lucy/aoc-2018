#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

enum op {
	addr, addi, mulr, muli,
	banr, bani, borr, bori,
	setr, seti,
	gtir, gtri, gtrr,
	eqir, eqri, eqrr,
};

char *names[] = {
	"addr", "addi", "mulr", "muli",
	"banr", "bani", "borr", "bori",
	"setr", "seti",
	"gtir", "gtri", "gtrr",
	"eqir", "eqri", "eqrr",
};

static inline int run(int op, int a, int b, int r[6]) {
	switch (op) {
	case addr: return r[a] + r[b];
	case addi: return r[a] + b;
	case mulr: return r[a] * r[b];
	case muli: return r[a] * b;
	case banr: return r[a] & r[b];
	case bani: return r[a] & b;
	case borr: return r[a] | r[b];
	case bori: return r[a] | b;
	case setr: return r[a];
	case seti: return a;
	case gtir: return a > r[b];
	case gtri: return r[a] > b;
	case gtrr: return r[a] > r[b];
	case eqir: return a == r[b];
	case eqri: return r[a] == b;
	case eqrr: return r[a] == r[b];
	}
	abort();
}

struct ins { int op, a, b, c; };

int main(void) {
	int ipr;
	if (scanf("#ip %d\n", &ipr) != 1) return 1;
	struct ins insns[1000];
	int len = 0;
	for (;;) {
		int op, a, b, c;
		char iname[5];
		int n = scanf("%4s %d %d %d\n", iname, &a, &b, &c);
		if (n != 4) break;
		op = -1;
		if (strcmp(iname, "addr") == 0) { op = addr; }
		else if (strcmp(iname, "addi") == 0) { op = addi; }
		else if (strcmp(iname, "mulr") == 0) { op = mulr; }
		else if (strcmp(iname, "muli") == 0) { op = muli; }
		else if (strcmp(iname, "banr") == 0) { op = banr; }
		else if (strcmp(iname, "bani") == 0) { op = bani; }
		else if (strcmp(iname, "borr") == 0) { op = borr; }
		else if (strcmp(iname, "bori") == 0) { op = bori; }
		else if (strcmp(iname, "setr") == 0) { op = setr; }
		else if (strcmp(iname, "seti") == 0) { op = seti; }
		else if (strcmp(iname, "gtir") == 0) { op = gtir; }
		else if (strcmp(iname, "gtri") == 0) { op = gtri; }
		else if (strcmp(iname, "gtrr") == 0) { op = gtrr; }
		else if (strcmp(iname, "eqir") == 0) { op = eqir; }
		else if (strcmp(iname, "eqri") == 0) { op = eqri; }
		else if (strcmp(iname, "eqrr") == 0) { op = eqrr; }
		insns[len++] = (struct ins) { .op = op, .a = a, .b = b, .c = c };
	}
	int r[6] = {0};
	r[0] = 0;
	while (0 <= r[ipr] && r[ipr] < len) {
		struct ins ins = insns[r[ipr]];
		r[ins.c] = run(ins.op, ins.a, ins.b, r);
		r[ipr]++;
	}
	printf("%d\n", r[0]);
	/* part 2: set register 0 to 1 at the start, google "factors of a number",
	 * input register 1, add them */
}
