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
		//	std::cout << "addP" << std::endl;
		P p;
		C c = C(0);
		parent.push_back(p.getInfo());
		offset.push_back((size_t)(static_cast<P*>(&c)) - (size_t)(&c));
		for (int i = 0; i < p.getInfo().parent.size(); ++i) {
			parent.push_back(p.getInfo().parent[i]);
			offset.push_back((size_t)(static_cast<P*>(&c)) - (size_t)(&c) + p.getInfo().offset[i]);
		}
		//	std::cout << "addPEnd" << std::endl;
	}
	my_type_info(){ ; }
	my_type_info(std::string name) :classN(name){ ; }
};


template <typename targetClass, typename sourseClass>
targetClass* casting(sourseClass* soursePtr) {
	size_t realClassHash = soursePtr->getVirtualInfo().getClassHash();
	size_t sourseClassHash = soursePtr->getInfo().getClassHash();
	targetClass tC;
	size_t targetClassHash = tC.getInfo().getClassHash();
	size_t offsetUp = 0;
	size_t offsetDown = 0;
	size_t deep = 0;
	for (int i = 0; i < soursePtr->getVirtualInfo().parent.size(); ++i) {
		//	std::cout << "%" << soursePtr->getVirtualInfo().parent[i].classN << std::endl;
		if (soursePtr->getVirtualInfo().parent[i].getClassHash() == targetClassHash) {
			offsetUp = soursePtr->getVirtualInfo().offset[i];
		}
		if (soursePtr->getVirtualInfo().parent[i].getClassHash() == sourseClassHash) {
			offsetDown = soursePtr->getVirtualInfo().offset[i];
		}

	}
	return reinterpret_cast<targetClass*>((void *)((size_t)soursePtr - offsetDown + offsetUp));
}


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
			info.classN = #className;\

#define RTTI_CONSTRUCT_END(className)}\
	private:

#define DECLARE_PARENT(className, classNameP) info.addParent<classNameP, className>();


#define DYNAMIC_CAST(targetClass, sourseClass, soursePtr) casting<targetClass, sourseClass>(soursePtr);
