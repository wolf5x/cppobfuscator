#include <stdio.h>
#include <string>
using namespace std;
void init_nop() {}
/*
void init_class() {
	string s;
	s = string();
	s = string("xx");
	std::string empty = string();
	string x = "xxx";
	std::string y = std::string("yyy");
	y = string();
	y = string("xxx");
	string z = string();
	z = (string)("xxx");
}
*/

void init_array() {
	bool b(2);
	std::string c("y"), c1, c2[] = {}, c3(c);
	int a[] = {1,2,};
	std::string d = "yy";
	string e = c;
	std::string f;
}

/*
//static
void init_static(int m) {
	int n = 2;
	{
		static int s;
		printf("%d\n", ++s);
	}
}
*/

int main()
{
	struct POINT{
		int x, y, z;
	}pxx;
// 	POINT po = pxx;
//	printf("%d,%d,%d\n", po.x, po.y, po.z);

//	init_static(1);
//	init_static(-10);

	
	return 0;
}
