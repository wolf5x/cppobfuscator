#include <stdio.h>

int main() {
	void *addr[] = {&&LBL0, &&LBL1, &&LBL2};
	int a = 0, b = 1, c = 2;
	goto *addr[1];
LBL0:
	goto *addr[1];
LBL1:
	goto *addr[2];
LBL2:
	goto *addr[0];
	return 0;
}
