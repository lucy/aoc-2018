#define _POSIX_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define id_sleep -1
#define id_wake  -2
#define is_guard(id) (id >= 0)

struct line { int minute, id; };

struct guard { int id, schedule[60]; };

int compare(const void *a, const void *b) {
	return strcmp(*(char *const *)a, *(char *const *)b);
}

ssize_t input(struct line **p) {
	size_t len = 0, cap = 32;
	char **raw = malloc(cap*sizeof(*raw));
	size_t llen = 0;
	char *raw_line = 0;
	ssize_t n;
	while ((n = getline(&raw_line, &llen, stdin)) != -1) {
		if (len >= cap) raw = realloc(raw, (cap*=2)*sizeof(*raw));
		raw[len++] = raw_line;
		raw_line = 0;
		llen = 0;
	}
	qsort(raw, len, sizeof(*raw), compare);
	struct line *ls = malloc(len*sizeof(*ls));
	struct line l;
	for (size_t i = 0; i < len; i++) {
		int n;
		int r = sscanf(raw[i], "[%*d-%*d-%*d %*d:%d] %n", &l.minute, &n);
		if (r != 1) break;
		char *s = raw[i] + n;
		if (strcmp(s, "falls asleep\n") == 0) l.id = id_sleep;
		else if (strcmp(s, "wakes up\n") == 0) l.id = id_wake;
		else sscanf(s, "Guard #%d begins shift\n", &l.id);
		ls[i] = l;
	}
	*p = ls;
	return len;
}

ssize_t guards(struct guard **p, struct line *ls, size_t llen) {
	size_t len = 0, cap = 32;
	struct guard *gs = malloc(cap*sizeof(*gs));
	size_t current_index = 0;
	int sleep_minute = 0;
	for (size_t i = 0; i < llen; i++) {
		struct line l = ls[i];
		if (is_guard(l.id)) {
			for (size_t j = 0; j < len; j++)
				if (gs[j].id == l.id) {
					current_index = j;
					goto L;
				}
			if (len >= cap) gs = realloc(gs, (cap*=2)*sizeof(*gs));
			current_index = len;
			gs[len++] = (struct guard) { l.id, {0} };
		} else if (l.id == id_sleep) {
			sleep_minute = l.minute;
		} else if (l.id == id_wake) {
			for (int j = sleep_minute; j <= l.minute; j++)
				gs[current_index].schedule[j]++;
		}
L:;
	}
	*p = gs;
	return len;
}

int main(void) {
	struct line *ls;
	ssize_t llen = input(&ls);
	struct guard *gs;
	ssize_t glen = guards(&gs, ls, llen);

	int maxs = 0;
	struct guard maxg = {0};

	for (size_t i = 0; i < (size_t)glen; i++) {
		int sleep = 0;
		for (size_t j = 0; j < 60; j++)
			sleep += gs[i].schedule[j];
		if (sleep > maxs) {
			maxs = sleep;
			maxg = gs[i];
		}
	}

	int maxm = 0, m = 0;
	for (size_t j = 0; j < 60; j++)
		if (maxg.schedule[j] > maxm) {
			maxm = maxg.schedule[j];
			m = j;
		}
	printf("%d %d\n", maxg.id, m);
	printf("%d\n", maxg.id * m);

	maxs = 0;
	for (size_t i = 0; i < (size_t)glen; i++) {
		struct guard g = gs[i];
		for (size_t j = 0; j < 60; j++) {
			int sleep = g.schedule[j];
			if (sleep > maxs) { maxs = sleep; m = j; maxg = g; }
		}
	}
	printf("%d\n", maxg.id * m);
}
