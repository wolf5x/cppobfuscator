#include <stdio.h>
#include <string>
using namespace std;

void init_array() {
	int a[] = {1,2,};
	bool b(2);
	std::string c("y");
	std::string d = "yy";
	string e = c;
	string f;
}

//static
void init_static(int m) {
	int n = 2;
	{
		static int s;
		printf("%d\n", ++s);
	}
}

int main()
{
	struct POINT{
		int x, y, z;
	}pxx;
   	POINT po = pxx;
	printf("%d,%d,%d\n", po.x, po.y, po.z);

	init_static(1);
	init_static(-10);

	
	return 0;
}
