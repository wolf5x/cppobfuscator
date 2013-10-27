#include "obfs.h"

#include "stdafx.h"
#include "Obfuscator.h"
#include <iostream>
using std::cout;

int main(int argc, char **argv) {
	//assert(argc == 2 && "No input file is specified.");
	Obfuscator obfs;
	obfs.init();
	cout << "done1" << endl;
	obfs.doit(argc, argv);
	cout << "done2" << endl;
	
	return 0;
}

