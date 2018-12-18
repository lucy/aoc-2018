#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

static uint32_t loop(uint8_t *a, uint32_t d, uint32_t mask) {
	uint32_t i1 = 0, i2 = 1, len = 2, r = 0;
	uint8_t sum = 10;
	for (;;) {
		if (sum >= 10) {
			a[len++] = 1;
			sum %= 10;
			r = ((r << 4) | 1) & mask;
			if (r == d) break;
		}
		a[len++] = sum;
		r = ((r << 4) | sum) & mask;
		if (r == d) break;
		i1 = (i1+a[i1]+1)%len;
		i2 = (i2+a[i2]+1)%len;
		sum = a[i1] + a[i2];
	}
	return len;
}

int main(void) {
	uint32_t input;
	scanf("%u", &input);
	uint8_t *a = malloc(30000000 * sizeof(uint8_t)), s[12];
	memset(a, 1, 30000000);
	sprintf((char *)s, "%u", input);
	uint32_t d = 0, mask = 0;
	for (uint8_t *p = s; *p; p++) {
		d = (d << 4) | (*p-'0');
		mask = (mask << 4) | 0xf;
	}
	a[0] = 3; a[1] = 7;
	uint32_t len = loop(a, d, mask);
	for (uint32_t i = input; i < input+10; i++)
		printf("%d", a[i]);
	printf("\n%u\n", len-(uint32_t)strlen((char *)s));
	printf("%d\n",  22 << 20);
	printf("%d\n",  8 << 20);
}
