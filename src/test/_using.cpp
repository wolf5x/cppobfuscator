#include <stdio.h>
#include "_using1.h" //ns1::a
#include "_using2.h" //ns2::a

int main() {

	int x = ns1::a, y = ns2::a;
	namespace lns1{
		int a = 1;
	}
	namespace lns2{
		int a = 2;
	}

	return 0;
}
