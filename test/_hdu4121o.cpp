using namespace std;
char map[11][11];
const int dir[4][2] = { 0, 1, 1, 0, -1, 0, 0, -1 };
int note[10][2];
int cnt;
int b_x;
int b_y;
bool law_G(int ____localvar____0, int ____localvar____1) {
    int ____localvar____2 = 6;
    while (____localvar____2 != 0)
        switch (____localvar____2) {
          case 1:
            {
                return ____localvar____0 <= 3 && ____localvar____0 >= 1 && ____localvar____1 <= 6 && ____localvar____1 >= 4;
                ____localvar____2 = 0;
                break;
            }
          case 2:
            {
                if (____localvar____0 <= 3 && ____localvar____0 >= 1 && ____localvar____1 <= 6) {
                    ____localvar____2 = 3;
                } else {
                    ____localvar____2 = 1;
                }
                break;
            }
          case 3:
            {
                ____localvar____1 >= 4;
                ____localvar____2 = 1;
                break;
            }
          case 4:
            {
                if (____localvar____0 <= 3 && ____localvar____0 >= 1) {
                    ____localvar____2 = 5;
                } else {
                    ____localvar____2 = 2;
                }
                break;
            }
          case 5:
            {
                ____localvar____1 <= 6;
                ____localvar____2 = 2;
                break;
            }
          case 6:
            {
                if (____localvar____0 <= 3) {
                    ____localvar____2 = 7;
                } else {
                    ____localvar____2 = 4;
                }
                break;
            }
          case 7:
            {
                ____localvar____0 >= 1;
                ____localvar____2 = 4;
                break;
            }
        }
}

;
bool deal_G(int ____localvar____3, int ____localvar____4, int ____localvar____5, int ____localvar____6) {
    int ____localvar____7 = 10;
    while (____localvar____7 != 0)
        switch (____localvar____7) {
          case 1:
            {
              ____label____2:
                ;
                return false;
                ____localvar____7 = 0;
                break;
            }
          case 2:
            {
              ____label____1:
                ;
                ____localvar____7 = 8;
                break;
            }
          case 3:
            {
                return false;
                ____localvar____7 = 0;
                break;
            }
          case 4:
            {
                ____localvar____7 = 2;
                break;
            }
          case 5:
            {
                if (map[____localvar____3][____localvar____4] == '.') {
                    ____localvar____7 = 4;
                } else {
                    ____localvar____7 = 3;
                }
                break;
            }
          case 6:
            {
                return true;
                ____localvar____7 = 0;
                break;
            }
          case 7:
            {
                if (____localvar____3 == ____localvar____5) {
                    ____localvar____7 = 6;
                } else {
                    ____localvar____7 = 5;
                }
                break;
            }
          case 8:
            {
              ____label____0:
                ;
                if (--____localvar____3 >= 1) {
                    ____localvar____7 = 7;
                } else {
                    ____localvar____7 = 1;
                }
                break;
            }
          case 9:
            {
                return false;
                ____localvar____7 = 0;
                break;
            }
          case 10:
            {
                if (____localvar____6 != ____localvar____4) {
                    ____localvar____7 = 9;
                } else {
                    ____localvar____7 = 8;
                }
                break;
            }
        }
}

;
bool check_right(int ____localvar____8, int ____localvar____9, int ____localvar____10) {
    int ____localvar____11;
    int ____localvar____12 = 10;
    while (____localvar____12 != 0)
        switch (____localvar____12) {
          case 1:
            {
              ____label____6:
                ;
                return false;
                ____localvar____12 = 0;
                break;
            }
          case 2:
            {
              ____label____5:
                ;
                ++____localvar____11;
                ____localvar____12 = 8;
                break;
            }
          case 3:
            {
                return false;
                ____localvar____12 = 0;
                break;
            }
          case 4:
            {
                ____localvar____12 = 2;
                break;
            }
          case 5:
            {
                if (map[____localvar____8][____localvar____11] == '.') {
                    ____localvar____12 = 4;
                } else {
                    ____localvar____12 = 3;
                }
                break;
            }
          case 6:
            {
                return true;
                ____localvar____12 = 0;
                break;
            }
          case 7:
            {
                if (____localvar____11 == ____localvar____10) {
                    ____localvar____12 = 6;
                } else {
                    ____localvar____12 = 5;
                }
                break;
            }
          case 8:
            {
              ____label____4:
                ;
                if (____localvar____11 <= 9) {
                    ____localvar____12 = 7;
                } else {
                    ____localvar____12 = 1;
                }
                break;
            }
          case 9:
            {
              ____label____3:
                ;
                ____localvar____11 = ____localvar____9 + 1;
                ____localvar____12 = 8;
                break;
            }
          case 10:
            {
                ____localvar____12 = 9;
                break;
            }
        }
}

