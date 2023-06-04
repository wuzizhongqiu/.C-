#define _CRT_SECURE_NO_WARNINGS 1

#include "AVLTree.h"
#include <vector>


void test() {
	vector<int> v{ 16, 3, 7, 11, 9, 26, 18, 14, 15 };
	AVLTree<int, int> t;
	for (auto e : v) {
		t.Insert(make_pair(e, e));
	}
	t.Inorder();
	cout << t.IsBalance() << endl;
}

int main()
{
	test();
	return 0;
}