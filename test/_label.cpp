#include <stdio.h>

void lbl_body() {
BODY0:
	for(int x = 0;;) {
BODY1:
		if(x == 
                3 && x == 4){
BODY2:
			x = 1;
			x = 2;
BODY3:{}
		}
	}
}

class A{
public:
	int m;
	A(int _m):m(_m){ printf("c%d\n", m); }
};

int main() 
{
//	void *tbl[] = { &&LBL0, &&LBL1, &&LBL2, &&LBL3, &&LBL4, &&LBL5, &&LBL6 };
	/*
LBL0:
	int next = 0;
	if(next == 0) goto *tbl[1];
LBL1:
	int x = 1;
LBL2:
	x = x+1;
	printf("%d ", x);
	goto LBL1;
LBL3:
	int y = 2;
LBL4:
	y = y - 2;
	goto *tbl[2];
LBL5:
	goto *tbl[0];
LBL6:
*/
	return 0;
}