;
bool check_left(int ____localvar____13, int ____localvar____14, int ____localvar____15) {
    int ____localvar____16;
    int ____localvar____17 = 10;
    while (____localvar____17 != 0)
        switch (____localvar____17) {
          case 1:
            {
              ____label____10:
                ;
                return false;
                ____localvar____17 = 0;
                break;
            }
          case 2:
            {
              ____label____9:
                ;
                --____localvar____16;
                ____localvar____17 = 8;
                break;
            }
          case 3:
            {
                return false;
                ____localvar____17 = 0;
                break;
            }
          case 4:
            {
                ____localvar____17 = 2;
                break;
            }
          case 5:
            {
                if (map[____localvar____13][____localvar____16] == '.') {
                    ____localvar____17 = 4;
                } else {
                    ____localvar____17 = 3;
                }
                break;
            }
          case 6:
            {
                return true;
                ____localvar____17 = 0;
                break;
            }
          case 7:
            {
                if (____localvar____16 == ____localvar____15) {
                    ____localvar____17 = 6;
                } else {
                    ____localvar____17 = 5;
                }
                break;
            }
          case 8:
            {
              ____label____8:
                ;
                if (____localvar____16 >= 1) {
                    ____localvar____17 = 7;
                } else {
                    ____localvar____17 = 1;
                }
                break;
            }
          case 9:
            {
              ____label____7:
                ;
                ____localvar____16 = ____localvar____14 - 1;
                ____localvar____17 = 8;
                break;
            }
          case 10:
            {
                ____localvar____17 = 9;
                break;
            }
        }
}

;
bool check_up(int ____localvar____18, int ____localvar____19, int ____localvar____20) {
    int ____localvar____21;
    int ____localvar____22 = 10;
    while (____localvar____22 != 0)
        switch (____localvar____22) {
          case 1:
            {
              ____label____14:
                ;
                return false;
                ____localvar____22 = 0;
                break;
            }
          case 2:
            {
              ____label____13:
                ;
                --____localvar____21;
                ____localvar____22 = 8;
                break;
            }
          case 3:
            {
                return false;
                ____localvar____22 = 0;
                break;
            }
          case 4:
            {
                ____localvar____22 = 2;
                break;
            }
          case 5:
            {
                if (map[____localvar____21][____localvar____19] == '.') {
                    ____localvar____22 = 4;
                } else {
                    ____localvar____22 = 3;
                }
                break;
            }
          case 6:
            {
                return true;
                ____localvar____22 = 0;
                break;
            }
          case 7:
            {
                if (____localvar____21 == ____localvar____20) {
                    ____localvar____22 = 6;
                } else {
                    ____localvar____22 = 5;
                }
                break;
            }
          case 8:
            {
              ____label____12:
                ;
                if (____localvar____21 >= 1) {
                    ____localvar____22 = 7;
                } else {
                    ____localvar____22 = 1;
                }
                break;
            }
          case 9:
            {
              ____label____11:
                ;
                ____localvar____21 = ____localvar____18 - 1;
                ____localvar____22 = 8;
                break;
            }
          case 10:
            {
                ____localvar____22 = 9;
                break;
            }
        }
}

;
bool check_down(int ____localvar____23, int ____localvar____24, int ____localvar____25) {
    int ____localvar____26;
    int ____localvar____27 = 10;
    while (____localvar____27 != 0)
        switch (____localvar____27) {
          case 1:
            {
              ____label____18:
                ;
                return false;
                ____localvar____27 = 0;
                break;
            }
          case 2:
            {
              ____label____17:
                ;
                ++____localvar____26;
                ____localvar____27 = 8;
                break;
            }
          case 3:
            {
                return false;
                ____localvar____27 = 0;
                break;
            }
          case 4:
            {
                ____localvar____27 = 2;
                break;
            }
          case 5:
            {
                if (map[____localvar____26][____localvar____24] == '.') {
                    ____localvar____27 = 4;
                } else {
                    ____localvar____27 = 3;
                }
                break;
            }
          case 6:
            {
                return true;
                ____localvar____27 = 0;
                break;
            }
          case 7:
            {
                if (____localvar____26 == ____localvar____25) {
                    ____localvar____27 = 6;
                } else {
                    ____localvar____27 = 5;
                }
                break;
            }
          case 8:
            {
              ____label____16:
                ;
                if (____localvar____26 <= 10) {
                    ____localvar____27 = 7;
                } else {
                    ____localvar____27 = 1;
                }
                break;
            }
          case 9:
            {
              ____label____15:
                ;
                ____localvar____26 = ____localvar____23 + 1;
                ____localvar____27 = 8;
                break;
            }
          case 10:
            {
                ____localvar____27 = 9;
                break;
            }
        }
}

