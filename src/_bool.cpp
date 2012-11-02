bool or_or() {
	int a, b, c, d;
	a = b ? c : d ? a : b ? c : d;
	if((a++) || (++b) || (--c && (d*=2)))
		return true;
	else 
		return false;
}
int main() {return 0;}
