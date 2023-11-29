#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double epsilon = 10e-6;
int count = 0;

struct simplex_t{
    int         m;
    int         n;
    int*        var;
    double**    a;
    double*     b;
    double*     x;
    double*     c;
    double      y;
};

double simplex(int m, int n, double** a, double* b, double* c, double* x, double y);

double xsimplex(int m, int n, double** a, double* b, double* c, double* x, double y, int* var, int h);

void pivot(struct simplex_t* s, int row, int col);

int initial(struct simplex_t* s, int m, int n, double** a, double* b, double* c, double* x, double y, int* var);

int init(struct simplex_t* s, int m, int n, double** a, double* b, double* c, double* x, double y, int* var);

int select_nonbasic(struct simplex_t* s);

int main() {
    int m;
    int n;
    double** a;
    double* b;
    double* c;
    double* x;
    size_t i;
    int glob;

    scanf("%d", &m);
    scanf("%d", &n);

    a = calloc(m, sizeof(double*));
    b = calloc(m, sizeof(double));
    c = calloc(n, sizeof(double));
    x = calloc(n + 1, sizeof(double));

    for (i = 0; i < n; i++) {
        scanf("%lf", &c[i]);
        glob += 1;
    }
    for (i = 0; i < m; i++) {
        a[i] = calloc(n, sizeof(double));
        for (size_t j = 0; j < n; ++j) {
            scanf("%lf",&a[i][j]);
        }
    }
    for (i = 0; i < m; i++) {
        scanf("%lf", &b[i]);
    }

    printf("\nresult: %lf\n", simplex(m, n, a, b, c, x, 0));

    free(b);
    for (i = 0; i < m; i++) {
        free(a[i]);
    }
    free(a);
    free(c);
    free(x);
}

void print(struct simplex_t* s) {
    if (count < 1)
    printf("m = %d, n = %d\n\n", s->m, s->n);
    count++;

    printf("max z = ");

    int i, j;

	for (i = 0; i < s->m; i += 1)	{
		if (i == s->n - 1)	{
			printf("%lf*x_%d", s->c[i], i);
		} else	{
			printf("%lf*x_%d + ", s->c[i], i);
		}
	}
	printf("\n");

	for (i = 0; i < s->m; i += 1)	{
		for (j = 0; j < s->n; j += 1)	{
			printf("%10.3lf*x_%d", s->a[i][j], j);
			if (j != s->n - 1)	{
				printf("\t  +");
			} else	{
				printf("\t\u2264 %10.3lf", s->b[i]);
			}
		}
		printf("\n");
	}
}

double simplex(int m, int n, double** a, double* b, double* c, double* x, double y) {
    return xsimplex(m, n, a, b, c, x, y, NULL, 0);
}

double xsimplex(int m, int n, double** a, double* b, double* c, double* x, double y, int* var, int h) {
    struct simplex_t    s;
    int                 i, row, col;

    if (!initial(&s, m, n, a, b, c, x, y, var)) {
        free(s.var);
        return NAN; //not a number.
    }

    while ((col = select_nonbasic(&s)) >= 0) {
        row = -1;
        for (i = 0; i < m; i++) {
            if (a[i][col] > epsilon &&
            (row < 0 || b[i] / a[i][col] < b[row] / a[row][col])) {
                row = i;
            }
        }

        if (row < 0) {
            free(s.var);
            return INFINITY; // unbounded
        }

        pivot(&s, row, col);
        print(&s);
    }

    if (h == 0) {
        for (i = 0; i < n; i++) {
            if (s.var[i] < n) {
                x[s.var[i]] = 0;
            }
        }
        for (i = 0; i < m; i++) {
            if (s.var[n + i] < n) {
                x[s.var[n + i]] = s.b[i];
            }
        }
        free(s.var);
    } else {
        for (i = 0; i < n; i++) {
            x[i] = 0;
        }
        for (i = n; i < n + m; i++) {
            x[i] = s.b[i - n];
        }
    }

    return s.y;
}

void pivot(struct simplex_t* s, int row, int col) {
    double**        a = s->a;
    double*         b = s->b;
    double*         c = s->c;
    int             m = s->m;
    int             n = s->n;
    int             i, j, t;

    t = s->var[col];
    s->var[col] = s->var[n + row];
    s->var[n + row] = t;
    s->y = s->y + c[col] * b[row] / a[row][col];

    for (i = 0; i < n; ++i) {
        if (i != col) {
            c[i] = c[i] - c[col] * a[row][i] / a[row][col];
        }
    }
    c[col] = -c[col] / a[row][col];

    for (i = 0; i < m; ++i) {
        if (i != row) {
            b[i] = b[i] - a[i][col] * b[row] / a[row][col];
        }
    }

    for (i = 0; i < m; ++i) {
        if (i != row) {
            for (j = 0; j < n; ++j) {
                if (j != col) {
                    a[i][j] = a[i][j] - a[i][col] * a[row][j] / a[row][col];
                }
            }
        }
    }

    for (i = 0; i < m; ++i) {
        if (i != row) {
            a[i][col] = -a[i][col] / a[row][col];
        }
    }

    for (i = 0; i < n; ++i) {
        if (i != col) {
            a[row][i] = a[row][i] / a[row][col];
        }
    }

    b[row] = b[row] / a[row][col];
    a[row][col] = 1 / a[row][col];

    printf("\npivot: row: %d, col: %d\n\n", row, col);
}

int initial(struct simplex_t* s, int m, int n, double** a, double* b, double* c, double* x, double y, int* var) {
    int             i, j, k;
    double          w;

    k = init(s, m, n, a, b, c, x, y, var);

    // assume b[k] >= 0
    return 1; // feasible
}

int init(struct simplex_t* s, int m, int n, double** a, double* b, double* c, double* x, double y, int* var) {
    int i, k;

    s->m = m;
    s->n = n;
    s->a = a;
    s->b = b;
    s->c = c;
    s->x = x;
    s->y = y;
    s->var = var;

    if (s->var == NULL) {
        s->var = (int*)calloc(m + n + 1, sizeof(int));
        for (i = 0; i < m + n; i++) {
            s->var[i] = i;
        }
    }

    for (k = 0, i = 1; i < m; i++) {
        if (b[i] < b[k]) {
            k = i;
        }
    }

    return k;
}

int select_nonbasic(struct simplex_t* s) {
    int i;
    for (i = 0; i < s->n; ++i) {
        if (s->c[i] > epsilon) {
            return i;
        }
    }
    return -1;
}