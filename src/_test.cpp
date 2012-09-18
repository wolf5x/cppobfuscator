#include "@test.h"
#include <iostream>
using namespace std;

int recursive(int x) {
____lbl____2:
	goto ____lbl____2;
	return x > 0 ? recursive(x-1) + x : x;
}

int main() {
	int a = 0;
/*	
	while(a < 3) {
		cout << "a";
		a++;
	}

	do {
		cout << "b" ;
	} while(++a < 6);
*/
	void *cmd[] = { &&____lbl____3 };
____lbl____3:
	cout << "haha";
	for(int i = 0; i < 10; ++i) {
		for(int j = 0; j < 2; j++){
			cout << i << "," << j << "\n";
			goto *cmd[0];
		}
		for(;; i++);
		for( ;i<0;) recursive(i);
		for(int k = 0; k < 3; ++k)
			cout << i << "," << k << "xx\n";
	}
	MyClass<int> c;
	c.out(10);

	return 0;
}

