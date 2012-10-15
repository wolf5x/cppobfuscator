#include <stdio.h>

void let() {
	if(true){}
	if(1){}
}

int main() {
	int x = 3;
	if(int a = x){
		int b = 1;
	} else if(true) {
		;
	} else{
		int c = 2;
	}
	if(x+1+2){
		;
	}
	if(true){
		;
	}
	return 0;
}
