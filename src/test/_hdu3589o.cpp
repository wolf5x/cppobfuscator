int MIN(int ____localvar____0, int ____localvar____1) {
    int ____localvar____2 = 3;
    while (____localvar____2 != 0)
        switch (____localvar____2) {
          case 1:
            {
                return ____localvar____1;
                ____localvar____2 = 0;
                break;
            }
          case 2:
            {
                return ____localvar____0;
                ____localvar____2 = 0;
                break;
            }
          case 3:
            {
                if (____localvar____0 < ____localvar____1) {
                    ____localvar____2 = 2;
                } else {
                    ____localvar____2 = 1;
                }
                break;
            }
        }
}

;
int MAX(int ____localvar____3, int ____localvar____4) {
    int ____localvar____5 = 3;
    while (____localvar____5 != 0)
        switch (____localvar____5) {
          case 1:
            {
                return ____localvar____4;
                ____localvar____5 = 0;
                break;
            }
          case 2:
            {
                return ____localvar____3;
                ____localvar____5 = 0;
                break;
            }
          case 3:
            {
                if (____localvar____3 > ____localvar____4) {
                    ____localvar____5 = 2;
                } else {
                    ____localvar____5 = 1;
                }
                break;
            }
        }
}

;
using namespace std;
typedef long long LL;
const int MAXPRI = 1000000 + 10;
bool isNotPri[1000010];
int priList[1000010];
int priNum;
void CalPri() {
    int ____localvar____6;
    int ____localvar____7;
    int ____localvar____8 = 20;
    while (____localvar____8 != 0)
        switch (____localvar____8) {
          case 1:
            {
              ____label____11:
                ;
                ____localvar____8 = 0;
                break;
            }
          case 2:
            {
              ____label____10:
                ;
                ____localvar____6 += 2;
                ____localvar____8 = 13;
                break;
            }
          case 3:
            {
              ____label____7:
                ;
                ____localvar____8 = 2;
                break;
            }
          case 4:
            {
              ____label____6:
                ;
                ++____localvar____7;
                ____localvar____8 = 8;
                break;
            }
          case 5:
            {
                ____localvar____8 = 3;
                break;
            }
          case 6:
            {
                isNotPri[____localvar____6 * priList[____localvar____7]] = 1;
                if (____localvar____6 % priList[____localvar____7] == 0) {
                    ____localvar____8 = 5;
                } else {
                    ____localvar____8 = 4;
                }
                break;
            }
          case 7:
            {
                if (____localvar____7 < priNum && ____localvar____6 * priList[____localvar____7] < MAXPRI) {
                    ____localvar____8 = 6;
                } else {
                    ____localvar____8 = 3;
                }
                break;
            }
          case 8:
            {
              ____label____5:
                ;
                if (____localvar____7 < priNum) {
                    ____localvar____8 = 9;
                } else {
                    ____localvar____8 = 7;
                }
                break;
            }
          case 9:
            {
                ____localvar____6 * priList[____localvar____7] < MAXPRI;
                ____localvar____8 = 7;
                break;
            }
          case 10:
            {
              ____label____4:
                ;
                ____localvar____7 = 0;
                ____localvar____8 = 8;
                break;
            }
          case 11:
            {
                priList[priNum++] = ____localvar____6;
                ____localvar____8 = 10;
                break;
            }
          case 12:
            {
                if (!isNotPri[____localvar____6]) {
                    ____localvar____8 = 11;
                } else {
                    ____localvar____8 = 10;
                }
                break;
            }
          case 13:
            {
              ____label____9:
                ;
                if (____localvar____6 < MAXPRI) {
                    ____localvar____8 = 12;
                } else {
                    ____localvar____8 = 1;
                }
                break;
            }
          case 14:
            {
              ____label____8:
                ;
                ____localvar____6 = 3;
                ____localvar____8 = 13;
                break;
            }
          case 15:
            {
              ____label____3:
                ;
                priNum = 0;
                ____localvar____8 = 14;
                break;
            }
          case 16:
            {
              ____label____2:
                ;
                ____localvar____6 += 2;
                ____localvar____8 = 18;
                break;
            }
          case 17:
            {
                isNotPri[____localvar____6] = 1;
                ____localvar____8 = 16;
                break;
            }
          case 18:
            {
              ____label____1:
                ;
                if (____localvar____6 < MAXPRI) {
                    ____localvar____8 = 17;
                } else {
                    ____localvar____8 = 15;
                }
                break;
            }
          case 19:
            {
              ____label____0:
                ;
                ____localvar____6 = 4;
                ____localvar____8 = 18;
                break;
            }
          case 20:
            {
                memset(isNotPri, 0, sizeof (isNotPri));
                isNotPri[0] = isNotPri[1] = 1;
                ____localvar____8 = 19;
                break;
            }
        }
}

