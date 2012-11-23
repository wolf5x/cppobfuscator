#ifndef OPTION
	#error "Define OPTION prior to including this file!"
#endif

#include <getopt.h>

//Options
OPTION("directory", required_argument, 0, Directory, "d", 0)
OPTION("scope-mode", required_argument, 0, ScopeMode, 0, 0)
OPTION("scope-include-flag", required_argument, 0, ScopeIncludeFlag, 0, 0)
OPTION("scope-exclude-flag", required_argument, 0, ScopeExcludeFlag, 0, 0)
OPTION("obfs", required_argument, 0, CustomObfsArg, 0, 0)
OPTION("Xclang", required_argument, 0, CustomClangArg, 0, 0)

