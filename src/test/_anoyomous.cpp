#include <stdio.h>

int main() {
	struct { 
		int a, b;  
	} X, Y=X;
	return 0;
}
