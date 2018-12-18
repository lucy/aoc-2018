#include <stdio.h>
#include <stdlib.h>

int main(void) {
	char *a = calloc(1000000000, sizeof(char)), s[13];
	int input, i1 = 0, i2 = 1, len = 2, m = 0;
	a[i1] = 3; a[i2] = 7;
	scanf("%12s", s);
	sscanf(s, "%d", &input);
	while (s[m]) {
		int sum = a[i1] + a[i2], di = 2, d[2];
		do { d[--di] = sum%10; sum /= 10; } while (sum);
		for (; di < 2 && s[m]; di++) {
			a[len++] = d[di];
			if (s[m] == d[di]+'0') m++; else m = 0;
		}
		i1 = (i1+a[i1]+1)%len;
		i2 = (i2+a[i2]+1)%len;
	}
	for (int i = input; i < input+10; i++)
		printf("%d", a[i]);
	printf("\n%d\n", len-m);
}
