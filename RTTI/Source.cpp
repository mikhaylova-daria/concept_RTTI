#include "macros.h"

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
