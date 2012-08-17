#ifndef TEST_H
#define TEST_H

#include "stdio.h"

class TestClass {
public:
	TestClass() { puts("constructed."); }
	~TestClass() { puts("destructed."); }

	void touch() { puts("don't touch me!"); }
};

void func() {
	TestClass A;
	A.touch();
}

#endif
