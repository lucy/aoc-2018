#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

int main(void) {
	uint32_t input;
	scanf("%u", &input);
	uint8_t *a = calloc(30000000, sizeof(int)), s[12];
	sprintf((char *)s, "%u", input);
	uint32_t d = 0, mask = 0, r = 0;
	for (uint8_t *p = s; *p; p++) {
		d = (d << 4) | (*p-'0');
		mask = (mask << 4) | 0xf;
	}
	uint32_t i1 = 0, i2 = 1, len = 2;
	a[i1] = 3; a[i2] = 7;
	for (;;) {
		uint32_t sum = a[i1] + a[i2];
		if (sum >= 10) {
			a[len++] = 1;
			r = ((r << 4) | 1) & mask;
			if (r == d) break;
			sum %= 10;
		}
		a[len++] = sum;
		r = ((r << 4) | sum) & mask;
		if (r == d) break;
		i1 = (i1+a[i1]+1)%len;
		i2 = (i2+a[i2]+1)%len;
	}
	for (uint32_t i = input; i < input+10; i++)
		printf("%d", a[i]);
	printf("\n%u\n", len-(uint32_t)strlen((char *)s));
}
