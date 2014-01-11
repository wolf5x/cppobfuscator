#include <stdio.h>
#include <string>

template<typename T>
class Class {
	T func();
	void func2(T t){ while(true){}; int u; }
};

template<typename T>
T Class<T>::func() {
	for(int u;;) {
		int t = u; 
	}
}
/*
template<typename T>
void funttt(T a) {
	T b = a;
	T c(a);
}
*/

int main() {
	return 0;
}
