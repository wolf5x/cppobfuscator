#include <stdio.h>

void nullbody() {
	while(true){}
	do{}while(true);
	for(;;){}
}

int main() {
LBL_WHILE:
	int y;
	while(y = 0+3) {
		while(y*3 >0) {
			break;
		}
		continue;
	}
LBL_DO:
	do{
		int y = 0;
		if(y) break;
		if(!y) continue;
	} while(false);
LBL_FOR:
	for(int x = 1, y; y = x; x--) {
	   for(int j = 0; j != x; j++) {
			continue;
	   }		
	   break;
	}
	return 0;
}
