#define _CRT_SECURE_NO_WARNINGS 1

#include "RBTree.h"
#include <vector>

int main()
{
	vector<int> v{ 2, 4, 1, 3 };
	RBTree<int, int> t;
	for (auto e : v) {
		t.Insert({e, e});
	}
	t.InOrder();
	return 0;
}