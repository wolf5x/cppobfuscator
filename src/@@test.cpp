using namespace std;
int recursive(int x) {
    return x > 0 ? recursive(x - 1) + x : x;
}

;
int main() {
    int a = 0;
    void *cmd[1] = { &&____lbl____3 };
  ____lbl____3:
    cout << "haha";
    for (int i = 0; i < 10; ++i) {
      ____lbl____1:
        ;
        for (int j = 0; j < 2; j++) {
          ____lbl____2:
            ;
            cout << i << "," << j << "\n";
            goto *cmd[0];
        }
        for (;; i++) {
          ____lbl____4:
            ;
        }
        for (; i < 0;) {
          ____lbl____5:
            ;
            recursive;
            i;
        }
        for (int k = 0; k < 3; ++k) {
          ____lbl____6:
            ;
            operator<<;
            cout << i << "," << k;
            "xx\n";
        }
    }
    MyClass<int> c;
    c.out(10);
    return 0;
}

;
