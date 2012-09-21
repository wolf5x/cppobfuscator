#include <stdio.h>
#include <string>
#include <assert.h>
using namespace std;

bool BOOL;
int INT = 1, *PINT = &INT;
string STR("xx"), *PSTR = &STR;

void externvar() {
	int INT = 2;
	{
		extern int INT;
		INT = 3;
		printf("ext1 %d\n", INT);
	}
	printf("ext2 %d\n", INT);
}

void constvar() {
 	int x = 1, y = 2;
	x = 2;
	const int *p = &x;
	p = &y;
	int* const q = &x;
	*q = 4;
}

void callfunc(string &ref, string *ptr) {
	int x = (ref.length(), ptr->length());
	int y = (ref.length()+((*ptr).length() * ptr->length())) / 2;
	string rr = "test", ss("ttest");
	string *rf;
	if(ref.c_str() == ptr->c_str() 
			&& (rr=ref).c_str() == (*ptr).c_str()){
		string &f = *ptr;
		rf = &(*ptr);
		static int x, y = 3;
		printf("static %d\n", ++y + x);
	}

	void (*fp)(string &, string *) = callfunc;
	void (*&fpr)(string &, string *) = fp;
	void (**fpp)(string &, string *) = &(fp);
	printf("%u %u %u\n", (unsigned int)fp, (unsigned int)fpr, (unsigned int)(*fpp));
}

int main() {
	string LSTR = STR;
	printf("%s\n", LSTR.c_str());
	externvar();
	callfunc(LSTR, &LSTR);
	int n = 3;
LBL1:
	{
		int ary[] = {n, n*2};
		printf("%d\n", ary[1]);
	}
LBL2:
	{
		int ary[3] = {3, n/2};
		printf("%d\n", ary[1]);
	}
LBL3:
	{
		int ary[][2][3] = { 
			{ {111,112,113}, {121,122,123} },
			{ {211,212}, {221} },
			{}
		};
		printf("%d %d\n", ary[1][1][0], ary[2][1][2]);
	}
	return 0;
}
