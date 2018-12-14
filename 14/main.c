#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
	int input;
	scanf("%d", &input);
	// answer is around ~20 million (hopefully)
	char *a = calloc(30000000, sizeof(int)), s[12];
	sprintf(s, "%d", input);
	long long d = 0, mask = 0, r = 0;
	for (char *p = s; *p; p++) {
		d = (d << 8) | (*p-'0');
		mask = (mask << 8) | 0xff;
	}
	int i1 = 0, i2 = 1, len = 2;
	a[i1] = 3; a[i2] = 7;
	for (;;) {
		int sum = a[i1] + a[i2];
		if (sum >= 10) {
			a[len++] = 1;
			r = ((r << 8) | 1) & mask;
			if (r == d) break;
		}
		a[len++] = sum%10;
		r = ((r << 8) | (sum%10)) & mask;
		if (r == d) break;
		i1 = (i1+a[i1]+1)%len;
		i2 = (i2+a[i2]+1)%len;
	}
	for (int i = input; i < input+10; i++)
		printf("%d", a[i]);
	printf("\n%d\n", len-(int)strlen(s));
}