;
bool deal_R(int ____localvar____28, int ____localvar____29, int ____localvar____30, int ____localvar____31) {
    int ____localvar____32 = 18;
    while (____localvar____32 != 0)
        switch (____localvar____32) {
          case 1:
            {
                return false;
                ____localvar____32 = 0;
                break;
            }
          case 2:
            {
                return true;
                ____localvar____32 = 0;
                break;
            }
          case 3:
            {
                if (check_up(____localvar____28, ____localvar____29, ____localvar____30)) {
                    ____localvar____32 = 2;
                } else {
                    ____localvar____32 = 1;
                }
                break;
            }
          case 4:
            {
                return false;
                ____localvar____32 = 0;
                break;
            }
          case 5:
            {
                return true;
                ____localvar____32 = 0;
                break;
            }
          case 6:
            {
                if (check_down(____localvar____28, ____localvar____29, ____localvar____30)) {
                    ____localvar____32 = 5;
                } else {
                    ____localvar____32 = 4;
                }
                break;
            }
          case 7:
            {
                if (____localvar____28 < ____localvar____30) {
                    ____localvar____32 = 6;
                } else {
                    ____localvar____32 = 3;
                }
                break;
            }
          case 8:
            {
                return false;
                ____localvar____32 = 0;
                break;
            }
          case 9:
            {
                return true;
                ____localvar____32 = 0;
                break;
            }
          case 10:
            {
                if (check_left(____localvar____28, ____localvar____29, ____localvar____31)) {
                    ____localvar____32 = 9;
                } else {
                    ____localvar____32 = 8;
                }
                break;
            }
          case 11:
            {
                return false;
                ____localvar____32 = 0;
                break;
            }
          case 12:
            {
                return true;
                ____localvar____32 = 0;
                break;
            }
          case 13:
            {
                if (check_right(____localvar____28, ____localvar____29, ____localvar____31)) {
                    ____localvar____32 = 12;
                } else {
                    ____localvar____32 = 11;
                }
                break;
            }
          case 14:
            {
                if (____localvar____29 < ____localvar____31) {
                    ____localvar____32 = 13;
                } else {
                    ____localvar____32 = 10;
                }
                break;
            }
          case 15:
            {
                if (____localvar____28 == ____localvar____30) {
                    ____localvar____32 = 14;
                } else {
                    ____localvar____32 = 7;
                }
                break;
            }
          case 16:
            {
                return false;
                ____localvar____32 = 0;
                break;
            }
          case 17:
            {
                if (____localvar____28 != ____localvar____30 && ____localvar____29 != ____localvar____31) {
                    ____localvar____32 = 16;
                } else {
                    ____localvar____32 = 15;
                }
                break;
            }
          case 18:
            {
                if (____localvar____28 != ____localvar____30) {
                    ____localvar____32 = 19;
                } else {
                    ____localvar____32 = 17;
                }
                break;
            }
          case 19:
            {
                ____localvar____29 != ____localvar____31;
                ____localvar____32 = 17;
                break;
            }
        }
}

