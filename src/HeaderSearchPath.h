#ifndef _HSPATH_H_
#define _HSPATH_H_

#include "stdafx.h"

#define LLVM_PREFIX "/usr/local"
const std::string HS_PATHS[] = {
	LLVM_PREFIX "/lib/clang/" CLANG_VERSION_STRING "/include",
	"/usr/local/include",
	"/usr/include",
	"/usr/include/linux",
	"/usr/include/c++/4.6",
	"/usr/include/c++/4.6/i486-linux-gnu",
	"/usr/include/c++/4.6/backward",
};
#endif
