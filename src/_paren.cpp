#include <stdio.h>

void paren_cast() {
	int *p;
	int a = ( (p) && (*p) ) && long((p+1));
}

int main() {
	return 0;
}
