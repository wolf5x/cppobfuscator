#include "obfs.h"
#include "stdafx.h"
#include "Obfuscator.h"
#include <iostream>
using namespace std;

int main(int argc, char **argv) {
	assert(argc == 2 && "No input file is specified.");
	Obfuscator obfs;
	obfs.init();
	cout << "done1" << endl;
	obfs.doit(argv[1]);
	cout << "done2" << endl;
	
	return 0;
}

