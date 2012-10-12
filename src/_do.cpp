#include <stdio.h>

int main() {
	int x = 1;
	do {
		int y = 2;
	} while(y); //y not in scope
	return 0;
}
