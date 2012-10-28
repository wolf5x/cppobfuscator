#include <string>
int multi_exit() {
	int a = 0;
	if(a){
		return 1;
	} else if(a==0) {
		return 2;
	} else {
		return 3;
	}

	return 3;
}
int main(){
	int a = 0;
}

void empty() {}

void swi() {
	int a ;
	switch(a) {
		case 0:
			return;
		case 1:
			break;
		case 2:
			return;
		default:
			break;
	}
	return ;
}
