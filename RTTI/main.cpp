#include <iostream>
#include <vector>
#include <string>

class my_type_info {
public:
	std::string classN;
	std::vector<my_type_info> parent;
	std::vector<size_t> offset;
	std::hash<std::string> classHash;
	size_t getClassHash() {
		return classHash(classN);
	}

	template <typename P, typename C>
	void addParent(){
		std::cout << "addP" << std::endl;
		P p;
		C c = C(0);
		parent.push_back(p.getInfo());
		offset.push_back((size_t)(static_cast<P*>(&c)) - (size_t)(&c));
		std::cout << "addPEnd" << std::endl;
	}
	my_type_info(){ ; }
	my_type_info(std::string name) :classN(name){ ; }
};


#define RTTI_CONSTRUCT(className) public: \
	protected:\
    my_type_info info;\
	public:\
	className(int flag){;}\
	virtual my_type_info getVirtualInfo() {\
		return info;\
	}\
	my_type_info getInfo() {\
		return info;\
	}\
	className(){ \
		std::cout<<#className<<std::endl;\
			info.classN = #className;\

#define RTTI_CONSTRUCT_END(className)}\
	private:

#define DECLARE_PARENT(className, classNameP) info.addParent<classNameP, className>();

template <typename targetClass, typename sourseClass>  
targetClass* casting(sourseClass* soursePtr) {
	size_t realClassHash = soursePtr->getVirtualInfo().getClassHash(); 
	size_t sourseClassHash = soursePtr->getInfo().getClassHash(); 
	targetClass tC; 
	size_t targetClassHash = tC.getInfo().getClassHash();
	size_t offsetUp = 0;
	size_t offsetDown = 0;
	for (int i = 0; i < soursePtr->getVirtualInfo().parent.size(); ++i) {
		std::cout << "%" << soursePtr->getVirtualInfo().parent[i].classN << std::endl;
		if (soursePtr->getVirtualInfo().parent[i].getClassHash() == targetClassHash) {
			offsetUp = soursePtr->getVirtualInfo().offset[i];
		}
		if (soursePtr->getVirtualInfo().parent[i].getClassHash() == sourseClassHash) {
			offsetDown = soursePtr->getVirtualInfo().offset[i];
		}

	}
	return reinterpret_cast<targetClass*>((void *)((size_t)soursePtr - offsetDown + offsetUp));
}


#define DYNAMIC_CAST(targetClass, sourseClass, soursePtr) casting<targetClass, sourseClass>(soursePtr);
	
					

class A {
	RTTI_CONSTRUCT(A)
	RTTI_CONSTRUCT_END(A)
public:
	int a = 1;
	void helloA() {
		std::cout << "I am A!" << std::endl;
	}
};


class C {
	RTTI_CONSTRUCT(C)
	RTTI_CONSTRUCT_END(C)
public:
	int c = 2;
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
	c = DYNAMIC_CAST(C, A, a1);
	c->helloC();
	std::cout << c->c;
	//пытаемся связать два базовых класса через объект-наследник - поле "a" уходит из области видимости,
	//поле "c" появляется и не отождествляется с полем "a"
	c = DYNAMIC_CAST(C, A, a);
	c->helloC();
	std::cout << c->c;
	int n;
	std::cin >> n;
	return 0;
}
