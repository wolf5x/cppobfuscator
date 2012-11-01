bool or_or() {
	int a, b, c, d;
	if((a++) || (++b) || (--c && (d*=2)))
		return true;
	return false;
}
