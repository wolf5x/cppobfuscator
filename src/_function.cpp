int pow() {
	int a = 0;
	pow();
	a += pow();
	{
		int b = 1;
		a += b, b += a, b += b;
	}
	return a;
}

int main() {return 0;}
