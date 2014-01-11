#include <stdio.h>
#include <string>
using namespace std;

class S1 : public string {
public:
	void id() {
		puts("1");
	}
};

class S2 : public string {
public:
	int xx[10];
	void id() {
		puts("2");
	}
};


int main() {
	S1 *p = new S1[10];
	
	p[3].assign("yyy");
	printf("&p[2] = %u\n", (unsigned int)(p+2));
	S2 *q = (S2*)(p+2);
	printf("q = %u\n", (unsigned int)(q));
	while(true)
		q = new S2();
	printf("new q = %u\n", (unsigned int)(q));

	printf("sp = %d, sq = %d\n", sizeof(p[2]), sizeof(*q));
	return 0;
}
