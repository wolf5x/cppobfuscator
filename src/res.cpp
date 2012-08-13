#include <stdio.h>
#include <iostream>
using namespace std;

class ThisShit {
public:
	ThisShit(){
		cout << "damn\n";
		//removed do-while begin
		//{ removed do-while body }
//removed do-while end
		;

		//removed do-while begin
		//{ removed do-while body }
//removed do-while end
				;
		cout << "xx " << __FILE__ << "," << __LINE__ << endl;
		while(true);
		
	}

	void nop() {
		cout << "nop\n";
	}
};

void output(const char *x)
{
	puts(x);
}

int main()
{
	printf("hello, world\n");
	output("124");
	{
		ThisShit shit, *ps = &shit;
		//removed do-while begin
		//{ removed do-while body }
//removed do-while end
		;
		ps->nop();
	}
	return 0;
}



