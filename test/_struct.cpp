struct option {
	char a[32];
	int b;
	unsigned c;
	char d;
};

int main() {
	option haha = {"aa", 1, 2, '3'}; // error
	return 0;
}
