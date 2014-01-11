#include <stdio.h>
#include <stdlib.h>
#include <string>
using namespace std;

class A{
public:
	int _x;
	char *buf;
	A(int x=0):_x(x), buf(new char[10240]){
		buf[0] = 'a'; buf[273] = 'b'; buf[1734] = 'c'; buf[9317] = 'd';
	}
	~A(){ delete[] buf; puts("d");}
	void S(){printf("x%d\n", _x);}
};

class B{
public:
	int _x[10240];
};

int main()
{
	int x;
	string y;
	A z, *pz = &z;
	void *vals[10] = {};
	for(int i = 0; ; i++){
		B* pb = new B;
		
		//new (&y) string("yy");
		//new (&x) int(i);
		//new (pz) A(i);
	}
	return 0;
}
