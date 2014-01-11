#include <stdio.h>
#include <vector>
#include <string>
using namespace std;

void declref() {
    int ____localvar____0[3][3][2];
    int ____localvar____1 = 1;
    while (____localvar____1 != 0)
        switch (int ____localvar____1 = 1) {
          case 1:
            {
                ____localvar____0[0][0][0] = 1;
                ____localvar____0[0][0][1] = 2;
                ____localvar____0[0][1][0] = 3;
                ____localvar____0[2][0][0] = 3;
                ____localvar____0[2][1][0] = 9;
                ____localvar____0[2][1][1] = 8;
                ____localvar____0[0][0][1] = 0;
                break;
            }
        }
}


void classref() {
    string ____localvar____2[2][2];
    vector<string> ____localvar____3;
    int ____localvar____4 = 1;
    while (____localvar____4 != 0)
        switch (int ____localvar____4 = 1) {
          case 1:
            {
                ____localvar____2[1][0] = std::basic_string<char>("xx");
                ____localvar____2[1][1] = std::basic_string<char>(string("yyy"));
                ____localvar____3 = std::vector<std::basic_string<char>, std::allocator<std::basic_string<char> > >();
                ____localvar____3[2][3] = 'x';
                break;
            }
        }
}


int main() {
    int ____localvar____5 = 1;
    while (____localvar____5 != 0)
        switch (int ____localvar____5 = 1) {
          case 1:
            {
                return 0;
                break;
            }
        }
}

