#include <stdio.h>

typedef int (*funcPtr_t)(int a, int b);

int sum(int a, int b) { return a+b; }
int prod(int a, int b) { return a*b; }

int main(int argc, char *argv[]) {
	funcPtr_t fp = sum;
	printf("%d\n", fp(2, 3));
	fp = prod;
	printf("%d\n", fp(2, 3));
}
