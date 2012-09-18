using namespace std;
using namespace clang;
int main(int argc, char **argv) {
    ((argc == 2 && "No input file is specified.") ? static_cast<void>(0) : __assert_fail("argc == 2 && \"No input file is specified.\"", "obfs.cpp", 8, __PRETTY_FUNCTION__));
    Obfuscator obfs;
    obfs.init();
    cout << "done1" << endl;
    obfs.doit(argv[1]);
    cout << "done2" << endl;
    return 0;
}

;
