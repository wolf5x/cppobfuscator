#include <stdio.h>

int main() 
{
	int x = 1, y = x*2;
	int next = 0;
	for(;;){
		switch(int next = x*y) {
			case 0:
				next = 2;
			case  2 ... 3:
				break;
			case 4:
				y = y - 2;
				next = 3;
				break;
			case 5:
				;
			default:
				goto ____lbl____0;
				break;
		}
	}
____lbl____0:
	return 0;
}
