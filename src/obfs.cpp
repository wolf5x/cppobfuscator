#include "stdafx.h"
#include "Obfuscator.h"
using namespace std;
using namespace clang;

int main() {
	Obfuscator obfs;
	obfs.initialize();
	cout << "done1" << endl;
	obfs.initParseAST("test.cpp");
	cout << "done2" << endl;
	obfs.rewriteToSourceFile("testo.cpp");
	cout << "done3" << endl;
	return 0;
}

