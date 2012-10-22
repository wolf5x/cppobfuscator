CXX = g++
CLANG = clang

ROOT = .
SUBDIR = . res alg alg/FlattenCFG
INCLUDE = $(addprefix -I,$(ROOT)) #$(SUBDIR)

TARGET = obfs
TAROBJ = $(TARGET).o

SRCS = $(filter-out ./$(TARGET).cpp, $(foreach dir,$(SUBDIR),$(wildcard $(dir)/[^@_]*.cpp)))
OBJS = $(patsubst %.cpp,%.o,$(SRCS))

DEBUGFLAG = -Wformat=0 -g -w -fno-rtti
CXXFLAGS = $(DEBUGFLAG) -c $(INCLUDE)

#LLVM_LIBS = core mc
#LLVM_CONFIG_COMMAND = $(LLVM_BIN_PATH)/llvm-config --cxxflags --ldflags \
						--libs $(LLVM_LIBS)

PCHFLAGS = \
	-I/usr/include/c++/4.6 \
	-I/usr/include/c++/4.6/i486-linux-gnu/. \
	-I/usr/include/c++/4.6/backward \
	-I/usr/lib/gcc/i486-linux-gnu/4.6/include \
	-I/usr/local/include \
	-I/usr/lib/gcc/i486-linux-gnu/4.6/include-fixed \
	-I/usr/include \
	$(shell llvm-config --cppflags) \
	-I/usr/local/include/lib/clang/3.1/include 

LLVMFLAGS = $(shell llvm-config --cxxflags)
#	-I/usr/local/include \
#	-D_DEBUG -D__NO_CTYPE_INLINE \
#	-D_GNU_SOURCE \
#	-D__STDC_CONSTANT_MACROS \
#	-D__STDC_FORMAT_MACROS \
#	-D__STDC_LIMIT_MACROS \
#	-O3 \
#	-fomit-frame-pointer \
#	-fvisibility-inlines-hidden \
#	-fno-exceptions -fno-rtti \
#	-Woverloaded-virtual \
#	-Wcast-qual

LLVMLIBS = $(shell llvm-config --ldflags --libs core mc)
#	-L/usr/local/lib \
#	-lLLVMMC -lLLVMObject -lLLVMCore -lLLVMSupport \
#	-lpthread -ldl -lm 

CLANGLIBS = \
	-lclangTooling \
	-lclangFrontendTool -lclangFrontend -lclangDriver -lclangSerialization -lclangCodeGen -lclangParse -lclangSema -lclangStaticAnalyzerFrontend -lclangStaticAnalyzerCheckers -lclangStaticAnalyzerCore -lclangAnalysis -lclangARCMigrate -lclangRewrite -lclangAST -lclangLex -lclangBasic -lclangEdit
	
.PHONY: default all rebuild clean pch

default: all

all: $(TARGET)

rebuild: clean all

pch: $(PCH).h
	$(CLANG) -cc1 -emit-pch -x c++-header ./$(PCH).h $(PCHFLAGS) -o ./$(PCH).h.pch
#	$(CXX) ./$(PCH).h $(PCHFLAGS) -o ./$(PCH).h.pch

$(TARGET): $(OBJS) $(TAROBJ) Makefile
	@echo Link $<
	@$(CXX) $(TAROBJ) $(OBJS) -o $@ $(CLANGLIBS) $(LLVMLIBS) $(LDFLAGS)

$(OBJS): %.o:%.cpp %.h
	@echo Build $<
	@$(CXX) $< $(CXXFLAGS) -o $@ $(LLVMFLAGS)

$(TAROBJ): %.o:%.cpp %.h
	@echo Bulid $<
	@$(CXX) $< $(CXXFLAGS) -o $@ $(LLVMFLAGS)

clean:
	@echo Clean
	@rm -rf $(TARGET) $(TAROBJ) $(OBJS) $(patsubst %.cpp,%.d,$(SRCS))
