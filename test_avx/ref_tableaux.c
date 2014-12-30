#include <stdio.h>

#define N 1024

int main(void)
{
	int a[N], b[N], c[N];
	
	for(int i=0; i<N; i++)
		{
			b[i] = c[i] = i;
			a[i] = b[i] + c[i];
			printf("a[%d] = %d\n", i, a[i]);
		}

	return 0;
}
