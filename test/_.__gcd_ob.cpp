#include <iostream>

double fast_pow(double ____localvar____5, int ____localvar____6) {
    double ____localvar____7;
    int ____localvar____8;
    int ____localvar____9;
    int ____localvar____10 = 27;
    while (____localvar____10 != 0)
        switch (int ____localvar____10 = 27) {
          case 1:
            {
              ____label____9:
                ;
                break;
            }
          case 2:
            {
              ____label____8:
                ;
                ____localvar____10 = 26;
                break;
            }
          case 3:
            {
              ____label____1:
                ;
                ____localvar____10 = 2;
                break;
            }
          case 4:
            {
                ____localvar____10 = 3;
                break;
            }
          case 5:
            {
                ____localvar____8 = 3;
                ____localvar____10 = 4;
                break;
            }
          case 6:
            {
                ____localvar____8 = 4;
                ____localvar____10 = 4;
                break;
            }
          case 7:
            {
              ____label____2:
                ;
                if (____localvar____6 == 0) {
                    ____localvar____10 = 6;
                } else {
                    ____localvar____10 = 5;
                }
                ____localvar____10 = 6;
                break;
            }
          case 8:
            {
              ____label____3:
                ;
                return 1;
                break;
            }
          case 9:
            {
                ____localvar____10 = 3;
                break;
            }
          case 10:
            {
                ____localvar____8 = 1;
                ____localvar____10 = 9;
                break;
            }
          case 11:
            {
                ____localvar____8 = 2;
                ____localvar____10 = 9;
                break;
            }
          case 12:
            {
              ____label____4:
                ;
                ____localvar____7 = fast_pow(____localvar____5, ____localvar____6 / 2);
                if (____localvar____6 % 2 == 0) {
                    ____localvar____10 = 11;
                } else {
                    ____localvar____10 = 10;
                }
                ____localvar____10 = 11;
                break;
            }
          case 13:
            {
              ____label____5:
                ;
                return ____localvar____7 * ____localvar____7;
                break;
            }
          case 14:
            {
              ____label____6:
                ;
                return ____localvar____7 * ____localvar____7 * ____localvar____5;
                break;
            }
          case 15:
            {
                ____localvar____10 = 3;
                break;
            }
          case 16:
            {
                ____localvar____10 = 14;
                break;
            }
          case 17:
            {
                if (____localvar____9 == 1) {
                    ____localvar____10 = 16;
                } else {
                    ____localvar____10 = 15;
                }
                ____localvar____10 = 16;
                break;
            }
          case 18:
            {
                ____localvar____10 = 13;
                break;
            }
          case 19:
            {
                if (____localvar____9 == 2) {
                    ____localvar____10 = 18;
                } else {
                    ____localvar____10 = 17;
                }
                ____localvar____10 = 18;
                break;
            }
          case 20:
            {
                ____localvar____10 = 12;
                break;
            }
          case 21:
            {
                if (____localvar____9 == 3) {
                    ____localvar____10 = 20;
                } else {
                    ____localvar____10 = 19;
                }
                ____localvar____10 = 20;
                break;
            }
          case 22:
            {
                ____localvar____10 = 8;
                break;
            }
          case 23:
            {
                if (____localvar____9 == 4) {
                    ____localvar____10 = 22;
                } else {
                    ____localvar____10 = 21;
                }
                ____localvar____10 = 22;
                break;
            }
          case 24:
            {
                ____localvar____10 = 7;
                break;
            }
          case 25:
            {
              ____label____0:
                ;
                ____localvar____9 = (____localvar____8);
                if (____localvar____9 == 5) {
                    ____localvar____10 = 24;
                } else {
                    ____localvar____10 = 23;
                }
                ____localvar____10 = 24;
                break;
            }
          case 26:
            {
              ____label____7:
                ;
                if (____localvar____8 != 0) {
                    ____localvar____10 = 25;
                } else {
                    ____localvar____10 = 1;
                }
                ____localvar____10 = 25;
                break;
            }
          case 27:
            {
                ____localvar____8 = 5;
                ____localvar____10 = 26;
                break;
            }
        }
}



int main() {
    int ____localvar____11;
    int ____localvar____12;
    int ____localvar____13 = 9;
    while (____localvar____13 != 0)
        switch (int ____localvar____13 = 9) {
          case 1:
            {
              ____label____15:
                ;
                break;
            }
          case 2:
            {
              ____label____14:
                ;
                ____localvar____13 = 8;
                break;
            }
          case 3:
            {
              ____label____11:
                ;
                ____localvar____13 = 2;
                break;
            }
          case 4:
            {
              ____label____12:
                ;
                std::cout << fast_pow(2, 17) << std::endl;
                return 0;
                break;
            }
          case 5:
            {
                ____localvar____13 = 3;
                break;
            }
          case 6:
            {
                ____localvar____13 = 4;
                break;
            }
          case 7:
            {
              ____label____10:
                ;
                ____localvar____12 = 1;
                if (____localvar____12 == 1) {
                    ____localvar____13 = 6;
                } else {
                    ____localvar____13 = 5;
                }
                ____localvar____13 = 6;
                break;
            }
          case 8:
            {
              ____label____13:
                ;
                if (____localvar____11 != 0) {
                    ____localvar____13 = 7;
                } else {
                    ____localvar____13 = 1;
                }
                ____localvar____13 = 7;
                break;
            }
          case 9:
            {
                ____localvar____11 = 1;
                ____localvar____13 = 8;
                break;
            }
        }
}



