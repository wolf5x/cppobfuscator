#include <stdio.h>

int main()
{
	int x(2);
	for(x++; x<3; ) {
		printf("Y");
	}
	printf("%d\n", x);
	for(++x; x<4; ) {
		printf("Z");
	}
	printf("%d\n", x);
	for(int i = 0; i < 2; i++){
		int y(x);
		if(y) {
			for(int j = 2; j>0; j--){
				x--;
			} 
		} else{
			for( x = 4; x < 6; ++x);
		}
		y++;
	}
	return 0;
}

