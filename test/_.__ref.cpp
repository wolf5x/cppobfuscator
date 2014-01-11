#include <stdio.h>
#include <vector>
using namespace std;
/*
void func(int *&x) {
	printf("%d\n", *x);

}

*/
void ref_in_expr(int *____localvar____0) {
    int **____localvar____5;
    int **____localvar____2;
    int ____localvar____3;
    class ____localtag____0 {
        void func()         {
            int ____localvar____4;
            ____localvar____4 = 1;
        }


    };
    int ____localvar____6 = 6;
    while (____localvar____6 != 0)
        switch (int ____localvar____6 = 6) {
          case 1:
            {
              ____label____3:
                ;
                break;
            }
          case 2:
            {
              ____label____2:
                ;
                ____localvar____6 = 4;
                break;
            }
          case 3:
            {
                ____localvar____3 = 0;
                ____localvar____6 = 2;
                break;
            }
          case 4:
            {
              ____label____1:
                ;
                if (true) {
                    ____localvar____6 = 3;
                } else {
                    ;
                }
                ____localvar____6 = 3;
                break;
            }
          case 5:
            {
              ____label____0:
                ;
                ____localvar____6 = 4;
                break;
            }
          case 6:
            {
                ____localvar____5 = &(____localvar____0);
                *(*____localvar____5) = (*____localvar____0 + *((*____localvar____5) + 1)) - (*____localvar____5)[-1];
                ____localvar____2 = &(*____localvar____5);
                ____localvar____6 = 5;
                break;
            }
        }
}


int main() {
    int ____localvar____7 = 1;
    while (____localvar____7 != 0)
        switch (int ____localvar____7 = 1) {
          case 1:
            {
                return 0;
                break;
            }
        }
}

