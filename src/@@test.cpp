using namespace std;
class ThisShit {
public:
    ThisShit()     {
        cout << "damn\n";
        do {
            this->nop();
        } while (false);
        do {
            this->nop();
            this->nop();
        } while (true && true && false);
        cout << "xx " << "@test.cpp" << "," << 22 << endl;
        while (true)
            ;
    }


    void nop()     {
        cout << "nop\n";
    }


};
void output(const char *x) {
    puts(x);
}

;
int main() {
    printf("hello, world\n");
    output("124");
    {
        ThisShit shit, *ps = &shit;
        do {
            ps->nop();
        } while (false);
        ps->nop();
    }
    return 0;
}

;