;
bool deal_C(int ____localvar____33, int ____localvar____34, int ____localvar____35, int ____localvar____36) {
    bool ____localvar____37;
    int ____localvar____38 = 39;
    while (____localvar____38 != 0)
        switch (____localvar____38) {
          case 1:
            {
                return false;
                ____localvar____38 = 0;
                break;
            }
          case 2:
            {
                return true;
                ____localvar____38 = 0;
                break;
            }
          case 3:
            {
                if (check_up(____localvar____33, ____localvar____34, ____localvar____35)) {
                    ____localvar____38 = 2;
                } else {
                    ____localvar____38 = 1;
                }
                break;
            }
          case 4:
            {
              ____label____30:
                ;
                if (____localvar____37) {
                    ____localvar____38 = 3;
                } else {
                    ____localvar____38 = 1;
                }
                break;
            }
          case 5:
            {
              ____label____29:
                ;
                ____localvar____38 = 9;
                break;
            }
          case 6:
            {
                ____localvar____37 = true;
                ____localvar____38 = 4;
                break;
            }
          case 7:
            {
                ____localvar____38 = 5;
                break;
            }
          case 8:
            {
                if (map[____localvar____33][____localvar____34] == '.') {
                    ____localvar____38 = 7;
                } else {
                    ____localvar____38 = 6;
                }
                break;
            }
          case 9:
            {
              ____label____28:
                ;
                if (--____localvar____33 > ____localvar____35) {
                    ____localvar____38 = 8;
                } else {
                    ____localvar____38 = 4;
                }
                break;
            }
          case 10:
            {
                return true;
                ____localvar____38 = 0;
                break;
            }
          case 11:
            {
                if (check_down(____localvar____33, ____localvar____34, ____localvar____35)) {
                    ____localvar____38 = 10;
                } else {
                    ____localvar____38 = 1;
                }
                break;
            }
          case 12:
            {
              ____label____27:
                ;
                if (____localvar____37) {
                    ____localvar____38 = 11;
                } else {
                    ____localvar____38 = 1;
                }
                break;
            }
          case 13:
            {
              ____label____26:
                ;
                ____localvar____38 = 17;
                break;
            }
          case 14:
            {
                ____localvar____37 = true;
                ____localvar____38 = 12;
                break;
            }
          case 15:
            {
                ____localvar____38 = 13;
                break;
            }
          case 16:
            {
                if (map[____localvar____33][____localvar____34] == '.') {
                    ____localvar____38 = 15;
                } else {
                    ____localvar____38 = 14;
                }
                break;
            }
          case 17:
            {
              ____label____25:
                ;
                if (++____localvar____33 < ____localvar____35) {
                    ____localvar____38 = 16;
                } else {
                    ____localvar____38 = 12;
                }
                break;
            }
          case 18:
            {
                if (____localvar____33 < ____localvar____35) {
                    ____localvar____38 = 17;
                } else {
                    ____localvar____38 = 9;
                }
                break;
            }
          case 19:
            {
                return true;
                ____localvar____38 = 0;
                break;
            }
          case 20:
            {
                if (check_left(____localvar____33, ____localvar____34, ____localvar____36)) {
                    ____localvar____38 = 19;
                } else {
                    ____localvar____38 = 1;
                }
                break;
            }
          case 21:
            {
              ____label____24:
                ;
                if (____localvar____37) {
                    ____localvar____38 = 20;
                } else {
                    ____localvar____38 = 1;
                }
                break;
            }
          case 22:
            {
              ____label____23:
                ;
                ____localvar____38 = 26;
                break;
            }
          case 23:
            {
                ____localvar____37 = true;
                ____localvar____38 = 21;
                break;
            }
          case 24:
            {
                ____localvar____38 = 22;
                break;
            }
          case 25:
            {
                if (map[____localvar____33][____localvar____34] == '.') {
                    ____localvar____38 = 24;
                } else {
                    ____localvar____38 = 23;
                }
                break;
            }
          case 26:
            {
              ____label____22:
                ;
                if (--____localvar____34 > ____localvar____36) {
                    ____localvar____38 = 25;
                } else {
                    ____localvar____38 = 21;
                }
                break;
            }
          case 27:
            {
                return true;
                ____localvar____38 = 0;
                break;
            }
          case 28:
            {
                if (check_right(____localvar____33, ____localvar____34, ____localvar____36)) {
                    ____localvar____38 = 27;
                } else {
                    ____localvar____38 = 1;
                }
                break;
            }
          case 29:
            {
              ____label____21:
                ;
                if (____localvar____37) {
                    ____localvar____38 = 28;
                } else {
                    ____localvar____38 = 1;
                }
                break;
            }
          case 30:
            {
              ____label____20:
                ;
                ____localvar____38 = 34;
                break;
            }
          case 31:
            {
                ____localvar____37 = true;
                ____localvar____38 = 29;
                break;
            }
          case 32:
            {
                ____localvar____38 = 30;
                break;
            }
          case 33:
            {
                if (map[____localvar____33][____localvar____34] == '.') {
                    ____localvar____38 = 32;
                } else {
                    ____localvar____38 = 31;
                }
                break;
            }
          case 34:
            {
              ____label____19:
                ;
                if (++____localvar____34 < ____localvar____36) {
                    ____localvar____38 = 33;
                } else {
                    ____localvar____38 = 29;
                }
                break;
            }
          case 35:
            {
                if (____localvar____34 < ____localvar____36) {
                    ____localvar____38 = 34;
                } else {
                    ____localvar____38 = 26;
                }
                break;
            }
          case 36:
            {
                if (____localvar____33 == ____localvar____35) {
                    ____localvar____38 = 35;
                } else {
                    ____localvar____38 = 18;
                }
                break;
            }
          case 37:
            {
                return false;
                ____localvar____38 = 0;
                break;
            }
          case 38:
            {
                if (____localvar____33 != ____localvar____35 && ____localvar____34 != ____localvar____36) {
                    ____localvar____38 = 37;
                } else {
                    ____localvar____38 = 36;
                }
                break;
            }
          case 39:
            {
                ____localvar____37 = false;
                if (____localvar____33 != ____localvar____35) {
                    ____localvar____38 = 40;
                } else {
                    ____localvar____38 = 38;
                }
                break;
            }
          case 40:
            {
                ____localvar____34 != ____localvar____36;
                ____localvar____38 = 38;
                break;
            }
        }
}

