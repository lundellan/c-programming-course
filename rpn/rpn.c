#include <stdio.h>
#include <ctype.h>

#define N		(10)

#define isFull() (current_index == N - 1)
#define isEmpty() (current_index == -1)
#define peek(out) (isEmpty() ? 0 : (*(out) = stack[current_index], 1))
#define pop(out) (isEmpty() ? 0 : (*(out) = stack[current_index], current_index--, 1))
#define push(in) (isFull() ? 0 : (stack[++current_index] = (in), 1))

int stack[N];
int current_index = -1;

int error_found = 0;
int finished = 0;
int current_line = 1;

void error()	{
	error_found = 1;
	current_index = -1;
	current_line++;
}

int main(void)	{
	int current_char = 0, parsed_int = 0, print_line = 1;

	int term_1, term_2;

	while (!finished)	{
		if (error_found)	{
			while (current_char != '\n')	{
				current_char = getchar();
			}
			error_found = 0;
		}

		print_line = 1;

		while (!error_found)	{
			current_char = getchar();
			if (print_line)	{
				if (current_char != EOF)	{
					printf("line %d: ", current_line);
					print_line = 0;
				} else	{
					finished = 1;
					break;
				}
			}

			if (isdigit(current_char))	{
				if (isFull())	{
					printf("error at %c\n", current_char);
					error();
					break;
				}

				parsed_int = current_char - '0';
				current_char = getchar();

				while (isdigit(current_char))	{
					parsed_int = parsed_int * 10 + (current_char - '0');

					current_char = getchar();
				}

				if (!push(parsed_int))	{
					printf("error at %d\n", parsed_int);
					error();
					break;
				}
			}

			if (current_char != ' ')	{
				if (current_char == '\n')	{
					if (!pop(&term_1) || !isEmpty())	{
						printf("error at \\n\n");
						error();
						break;
					} else	{
						printf("%d\n", term_1);
					}

					current_line++;
					break;
				} else if (current_char == EOF)	{
					finished = 1;
					break;
				} else	{
					if (!pop(&term_2) || !pop(&term_1))	{
						printf("error at %c\n", current_char);
						error();
						break;
					}

					switch (current_char)	{
						case '+':
							push(term_1 + term_2);
							break;
						case '-':
							push(term_1 - term_2);
							break;
						case '*':
							push(term_1 * term_2);
							break;
						case '/':
							if(term_2 == 0){
								printf("error at /\n");
								error();
							}else{
								push(term_1 / term_2);
							}
							break;
						default:
							printf("error at %c\n", current_char);
							error();
							break;
					}
				}
			}
		}
	}

	return 0;
}