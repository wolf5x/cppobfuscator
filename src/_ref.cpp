#include <stdio.h>
#include <vector>
using namespace std;

void func(int *&x) {
	printf("%d\n", *x);
}

void func(int *x) {
	
}

int main() {
	//array type reference
	
	int *x = 0;
	//func(x);
	
	int a[3][2][1] = { {1}, {2}, {3} };

	int (&ra)[3][2][1] = a;

	int (*pa)[3][2][1] = &a;

	printf("%d %d\n", ra[1][0][0], (*pa)[1][0][0]);


	return 0;
}