;
bool deal_H(int ____localvar____39, int ____localvar____40, int ____localvar____41, int ____localvar____42) {
    int ____localvar____43 = 30;
    while (____localvar____43 != 0)
        switch (____localvar____43) {
          case 1:
            {
                return false;
                ____localvar____43 = 0;
                break;
            }
          case 2:
            {
                return false;
                ____localvar____43 = 0;
                break;
            }
          case 3:
            {
                return true;
                ____localvar____43 = 0;
                break;
            }
          case 4:
            {
                if (map[____localvar____39][____localvar____40 - 1] == '.') {
                    ____localvar____43 = 3;
                } else {
                    ____localvar____43 = 2;
                }
                break;
            }
          case 5:
            {
                if (____localvar____40 - 2 == ____localvar____42 && (____localvar____39 - 1 == ____localvar____41 || ____localvar____39 + 1 == ____localvar____41)) {
                    ____localvar____43 = 4;
                } else {
                    ____localvar____43 = 1;
                }
                break;
            }
          case 6:
            {
                if (____localvar____40 - 2 == ____localvar____42) {
                    ____localvar____43 = 8;
                } else {
                    ____localvar____43 = 5;
                }
                break;
            }
          case 7:
            {
                ____localvar____39 - 1 == ____localvar____41 || ____localvar____39 + 1 == ____localvar____41;
                ____localvar____43 = 5;
                break;
            }
          case 8:
            {
                if (____localvar____39 - 1 == ____localvar____41) {
                    ____localvar____43 = 7;
                } else {
                    ____localvar____43 = 9;
                }
                break;
            }
          case 9:
            {
                ____localvar____39 + 1 == ____localvar____41;
                ____localvar____43 = 7;
                break;
            }
          case 10:
            {
                return false;
                ____localvar____43 = 0;
                break;
            }
          case 11:
            {
                return true;
                ____localvar____43 = 0;
                break;
            }
          case 12:
            {
                if (map[____localvar____39][____localvar____40 + 1] == '.') {
                    ____localvar____43 = 11;
                } else {
                    ____localvar____43 = 10;
                }
                break;
            }
          case 13:
            {
                if (____localvar____40 + 2 == ____localvar____42 && (____localvar____39 - 1 == ____localvar____41 || ____localvar____39 + 1 == ____localvar____41)) {
                    ____localvar____43 = 12;
                } else {
                    ____localvar____43 = 6;
                }
                break;
            }
          case 14:
            {
                if (____localvar____40 + 2 == ____localvar____42) {
                    ____localvar____43 = 16;
                } else {
                    ____localvar____43 = 13;
                }
                break;
            }
          case 15:
            {
                ____localvar____39 - 1 == ____localvar____41 || ____localvar____39 + 1 == ____localvar____41;
                ____localvar____43 = 13;
                break;
            }
          case 16:
            {
                if (____localvar____39 - 1 == ____localvar____41) {
                    ____localvar____43 = 15;
                } else {
                    ____localvar____43 = 17;
                }
                break;
            }
          case 17:
            {
                ____localvar____39 + 1 == ____localvar____41;
                ____localvar____43 = 15;
                break;
            }
          case 18:
            {
                return false;
                ____localvar____43 = 0;
                break;
            }
          case 19:
            {
                return true;
                ____localvar____43 = 0;
                break;
            }
          case 20:
            {
                if (map[____localvar____39 - 1][____localvar____40] == '.') {
                    ____localvar____43 = 19;
                } else {
                    ____localvar____43 = 18;
                }
                break;
            }
          case 21:
            {
                if (____localvar____39 - 2 == ____localvar____41 && (____localvar____40 - 1 == ____localvar____42 || ____localvar____40 + 1 == ____localvar____42)) {
                    ____localvar____43 = 20;
                } else {
                    ____localvar____43 = 14;
                }
                break;
            }
          case 22:
            {
                if (____localvar____39 - 2 == ____localvar____41) {
                    ____localvar____43 = 24;
                } else {
                    ____localvar____43 = 21;
                }
                break;
            }
          case 23:
            {
                ____localvar____40 - 1 == ____localvar____42 || ____localvar____40 + 1 == ____localvar____42;
                ____localvar____43 = 21;
                break;
            }
          case 24:
            {
                if (____localvar____40 - 1 == ____localvar____42) {
                    ____localvar____43 = 23;
                } else {
                    ____localvar____43 = 25;
                }
                break;
            }
          case 25:
            {
                ____localvar____40 + 1 == ____localvar____42;
                ____localvar____43 = 23;
                break;
            }
          case 26:
            {
                return false;
                ____localvar____43 = 0;
                break;
            }
          case 27:
            {
                return true;
                ____localvar____43 = 0;
                break;
            }
          case 28:
            {
                if (map[____localvar____39 + 1][____localvar____40] == '.') {
                    ____localvar____43 = 27;
                } else {
                    ____localvar____43 = 26;
                }
                break;
            }
          case 29:
            {
                if (____localvar____39 + 2 == ____localvar____41 && (____localvar____40 - 1 == ____localvar____42 || ____localvar____40 + 1 == ____localvar____42)) {
                    ____localvar____43 = 28;
                } else {
                    ____localvar____43 = 22;
                }
                break;
            }
          case 30:
            {
                if (____localvar____39 + 2 == ____localvar____41) {
                    ____localvar____43 = 32;
                } else {
                    ____localvar____43 = 29;
                }
                break;
            }
          case 31:
            {
                ____localvar____40 - 1 == ____localvar____42 || ____localvar____40 + 1 == ____localvar____42;
                ____localvar____43 = 29;
                break;
            }
          case 32:
            {
                if (____localvar____40 - 1 == ____localvar____42) {
                    ____localvar____43 = 31;
                } else {
                    ____localvar____43 = 33;
                }
                break;
            }
          case 33:
            {
                ____localvar____40 + 1 == ____localvar____42;
                ____localvar____43 = 31;
                break;
            }
        }
}

