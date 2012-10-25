#include <string>
int main(){
	int a = 0;
LBL0:
	switch(a) {
		case 1:
			{
				std::string a = string("b");
			}
			break;
		case 2 ... 3: 
			goto LBL0;
	}
	return 0;
}
