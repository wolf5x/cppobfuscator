#include <iostream>

template <typename T> 
class MyClass {
public:
	void out(T x) {
		int a = 0;
		while(a++ < 3) {
			std::cout << x << a << std::endl;
			for(int i = 0; i < 10 ;i++){
			}
		}
	}
};
