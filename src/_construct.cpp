#include <stdio.h>

class A{
public:
	static int counter;
	A(){
		printf("counter = %d\n", ++counter);
	}

	void C() const{
		int* pc = &counter;
		*pc = 1;
	}
};
int A::counter = 0;

A ga;

int main()
{
	char *p = (char*)(&("haha")[0]);
	double val = 3.14;
	const int &rr = val;
	const int &yy = 2.234;
	int hh;
	int *pref = &(hh);
 	hh = (*pref) = 32 + (*pref);
	typedef int MAT[2][2];
	MAT a, b = a;
	//int *prr = (int*)&val; // ill
	//*(int*)rr = 2;
	printf("%lf %d\n", val, (*pref));
	A la;
	la.C();
	return 0;
}
