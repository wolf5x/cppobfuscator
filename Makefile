CC = gcc
CXX = g++

ROOT = src
REL_SUBDIR = . res alg alg/FlattenCFG
SUBDIR = $(foreach dir,$(REL_SUBDIR),$(addprefix $(ROOT)/,$(dir)))
INCLUDE = $(addprefix -I,$(ROOT))

TARGET = $(ROOT)/obfs
SRCS = $(foreach dir,$(SUBDIR),$(wildcard $(dir)/[^@_]*.cpp))
OBJS = $(SRCS:.cpp=.o)

# Rules for create dependencies
.SUFFIXES: .d $(SUFFIXES)
NODEPS = clean
DEPFILES = $(SRCS:.cpp=.d)

# Flags for compiling and linking
DEBUGFLAG = -Wformat=0 -g -w -fno-rtti
CXXFLAGS = $(DEBUGFLAG) -c $(INCLUDE)
LLVMFLAGS = $(shell llvm-config --cxxflags)
LDFLAGS = $(shell llvm-config --ldflags)
LLVMLIBS = $(shell llvm-config --libs core mc mcparser bitreader)

CLANGLIBS = \
	-lclangTooling\
	-lclangFrontendTool\
	-lclangFrontend\
	-lclangDriver\
	-lclangSerialization\
	-lclangCodeGen\
	-lclangParse\
	-lclangSema\
	-lclangStaticAnalyzerFrontend\
	-lclangStaticAnalyzerCheckers\
	-lclangStaticAnalyzerCore\
	-lclangAnalysis\
	-lclangARCMigrate\
	-lclangRewriteFrontend\
	-lclangRewriteCore\
	-lclangEdit\
	-lclangASTMatchers \
	-lclangAST\
	-lclangLex\
	-lclangBasic

.PHONY: default all rebuild clean

default: all

ifeq (0, $(words $(findstring $(MAKECMDGOALS), $(NODEPS))))
-include $(DEPFILES)
endif

all: $(TARGET)

rebuild: clean all

$(TARGET): $(OBJS) Makefile
	@ echo ====== Link $@
	$(CXX) $(OBJS) -o $@ $(CLANGLIBS) $(LLVMLIBS) $(LDFLAGS)

$(OBJS): %.o:%.cpp %.d %.h
	@ echo ====== Build $@
	$(CXX) $< $(CXXFLAGS) $(LLVMFLAGS) -o $@ 

%.d: %.cpp
	@ echo ====== Generate $@
	$(CXX) $< -MM -MT '$(<:.cpp=.o) $@' $(CXXFLAGS) $(LLVMFLAGS) -MF $@

clean:
	@ echo ====== Clean
	rm -rf $(TARGET) $(OBJS) $(DEPFILES)

