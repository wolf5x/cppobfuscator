#include <iostream>

double fast_pow(double _var_0, int _var_1) {
    double _var_2;
    int _var_3 = 5;
    while (_var_3 != 0)
        switch (_var_3) {
          case 1:
            {
                return _var_2 * _var_2 * _var_0;
            }
          case 2:
            {
                return _var_2 * _var_2;
            }
          case 3:
            {
                _var_2 = fast_pow(_var_0, _var_1 / 2);
                if (_var_1 % 2 == 0) {
                    _var_3 = 2;
                } else {
                    _var_3 = 1;
                }
                break;
            }
          case 4:
            {
                return 1;
            }
          case 5:
            {
                if (_var_1 == 0) {
                    _var_3 = 4;
                } else {
                    _var_3 = 3;
                }
                break;
            }
        }
}


int main() {
    int _var_4 = 1;
    while (_var_4 != 0)
        switch (int _var_4 = 1) {
          case 1:
            {
                std::cout << fast_pow(2, 17) << std::endl;
                return 0;
            }
        }
}


