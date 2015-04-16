#include "macros.h"
class Y {
	RTTI_CONSTRUCT(Y)
	RTTI_CONSTRUCT_END(Y)
public:
	char y = 'y';
	void helloY() {
		std::cout << "I am Y!" << std::endl;
	}
};

class Z {
	RTTI_CONSTRUCT(Z)
	RTTI_CONSTRUCT_END(Z)
public:
	char z = 'z';
	void helloZ() {
		std::cout << "I am Z!" << std::endl;
	}
};

class A: public Z, public Y {
	RTTI_CONSTRUCT(A)
	DECLARE_PARENT(A, Z);
	DECLARE_PARENT(A, Y);
	RTTI_CONSTRUCT_END(A)
public:
	char a = 'a';
	void helloA() {
		std::cout << "I am A!" << std::endl;
	}
};


class C {
	RTTI_CONSTRUCT(C)
	RTTI_CONSTRUCT_END(C)
public:
	char c = 'c';
	void helloC() {
		std::cout << "I am C! My field c = " << this->c << std::endl;
	}
};

class B : public A, public C {

public:
	int i = 7;
	RTTI_CONSTRUCT(B)
	DECLARE_PARENT(B, A);
	DECLARE_PARENT(B, C);
	RTTI_CONSTRUCT_END(B)
};


int main() {
	A* a;
	B* b;
	C* c;
	a = new B();
	A* a1 = new A();
	//пытаемся связать два базовых класса без объекта-наследника - 
	//работает обычный reinterpret_cast, поле "с" отождествляется с полем "a" 
	std::cout << "base to base without derived object" << std::endl;
	c = DYNAMIC_CAST(C, A, a1);
	c->helloC();
	//пытаемся связать два базовых класса через объект-наследник - поле "a" уходит из области видимости,
	//поле "c" появляется и не отождествляется с полем "a"
	std::cout << "base to base with derived object" << std::endl;
	c = DYNAMIC_CAST(C, A, a);
	c->helloC();

	//кастуем "дедушку мамы к папе" без внука
	std::cout << "base to base of another base without derived object" << std::endl;
	Y* y1 = new Y();
	c = DYNAMIC_CAST(C, Y, y1);
	c->helloC();

	//кастуем "дедушку мамы к папе" через внука
	std::cout << "base to base of another base with derived object" << std::endl;
	Y* y = new B();
	c = DYNAMIC_CAST(C, Y, y);
	c->helloC();
	int n;
	std::cin >> n;
	return 0;
}
