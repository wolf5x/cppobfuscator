#include <stdio.h>
#include <string>
#include <assert.h>
using namespace std;

class CCC{
public:
	void ehaha() {
		static int fuck = 0;
		{
			int fuck = 1;
		}
		fuck++;
	}
};

bool BOOL;
int INT = 1, *PINT = &INT;
string STR("xx"), *PSTR = &STR;

template<typename T>
void tpl(T aa) {
	T yy = T();
	T zz = T("yy");
	yy = T();
	yy = "yy";
}

void assign() {
	int x;
	x = int(10);

	string uu = "yy";
	string yy = string();
	string zz = string("yy");
	yy = string();
	yy = string("yy");
	yy = "yy";

	uu = "yy", yy = "zz", zz = string();
}


void externvar() {
	tpl<string>("yy");
	int INT = 2;
	{
		extern int INT;
		INT = 3;
	}
	{}
}

void arraytype(){
	int n = 3;
	{
		typedef string stype;
		string str;
		stype ary;
		stype bry = ary;
	}
	{
		typedef int varray[n][n];
		varray ary;
	}
	{
		int ary[] = {n, n*2};
		printf("%d\n", ary[1]);
	}
	{
		int ary[3] = {3, n/2};
		printf("%d\n", ary[1]);
	}
	{
		int ary[][2][3] = { 
			{ {111,112,113}, {121,122,123} },
			{ {211,212}, {221} },
			{}
		};
		printf("%d %d\n", ary[1][1][0], ary[2][1][2]);
	}
	{
		int ary[n][n*2];
	}

}

void constvar() {
 	int x = 1, y = 2;
	x = 2;
	const int *p = &x;
	p = &y;
	int* const q = &x;
	*q = 4;
}

void init() {
	int xx(0), yy = 0, zz[] = {xx, yy};
	({int xx(2); yy = 3;});
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
	return 0;
}
