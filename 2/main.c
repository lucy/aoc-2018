#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

struct t { struct t *n[26]; };

struct t *t_node(void) { return calloc(1, sizeof(struct t)); }

int t_insert(struct t **t, unsigned char *s) {
	if (!*t) *t = t_node();
	for (; *s; s++) {
		unsigned char c = *s-'a';
		if (c >= sizeof((*t)->n)/sizeof(*(*t)->n)) abort();
		t = &(*t)->n[c];
		if (!*t) *t = t_node();
	}
}

int t_search(struct t *t, unsigned char *s) {
	for (; *s; s++) {
		unsigned char c = *s-'a';
		if (!t) return 0;
		t = t->n[c];
	}
	return 1;
}

unsigned char *t_fuzzy1_search(struct t *t, unsigned char *s) {
	unsigned char *fuzz = malloc(strlen(s));
	int errors = 0;
	for (unsigned char *f = fuzz; *s; s++) {
		unsigned char c = *s-'a';
		if (!t) goto end;
		if (t->n[c]) { *f++ = *s; t = t->n[c]; continue; }
		if (errors) { errors = 0; goto end; }
		errors++;
		for (size_t i = 0; i < sizeof(t->n)/sizeof(*t->n); i++) {
			if (t->n[i]) { t = t->n[i]; break; }
		}
	}
end:
	if (errors == 1) return fuzz;
	free(fuzz);
	return 0;
}

int main(void) {
	size_t cap = 32, len = 0;
	unsigned char **l = malloc(cap*sizeof(*l));
	while (1) {
		unsigned char *s = malloc(27);
		if (scanf("%26s", s) != 1) break;
		if (len >= cap) l = realloc(l, (cap*=2)*sizeof(*l));
		l[len++] = s;
	};

	int twos = 0, threes = 0;
	for (size_t i = 0; i < len; i++) {
		int two = 0, three = 0;
		int a[UCHAR_MAX] = {0};
		for (unsigned char *s = l[i]; *s; s++) a[*s]++;
		for (size_t i = 0; i < sizeof(a)/sizeof(*a); i++) {
			if (a[i] == 2) two = 1;
			if (a[i] == 3) three = 1;
		}
		twos += two;
		threes += three;
	}
	printf("%d\n", twos*threes);

	for (size_t i = 0; i < len; i++) {
		for (size_t j = i+1; j < len; j++) {
			int diff = 0;
			unsigned char *a, *b;
			for (a = l[i], b = l[j]; *a && *b && diff < 2; a++, b++)
				if (*a != *b) diff++;
			if (diff != 1) continue;
			for (a = l[i], b = l[j]; *a && *b; a++, b++)
				if (*a == *b) printf("%c", *a);
			printf("\n");
			break;
		}
	}

	/* part 2 with trie */
	struct t *t = 0;
	for (size_t i = 0; i < len; i++) {
		unsigned char *s;
		if ((s = t_fuzzy1_search(t, l[i]))) {
			printf("%s\n", s);
			break;
		}
		t_insert(&t, l[i]);
	}
}
