#ifndef OBFS_RES_OPTIONTABLE_H
#define OBFS_RES_OPTIONTABLE_H

#include <getopt.h>
#include <vector>
#include <string>

using std::vector;
using std::string;

// FIXME: implement class Option

//#define OPTION(NAME, HAS_ARG, FLAG, ID, SHORT, HELPTEXT)

//Option ID list
namespace options {
enum OPTID {
	Invalid = 0,
#define OPTION(NAME, HAS_ARG, FLAG, ID, SHORT, HELPTEXT) \
	OPT_##ID,
#include "res/Options.inc"
	Argument,
	EndOption
#undef OPTION
};

//Option List
static option InfoTable[] = {
#define OPTION(NAME, HAS_ARG, FLAG, ID, SHORT, HELPTEXT) \
	{NAME, HAS_ARG, FLAG, OPT_##ID},
#include "res/Options.inc"
#undef OPTION
	{0, 0, 0, 0}
};

}

// This class parses and stores options passed into main().
class OptionTable {
public:
	typedef vector<string> OptValueListTy;
	typedef vector<OptValueListTy> OptPoolTy; 

protected:
	OptPoolTy impl;

public:
	OptionTable(int argc, char **argv) 
		: impl(OptPoolTy(options::EndOption))
	{
		this->ParseArgs(argc, argv);
	}

	// Parse and store args
	bool ParseArgs(int argc, char **argv);
	
	// Get all values of option ID
	OptValueListTy& getOption(options::OPTID ID);
};

#endif

