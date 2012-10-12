template<typename T>
class A{
public:
	void func(T x);
};

template<typename T>
void A<T>::func(T x) {}

class B : public A<B>{
public:

};

int main() {
	B b;
	b.func(b);
	class LOCAL{
		int x;
	};
	class {
		int y;
	} anoy;

	return 0;
}
