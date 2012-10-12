#include <stdio.h>

int main() {
	int n = 12;
	while(int a = n) {
		int b = n--;
		printf("%d %d\n", a, b);
	}

	while(n == 12){
		n++;
	}
	int y = n+2;
	y *= 2;
	return 0;
}

