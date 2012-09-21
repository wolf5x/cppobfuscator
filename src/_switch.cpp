#include <stdio.h>

int main() 
{
	int next = 0;
	for(;;){
		switch(next) {
			case 0:
				next = 2;
				break;
			case 1:
				int x = 1;
				next = -1;
				break;
			case 2:
				x = x+1;
				printf("%d ", x);
				next = 4;
				break;
			case 3:
				int y = 2;
				next = 1;
				break;
			case 4:
				y = y - 2;
				next = 3;
				break;
			default:
				goto ____lbl____0;
				break;
		}
	}
____lbl____0:
	return 0;
}
