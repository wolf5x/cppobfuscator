#include <stdio.h>

int main()
{
	for( ;; );
	for(int i = 0, k = 2, x = 3; i < 2; i++){
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

