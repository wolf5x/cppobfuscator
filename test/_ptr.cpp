#include <stdio.h>
#include <string>
using namespace std;

int main()
{
	string::size_type (string::* pmf)()const = &string::length;
	string tmp("heloo");
	printf("%d", (tmp.*pmf)());
	return 0;
}
