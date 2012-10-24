#include <stdio.h>
#include <vector>
#include <string>
using namespace std;

void declref() {
	int a[][3][2] = { {1,2,3}, {}, {{3,},{9,8}} };
	a[0][0][1] = 0;
}

void classref() {
	string b[][2] = { {},{"xx", string("yyy")} };
	vector<string> A;
	A[2][3] = 'x';
}

int main() {
	return 0;
}
