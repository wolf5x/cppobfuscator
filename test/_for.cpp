#include <stdio.h>

int main(int argc, char **argv)
{
    for(int i = 0; i < 13 && i > -1 || i < 14; i++){
LABEL0:
        {
            --i;
LABEL1:
            {
                ----i;
LABEL2:
                ++++++i;
            }
        }
        if (i > 9) {
            break;
        } else if (i, i+1, i-1, i-3) {
            i += 2;
            continue;
        } else {
            i = i+1-1+1-1;
            i *= 1;
        }
    }
    return 0;
}

