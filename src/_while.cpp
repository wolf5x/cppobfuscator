#include <stdio.h>

int main() {
	int n = 12;
	while(int a = n + 12) {
		/*
		int b = n--;
		while(b < 11) {
			if(++b > 20) continue;
			if(b > 15) break;
		}
		printf("%d %d\n", a, b);
		*/
	}

	while(n == 12){
		n++;
	}
	int y = n+2;
	while(y=n){}
	y *= 2;
	return 0;
}

