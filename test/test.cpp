#include <stdio.h>
#include <iostream>
#include "@test.h"
using namespace std;

class ThisShit {
public:
	ThisShit(){
		cout << "damn\n";
		do {
			nop();
		} while(false);

		do
		{
			nop();nop();
		}

		while(
				true && true
				&& false );
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
		do {
			ps->nop();
		} while(false);
		ps->nop();
	}
	return 0;
}



