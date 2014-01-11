#include <stdio.h>

void func(int x) {}

int main() {
	void (*fp)(int) = func;
	void (*&fpr)(int) = fp;
	//(*fp)(0);
	//(*fpr)(0);

	int a = 2, b = 3;
	int *p = &a;

	printf("b = %d\n", *(p+2));

	return 0;
}