;
bool dfs(int ____localvar____44, int ____localvar____45) {
    int ____localvar____46;
    bool ____localvar____47;
    int ____localvar____48;
    int ____localvar____49;
    int ____localvar____50;
    int ____localvar____52 = 34;
    while (____localvar____52 != 0)
        switch (____localvar____52) {
          case 1:
            {
              ____label____44:
                ;
                return true;
                ____localvar____52 = 0;
                break;
            }
          case 2:
            {
              ____label____43:
                ;
                ++____localvar____46;
                ____localvar____52 = 32;
                break;
            }
          case 3:
            {
                return false;
                ____localvar____52 = 0;
                break;
            }
          case 4:
            {
              ____label____40:
                ;
                if (!____localvar____47) {
                    ____localvar____52 = 3;
                } else {
                    ____localvar____52 = 2;
                }
                break;
            }
          case 5:
            {
              ____label____39:
                ;
                ++____localvar____50;
                ____localvar____52 = 29;
                break;
            }
          case 6:
            {
                ____localvar____52 = 4;
                break;
            }
          case 7:
            {
              ____label____32:
                ;
                if (____localvar____47) {
                    ____localvar____52 = 6;
                } else {
                    ____localvar____52 = 5;
                }
                break;
            }
          case 8:
            {
                ____localvar____52 = 7;
                break;
            }
          case 9:
            {
                ____localvar____47 = true;
                ____localvar____52 = 8;
                break;
            }
          case 10:
            {
              ____label____33:
                ;
                if (deal_H(note[____localvar____50][0], note[____localvar____50][1], ____localvar____48, ____localvar____49)) {
                    ____localvar____52 = 9;
                } else {
                    ____localvar____52 = 8;
                }
                break;
            }
          case 11:
            {
                ____localvar____52 = 7;
                break;
            }
          case 12:
            {
                ____localvar____47 = true;
                ____localvar____52 = 11;
                break;
            }
          case 13:
            {
              ____label____34:
                ;
                if (deal_C(note[____localvar____50][0], note[____localvar____50][1], ____localvar____48, ____localvar____49)) {
                    ____localvar____52 = 12;
                } else {
                    ____localvar____52 = 11;
                }
                break;
            }
          case 14:
            {
                ____localvar____52 = 7;
                break;
            }
          case 15:
            {
                ____localvar____47 = true;
                ____localvar____52 = 14;
                break;
            }
          case 16:
            {
              ____label____35:
                ;
                if (deal_R(note[____localvar____50][0], note[____localvar____50][1], ____localvar____48, ____localvar____49)) {
                    ____localvar____52 = 15;
                } else {
                    ____localvar____52 = 14;
                }
                break;
            }
          case 17:
            {
                ____localvar____52 = 7;
                break;
            }
          case 18:
            {
                ____localvar____47 = true;
                ____localvar____52 = 17;
                break;
            }
          case 19:
            {
              ____label____36:
                ;
                if (deal_G(note[____localvar____50][0], note[____localvar____50][1], ____localvar____48, ____localvar____49)) {
                    ____localvar____52 = 18;
                } else {
                    ____localvar____52 = 17;
                }
                break;
            }
          case 20:
            {
                ____localvar____52 = 7;
                break;
            }
          case 21:
            {
                ____localvar____52 = 19;
                break;
            }
          case 22:
            {
                if (____localvar____51 == 'G') {
                    ____localvar____52 = 21;
                } else {
                    ____localvar____52 = 20;
                }
                break;
            }
          case 23:
            {
                ____localvar____52 = 16;
                break;
            }
          case 24:
            {
                if (____localvar____51 == 'R') {
                    ____localvar____52 = 23;
                } else {
                    ____localvar____52 = 22;
                }
                break;
            }
          case 25:
            {
                ____localvar____52 = 13;
                break;
            }
          case 26:
            {
                if (____localvar____51 == 'C') {
                    ____localvar____52 = 25;
                } else {
                    ____localvar____52 = 24;
                }
                break;
            }
          case 27:
            {
                ____localvar____52 = 10;
                break;
            }
          case 28:
            {
              ____label____31:
                ;
                if (____localvar____51 == 'H') {
                    ____localvar____52 = 27;
                } else {
                    ____localvar____52 = 26;
                }
                break;
            }
          case 29:
            {
              ____label____38:
                ;
                if (____localvar____50 < cnt) {
                    ____localvar____52 = 28;
                } else {
                    ____localvar____52 = 4;
                }
                break;
            }
          case 30:
            {
              ____label____37:
                ;
                ____localvar____50 = 0;
                ____localvar____52 = 29;
                break;
            }
          case 31:
            {
                ____localvar____47 = false;
                ____localvar____48 = ____localvar____44 + dir[____localvar____46][0];
                ____localvar____49 = ____localvar____45 + dir[____localvar____46][1];
                if (law_G(____localvar____48, ____localvar____49)) {
                    ____localvar____52 = 30;
                } else {
                    ____localvar____52 = 2;
                }
                break;
            }
          case 32:
            {
              ____label____42:
                ;
                if (____localvar____46 < 4) {
                    ____localvar____52 = 31;
                } else {
                    ____localvar____52 = 1;
                }
                break;
            }
          case 33:
            {
              ____label____41:
                ;
                ____localvar____46 = 0;
                ____localvar____52 = 32;
                break;
            }
          case 34:
            {
                ____localvar____52 = 33;
                break;
            }
        }
}

