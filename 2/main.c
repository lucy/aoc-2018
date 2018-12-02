#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

struct p { unsigned char *s; struct p *p; };

int main(void) {
	struct p *l = 0;
	while (1) {
		unsigned char *s = malloc(27);
		if (scanf("%26s", s) != 1) break;
		struct p *p = l;
		l = malloc(sizeof(*l));
		*l = (struct p) {s, p};
	};

	int twos = 0, threes = 0;
	for (struct p *p = l; p; p = p->p) {
		int two = 0, three = 0;
		int a[UCHAR_MAX] = {0};
		for (unsigned char *s = p->s; *s; s++) a[*s]++;
		for (size_t i = 0; i < sizeof(a)/sizeof(*a); i++) {
			if (a[i] == 2) two = 1;
			if (a[i] == 3) three = 1;
		}
		twos += two;
		threes += three;
	}
	printf("%d\n", twos*threes);

	for (struct p *p = l; p; p = p->p) {
		for (struct p *q = p; q; q = q->p) {
			int diff = 0;
			unsigned char *a, *b;
			for (a = p->s, b = q->s; *a && *b && diff < 2; a++, b++)
				if (*a != *b) diff++;
			if (diff != 1) continue;
			for (a = p->s, b = q->s; *a && *b; a++, b++)
				if (*a == *b)
					printf("%c", *a);
			printf("\n");
			break;
		}
	}
}
