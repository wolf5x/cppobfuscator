double fast_pow(double x, int n) {
    if (n == 0)
        return 1;
    double y = fast_pow(x, n/2);
    if (n%2 == 0)
        return y*y;
    else 
        return y*y*x;
}

int main() {
    fast_pow(2.5, 50);
    return 0;
}

