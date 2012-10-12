#include <stdio.h>

int main() {
	enum BOX{a,b,c};
	enum {p,q} eee = p, fff(q);
	{
		enum CAT{d,a,b,c};
		int x = a+b+d+p;
	}
	BOX z = a;
	BOX *s = &z;
	//eee = p;
	//z = BOX::a;
	//int zz = BOX::a;
	return 0;
};
