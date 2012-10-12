#include <stdio.h>
#include <string>

template<typename T>
void func(T a) {
	T b = a;
	T c(a);
	printf("%x\n", (unsigned int)&a);
}

int main() {
	func(std::string("tmp"));
	func(10.2);
	return 0;
}
