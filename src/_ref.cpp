#include <stdio.h>
#include <vector>
using namespace std;

void func(int *&x) {
	printf("%d\n", *x);

}

void ref_in_expr(int *x) {
	int *&p = x;
	*p = (*x+*(p+1)) - p[-1];

	int **pp = &p;
}

int main() {
	//array type reference
	
	//func(x);

	//false posi
	int a[3][2][1] = { {1}, {2}, {3} };

	int (&ra)[3][2][1] = a;

	int (*pa)[3][2][1] = &a;

	printf("%d %d\n", ra[1][0][0], (*pa)[1][0][0]);



	return 0;
}
