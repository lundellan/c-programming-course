#include "poly.h"

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "error.h"

#define ADVANCE_CHAR(string_ptr, current_char) \
  do {                                         \
    current_char = *(string_ptr++);            \
  }                                            \
  while (0)

#define SKIP_SPACE()              \
  do {                            \
    if (current_char == ' ') {    \
      current_char = *(string++); \
    }                             \
  }                               \
  while (0)

struct poly_t {
  int terms;
  int* coefficients;
  int* exponents;
};

poly_t* new_poly_from_string(const char* string) {
  poly_t* poly = malloc(sizeof(poly_t));
  int current_char, coefficient, exponent, sign, terms = 1;

  poly->terms = 0;
  poly->coefficients = malloc(sizeof(int));
  poly->exponents = malloc(sizeof(int));

  current_char = *(string++);

  while (1) {
    coefficient = 1;
    exponent = 0;
    sign = 1;

    // Sign check
    if (current_char == '-') {
      sign = -1;
      ADVANCE_CHAR(string, current_char);
      SKIP_SPACE();
    }
    else if (current_char == '+') {
      ADVANCE_CHAR(string, current_char);
      SKIP_SPACE();
    }

    // Coefficient check
    if (isdigit(current_char)) {
      coefficient = (current_char - '0');
      ADVANCE_CHAR(string, current_char);

      while (isdigit(current_char)) {
        coefficient = coefficient * 10 + (current_char - '0');
        ADVANCE_CHAR(string, current_char);
      }

      coefficient *= sign;
    }

    // Exponent check
    if (current_char == 'x') {
      exponent = 1;
      ADVANCE_CHAR(string, current_char);

      if (current_char == '^') {
        ADVANCE_CHAR(string, current_char);
        exponent = (current_char - '0');
        ADVANCE_CHAR(string, current_char);

        while (isdigit(current_char)) {
          exponent = exponent * 10 + (current_char - '0');
          ADVANCE_CHAR(string, current_char);
        }
      }
    }

    poly->coefficients[terms - 1] = coefficient;
    poly->exponents[terms - 1] = exponent;

    if (current_char == '\0') {
      break;
    }

    terms++;
    poly->coefficients = realloc(poly->coefficients, sizeof(int) * terms);
    poly->exponents = realloc(poly->exponents, sizeof(int) * terms);

    current_char = *(string++);
  }

  poly->terms = terms;

  return poly;
}

void free_poly(poly_t* p) {
  free(p->coefficients);
  free(p->exponents);
  free(p);
}

poly_t* mul(poly_t* poly1, poly_t* poly2) {
  int x, y, i = 0;

  int poly1_terms = poly1->terms;
  int* poly1_coefficients = poly1->coefficients;
  int* poly1_exponents = poly1->exponents;
  int poly2_terms = poly2->terms;
  int* poly2_coefficients = poly2->coefficients;
  int* poly2_exponents = poly2->exponents;

  int terms_max = poly1_terms * poly2_terms;

  int* coefficients = (int*)calloc(terms_max, sizeof(int));
  int* exponents = (int*)calloc(terms_max, sizeof(int));

  poly_t* product = malloc(sizeof(poly_t));
  product->terms = 0;
  product->coefficients = (int*)calloc(terms_max, sizeof(int));
  product->exponents = (int*)calloc(terms_max, sizeof(int));

  // Multiplication of terms
  for (x = 0; x < poly1_terms; ++x) {
    int poly1_coefficient = poly1_coefficients[x];
    int poly1_exponent = poly1_exponents[x];

    for (y = 0; y < poly2_terms; ++y) {
      product->coefficients[i] = poly1_coefficient * poly2_coefficients[y];
      product->exponents[i] = poly1_exponent + poly2_exponents[y];
      i++;
    }
  }

  // Sum terms with same exponent
  int coefficient, exponent, terms_after_add = 0;
  for (x = 0; x < terms_max; ++x) {
    coefficient = product->coefficients[x];
    exponent = product->exponents[x];

    for (y = 0; y < terms_after_add + 1; ++y) {
      if (y == terms_after_add) {
        coefficients[y] = coefficient;
        exponents[y] = exponent;
        terms_after_add++;
        break;
      }
      else if (exponents[y] == exponent) {
        coefficients[y] += coefficient;
        break;
      }
    }
  }

  // Sorting of terms
  int largest_exponent, last_exponent = __INT_MAX__;
  int terms_after_sort = 0;
  for (x = 0; x < terms_after_add; ++x) {
    i = 0;
    largest_exponent = 0;

    for (y = 0; y < terms_after_add; ++y) {
      exponent = exponents[y];
      if (exponent >= largest_exponent && exponent < last_exponent) {
        largest_exponent = exponent;
        i = y;
      }
    }

    // Removal of terms with coefficient of zero
    coefficient = coefficients[i];
    if (coefficient != 0) {
      product->coefficients[terms_after_sort] = coefficient;
      product->exponents[terms_after_sort] = largest_exponent;
      terms_after_sort++;
    }
    last_exponent = largest_exponent;
  }

  product->terms = terms_after_sort;
  product->coefficients = realloc(product->coefficients, sizeof(int) * terms_after_sort);
  product->exponents = realloc(product->exponents, sizeof(int) * terms_after_sort);

  free(coefficients);
  free(exponents);
  return product;
}

void print_poly(poly_t* p) {
  int coefficient, exponent, terms = p->terms;

  for (size_t i = 0; i < terms; ++i) {
    coefficient = p->coefficients[i];
    exponent = p->exponents[i];

    if (coefficient < 0) {
      printf("- ");
      coefficient *= -1;
    }
    else if (i != 0) {
      printf("+ ");
    }

    if (coefficient != 1 || exponent == 0) {
      printf("%d", coefficient);
    }

    if (exponent > 0) {
      printf("x");
    }
    if (exponent > 1) {
      printf("^%d", exponent);
    }

    if (i != terms - 1) {
      printf(" ");
    }
    else {
      printf("\n");
    }
  }
}