;
LL MyPow(LL ____localvar____9, LL ____localvar____10, LL ____localvar____11) {
    LL ____localvar____12;
    int ____localvar____13 = 7;
    while (____localvar____13 != 0)
        switch (____localvar____13) {
          case 1:
            {
                return (____localvar____12 * ____localvar____12) % ____localvar____11;
                ____localvar____13 = 0;
                break;
            }
          case 2:
            {
                return ((____localvar____12 * ____localvar____12) % ____localvar____11) * ____localvar____9 % ____localvar____11;
                ____localvar____13 = 0;
                break;
            }
          case 3:
            {
                MyPow(____localvar____9, ____localvar____10 / 2, ____localvar____11);
                ____localvar____12 = MyPow(____localvar____9, ____localvar____10 / 2, ____localvar____11);
                if (____localvar____10 & 1) {
                    ____localvar____13 = 2;
                } else {
                    ____localvar____13 = 1;
                }
                break;
            }
          case 4:
            {
                return ____localvar____9 % ____localvar____11;
                ____localvar____13 = 0;
                break;
            }
          case 5:
            {
                if (____localvar____10 == 1) {
                    ____localvar____13 = 4;
                } else {
                    ____localvar____13 = 3;
                }
                break;
            }
          case 6:
            {
                return 1;
                ____localvar____13 = 0;
                break;
            }
          case 7:
            {
                if (____localvar____10 == 0) {
                    ____localvar____13 = 6;
                } else {
                    ____localvar____13 = 5;
                }
                break;
            }
        }
}

;
int Cal(int ____localvar____14, int ____localvar____15) {
    LL ____localvar____16;
    int ____localvar____17 = 5;
    while (____localvar____17 != 0)
        switch (____localvar____17) {
          case 1:
            {
                return -1;
                ____localvar____17 = 0;
                break;
            }
          case 2:
            {
                return 1;
                ____localvar____17 = 0;
                break;
            }
          case 3:
            {
                MyPow(____localvar____14, (____localvar____15 - 1) / 2, ____localvar____15);
                ____localvar____16 = MyPow(____localvar____14, (____localvar____15 - 1) / 2, ____localvar____15);
                if (____localvar____16 == 1 % ____localvar____15) {
                    ____localvar____17 = 2;
                } else {
                    ____localvar____17 = 1;
                }
                break;
            }
          case 4:
            {
                return 0;
                ____localvar____17 = 0;
                break;
            }
          case 5:
            {
                if (____localvar____14 % ____localvar____15 == 0 % ____localvar____15) {
                    ____localvar____17 = 4;
                } else {
                    ____localvar____17 = 3;
                }
                break;
            }
        }
}

;
int main() {
    int ____localvar____18;
    int ____localvar____19;
    int ____localvar____20;
    int ____localvar____21;
    int ____localvar____22;
    int ____localvar____23;
    int ____localvar____24 = 19;
    while (____localvar____24 != 0)
        switch (____localvar____24) {
          case 1:
            {
              ____label____20:
                ;
                return 0;
                ____localvar____24 = 0;
                break;
            }
          case 2:
            {
              ____label____19:
                ;
                ____localvar____24 = 18;
                break;
            }
          case 3:
            {
                printf("%d\n", ____localvar____21);
                ____localvar____24 = 2;
                break;
            }
          case 4:
            {
                Cal(____localvar____18, ____localvar____19);
                ____localvar____21 = Cal(____localvar____18, ____localvar____19);
                ____localvar____24 = 3;
                break;
            }
          case 5:
            {
              ____label____17:
                ;
                ____localvar____24 = 3;
                break;
            }
          case 6:
            {
              ____label____16:
                ;
                ____localvar____24 = 15;
                break;
            }
          case 7:
            {
                Cal(____localvar____18, priList[____localvar____22 - 1]);
                ____localvar____23 = Cal(____localvar____18, priList[____localvar____22 - 1]);
                pow((double)____localvar____23, ____localvar____20);
                ____localvar____21 *= pow((double)____localvar____23, ____localvar____20);
                ____localvar____24 = 6;
                break;
            }
          case 8:
            {
              ____label____14:
                ;
                ++____localvar____22;
                if (____localvar____20 != 0) {
                    ____localvar____24 = 7;
                } else {
                    ____localvar____24 = 6;
                }
                break;
            }
          case 9:
            {
              ____label____13:
                ;
                ____localvar____24 = 12;
                break;
            }
          case 10:
            {
                ____localvar____19 /= priList[____localvar____22];
                ++____localvar____20;
                ____localvar____24 = 9;
                break;
            }
          case 11:
            {
                if (____localvar____19 % priList[____localvar____22] == 0 && ____localvar____19 != 1) {
                    ____localvar____24 = 10;
                } else {
                    ____localvar____24 = 8;
                }
                break;
            }
          case 12:
            {
              ____label____12:
                ;
                if (____localvar____19 % priList[____localvar____22] == 0) {
                    ____localvar____24 = 13;
                } else {
                    ____localvar____24 = 11;
                }
                break;
            }
          case 13:
            {
                ____localvar____19 != 1;
                ____localvar____24 = 11;
                break;
            }
          case 14:
            {
                ____localvar____20 = 0;
                ____localvar____24 = 12;
                break;
            }
          case 15:
            {
              ____label____15:
                ;
                if (____localvar____19 != 1) {
                    ____localvar____24 = 14;
                } else {
                    ____localvar____24 = 5;
                }
                break;
            }
          case 16:
            {
                ____localvar____22 = 0;
                ____localvar____24 = 15;
                break;
            }
          case 17:
            {
                ____localvar____21 = 1;
                if (isNotPri[____localvar____19]) {
                    ____localvar____24 = 16;
                } else {
                    ____localvar____24 = 4;
                }
                break;
            }
          case 18:
            {
              ____label____18:
                ;
                scanf("%d%d", &____localvar____18, &____localvar____19);
                if (scanf("%d%d", &____localvar____18, &____localvar____19) != (-1)) {
                    ____localvar____24 = 17;
                } else {
                    ____localvar____24 = 1;
                }
                break;
            }
          case 19:
            {
                CalPri();
                ____localvar____24 = 18;
                break;
            }
        }
}

;
