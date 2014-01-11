#include <stdio.h>

int main() {
	int m = 1;
	int n = m*m;
	struct HAHA{
		int x;
	};

	{
		struct HAHA{
			int y;
		} AA;
	}
	return 0;
}
