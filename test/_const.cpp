#include <stdio.h>
#include <string>
using namespace std;

//const array
void const_initlist() {
	const char *p[] = { "a", "b" };
	const char *aa = "xx";
	char* const bb = "yy";
	//char *p[] = { const_cast<char*>("a"), const_cast<char*>("b") };
}


//const reference
string gs;
const string &func() {
	return gs;
}

int main()
{
	const_initlist();
	//const ptr using new
	int *cp = (int*)new const int(1024);
	printf("%d ", *cp);
	*cp = 20;
	printf("%d\n", *cp);

	//const returned by STL
	string str;
	char *pstr = (char*)str.c_str();
	const string cstr = "xxxxxx";
	pstr = (char*)cstr.c_str();
	string *qstr = (string*)&func();


	//reference has conversion
	string &f = const_cast<string&>(func());
	string *pf = &(const_cast<string&>(func()));


	char *p = const_cast<char*>(&("haha")[0]);
	const char *q = &("haha")[0];
    // FIXME const char array is not a init list??
	char xx[];// = "hoho";
	char *px = xx;
	px = &xx[0];
	return 0;
}
