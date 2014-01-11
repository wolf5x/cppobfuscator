template<typename T>
class A{
public:
	void func(T x);
};

template<typename T>
void A<T>::func(T x) {
	while(true){
		do{}while(0);
	}
}
/*
class B : public A<B>{
public:

};
void gf() {
	while(true)
		for(;;){}
}
	
*/
int main() {
	A<int> a;
	A<bool> b;
	class LOCAL {
		void func() {
			int x = 0;
			for(;;){}
		}
	};
	LOCAL b;
	/*
	B b;
	b.func(b);
	class LOCAL{
		int x;
		void func() {
			switch(int y = x){
				while(true){
					case 3: ;
					for(;;){
						default:
						case 1: 
							;break;
						case 2:
							continue;
							do{
								case 0:
									break;
							}while(y);
						break;
					}
				}
			}
		}
	};
	class {
		int y;
	} anoy;

	*/
	return 0;
}
