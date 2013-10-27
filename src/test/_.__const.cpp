#include <stdio.h>
#include <string>
using namespace std;

//const array
void const_initlist() {
    const char *____localvar____0[2];
    const char *____localvar____1;
    char *____localvar____2;
    int ____localvar____3 = 1;
    while (____localvar____3 != 0)
        switch (int ____localvar____3 = 1) {
          case 1:
            {
                ____localvar____0[0] = "a";
                ____localvar____0[1] = "b";
                ____localvar____1 = "xx";
                ____localvar____2 = "yy";
                break;
            }
        }
}



//const reference
string gs;
const string &func() {
    int ____localvar____4 = 1;
    while (____localvar____4 != 0)
        switch (int ____localvar____4 = 1) {
          case 1:
            {
                return gs;
                break;
            }
        }
}


int main()
{
    int *____localvar____5;
    string ____localvar____6;
    char *____localvar____7;
    std::basic_string<char> ____localvar____8;
    string *____localvar____9;
    string *____localvar____16;
    string *____localvar____11;
    char *____localvar____12;
    const char *____localvar____13;
    char ____localvar____14[];
    char *____localvar____15;
    int ____localvar____17 = 1;
    while (____localvar____17 != 0)
        switch (int ____localvar____17 = 1) {
          case 1:
            {
                const_initlist();
                ____localvar____5 = (int *)new const int(1024);
                printf("%d ", *____localvar____5);
                *____localvar____5 = 20;
                printf("%d\n", *____localvar____5);
                ____localvar____6 = std::basic_string<char>();
                ____localvar____7 = (char *)(____localvar____6).c_str();
                ____localvar____8 = std::basic_string<char>("xxxxxx");
                ____localvar____7 = (char *)(____localvar____8).c_str();
                ____localvar____9 = (string *)&func();
                ____localvar____16 = &(const_cast<string &>(func()));
                ____localvar____11 = &(const_cast<string &>(func()));
                ____localvar____12 = const_cast<char *>(&("haha")[0]);
                ____localvar____13 = &("haha")[0];
                return 0;
                break;
            }
        }
}

