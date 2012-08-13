CXX = g++
CLANG = clang
BIN = obfs
OBJS = obfs.o
PCH = stdafx

#LLVM_LIBS = core mc
#LLVM_CONFIG_COMMAND = $(LLVM_BIN_PATH)/llvm-config --cxxflags --ldflags \
						--libs $(LLVM_LIBS)

PCHFLAGS = \
	-I/usr/include/c++/4.6 \
	-I/usr/include/c++/4.6/i686-linux-gnu/. \
	-I/usr/include/c++/4.6/backward \
	-I/usr/lib/gcc/i686-linux-gnu/4.6.1/include \
	-I/usr/local/include \
	-I/usr/lib/gcc/i686-linux-gnu/4.6.1/include-fixed \
	-I/usr/include/i386-linux-gnu \
	-I/usr/include \
	-D_DEBUG -D__NO_CTYPE_INLINE \
	-D_GNU_SOURCE \
	-D__STDC_CONSTANT_MACROS \
	-D__STDC_FORMAT_MACROS \
	-D__STDC_LIMIT_MACROS

LLVMFLAGS = \
	-I/usr/local/include \
	-D_DEBUG -D__NO_CTYPE_INLINE \
	-D_GNU_SOURCE \
	-D__STDC_CONSTANT_MACROS \
	-D__STDC_FORMAT_MACROS \
	-D__STDC_LIMIT_MACROS \
	-O3 \
	-fomit-frame-pointer \
	-fvisibility-inlines-hidden \
	-fno-exceptions -fno-rtti \
	-Woverloaded-virtual \
	-Wcast-qual

LLVMLIBS = \
	-L/usr/local/lib \
	-lLLVMMC -lLLVMObject -lLLVMCore -lLLVMSupport \
	-lpthread -ldl -lm 

CLANGLIBS = \
	-lclangTooling \
	-lclangFrontendTool -lclangFrontend -lclangDriver -lclangSerialization -lclangCodeGen -lclangParse -lclangSema -lclangStaticAnalyzerFrontend -lclangStaticAnalyzerCheckers -lclangStaticAnalyzerCore -lclangAnalysis -lclangARCMigrate -lclangRewrite -lclangAST -lclangLex -lclangBasic -lclangEdit
	
.PHONY: default all rebuild clean pch

default: all

all: print_all $(BIN)

rebuild: clean all

pch: $(PCH).h
	$(CLANG) -cc1 -x c++-header ./$(PCH).h $(PCHFLAGS) -o ./$(PCH).h.pch
#	$(CXX) ./$(PCH).h $(PCHFLAGS) -o ./$(PCH).h.pch

$(BIN): $(OBJS)
	$(CXX) $(OBJS) -o $@ $(CLANGLIBS) $(LLVMLIBS)

%.o: %.cpp
	$(CXX) -c $< $(LLVMFLAGS)

print_all:

clean:
	rm -rf *.o $(OBJS) $(BIN)