;
int main() {
    int ____localvar____53;
    char ____localvar____54;
    int ____localvar____55;
    int ____localvar____56;
    int ____localvar____58 = 31;
    while (____localvar____58 != 0)
        switch (____localvar____58) {
          case 1:
            {
              ____label____56:
                ;
                return 0;
                ____localvar____58 = 0;
                break;
            }
          case 2:
            {
              ____label____55:
                ;
                ____localvar____58 = 30;
                break;
            }
          case 3:
            {
                cout << "NO" << endl;
                ____localvar____58 = 2;
                break;
            }
          case 4:
            {
                cout << "YES" << endl;
                ____localvar____58 = 2;
                break;
            }
          case 5:
            {
              ____label____53:
                ;
                if (dfs(b_x, b_y)) {
                    ____localvar____58 = 4;
                } else {
                    ____localvar____58 = 3;
                }
                break;
            }
          case 6:
            {
              ____label____52:
                ;
                ____localvar____58 = 22;
                break;
            }
          case 7:
            {
              ____label____46:
                ;
                note[cnt][0] = ____localvar____55;
                note[cnt++][1] = ____localvar____56;
                ____localvar____58 = 6;
                break;
            }
          case 8:
            {
              ____label____47:
                ;
                map[____localvar____55][____localvar____56] = 'R';
                ____localvar____58 = 7;
                break;
            }
          case 9:
            {
              ____label____48:
                ;
                map[____localvar____55][____localvar____56] = 'H';
                ____localvar____58 = 7;
                break;
            }
          case 10:
            {
              ____label____49:
                ;
                map[____localvar____55][____localvar____56] = 'C';
                ____localvar____58 = 7;
                break;
            }
          case 11:
            {
              ____label____50:
                ;
                map[____localvar____55][____localvar____56] = 'G';
                ____localvar____58 = 7;
                break;
            }
          case 12:
            {
                ____localvar____58 = 7;
                break;
            }
          case 13:
            {
                ____localvar____58 = 11;
                break;
            }
          case 14:
            {
                if (____localvar____57 == 'G') {
                    ____localvar____58 = 13;
                } else {
                    ____localvar____58 = 12;
                }
                break;
            }
          case 15:
            {
                ____localvar____58 = 10;
                break;
            }
          case 16:
            {
                if (____localvar____57 == 'C') {
                    ____localvar____58 = 15;
                } else {
                    ____localvar____58 = 14;
                }
                break;
            }
          case 17:
            {
                ____localvar____58 = 9;
                break;
            }
          case 18:
            {
                if (____localvar____57 == 'H') {
                    ____localvar____58 = 17;
                } else {
                    ____localvar____58 = 16;
                }
                break;
            }
          case 19:
            {
                ____localvar____58 = 8;
                break;
            }
          case 20:
            {
              ____label____45:
                ;
                if (____localvar____57 == 'R') {
                    ____localvar____58 = 19;
                } else {
                    ____localvar____58 = 18;
                }
                break;
            }
          case 21:
            {
                cin >> ____localvar____54 >> ____localvar____55 >> ____localvar____56;
                ____localvar____58 = 20;
                break;
            }
          case 22:
            {
              ____label____51:
                ;
                if (____localvar____53--) {
                    ____localvar____58 = 21;
                } else {
                    ____localvar____58 = 5;
                }
                break;
            }
          case 23:
            {
                memset(map, '.', sizeof (map));
                cnt = 0;
                ____localvar____58 = 22;
                break;
            }
          case 24:
            {
                ____localvar____58 = 1;
                break;
            }
          case 25:
            {
                if (____localvar____53 == 0 && b_x == 0 && b_y == 0) {
                    ____localvar____58 = 24;
                } else {
                    ____localvar____58 = 23;
                }
                break;
            }
          case 26:
            {
                if (____localvar____53 == 0 && b_x == 0) {
                    ____localvar____58 = 27;
                } else {
                    ____localvar____58 = 25;
                }
                break;
            }
          case 27:
            {
                b_y == 0;
                ____localvar____58 = 25;
                break;
            }
          case 28:
            {
                if (____localvar____53 == 0) {
                    ____localvar____58 = 29;
                } else {
                    ____localvar____58 = 26;
                }
                break;
            }
          case 29:
            {
                b_x == 0;
                ____localvar____58 = 26;
                break;
            }
          case 30:
            {
              ____label____54:
                ;
                if (cin >> ____localvar____53 >> b_x >> b_y.operator void *()) {
                    ____localvar____58 = 28;
                } else {
                    ____localvar____58 = 1;
                }
                break;
            }
          case 31:
            {
                ____localvar____58 = 30;
                break;
            }
        }
}

;
