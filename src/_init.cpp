#include <stdio.h>

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
	};
   	POINT po{1,2};
	printf("%d,%d,%d\n", po.x, po.y, po.z);

	init_static(1);
	init_static(-10);

	
	return 0;
}
