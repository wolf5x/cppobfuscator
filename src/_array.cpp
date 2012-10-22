#include <stdio.h>

void declref() {
	int a[][3][2] = { {1,2,3}, {} };
	a[0][0][1] = 0;
}

int main() {
	return 0;
}
