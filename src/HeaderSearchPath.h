#ifndef OBFS_HSPATH_H
#define OBFS_HSPATH_H

//#include "stdafx.h"
#include "clang/Basic/Version.h"
#include <string>
using std::string;

#define LLVM_PREFIX "/usr/local"
const string HS_PATHS[] = {
	LLVM_PREFIX "/lib/clang/" CLANG_VERSION_STRING "/include",
	"/usr/local/include",
	"/usr/include",
	"/usr/include/linux",
	"/usr/include/c++/4.6",
	"/usr/include/c++/4.6/i686-linux-gnu",
	"/usr/include/c++/4.6/backward",
	"/usr/include/i386-linux-gnu/",
};
#endif
