#include <stdio.h> 
#include <stdlib.h> 

double** make_matrix(int m, int n)	{
	double**		a;
	int					i;

	a = calloc(m, sizeof(double));
	for (i = 0; i < m; i += 1)	{
		a[i] = calloc(n, sizeof(double));
	}

	return a;
}

int main()	{
	int					m;
	int					n;

	if (scanf("%d %d", &m, &n) == 2)	{
		printf("m = %d, n = %d\n", m, n);
	} else {
		printf("Invalid input.\n");
	}

	double*			c;
	double*			b;

	c = calloc(n, sizeof(double));
	b = calloc(m, sizeof(double));

	double** matrix = make_matrix(m, n);

	int					i, j;
	for (i = 0; i < m; i += 1)
		scanf("%lf", &c[i]);

	for (i = 0; i < m; i += 1)	{
		for (j = 0; j < n; j += 1)	{
			scanf("%lf", &matrix[i][j]);
		}
	}

	for (i = 0; i < m; i += 1)
		scanf("%lf", &b[i]);

	// Printing of values

	printf("max z = ");

	for (i = 0; i < n; i += 1)	{
		if (i == n - 1)	{
			printf("%lf*x%d", c[i], i);
		} else	{
			printf("%lf*x%d + ", c[i], i);
		}
	}
	printf("\n");

	for (i = 0; i < m; i += 1)	{
		for (j = 0; j < n; j += 1)	{
			printf("%10.3lf*x_%d", matrix[i][j], j);
			if (j != n - 1)	{
				printf("\t  +");
			} else	{
				printf("\t\u2264 %10.3lf", b[i]);
			}
		}
		printf("\n");
	}

	free(c);
	free(b);
	for (i = 0; i < m; i += 1)	{
		free(matrix[i]);
	}
	free(matrix);

	return 0;
}