#pragma once

#include <iostream>
using namespace std;

template<class K>
class BSTreeNode {
public:
	BSTreeNode<K>* _left;
	BSTreeNode<K>* _right;
	K _key;

	BSTreeNode(int key) 
		: _left(nullptr)
		, _right(nullptr)
		, _key(key)
	{}
};

template<class K>
class BSTree {
	typedef BSTreeNode<K> Node;
public:
	//BSTree() {} //因为写了构造函数，所以编译器就不会生成默认的构造了，就需要自己写一个
	BSTree() = default; //C++11 强制编译器生成一个默认构造

	BSTree(const BSTree<K>& k) { _root = _Copy(k._root); } //拷贝构造

	BSTree<K>& operator=(BSTree<K> k) { //赋值构造
		swap(_root, k._root); //让*this白嫖拷贝过来的形参k
		return *this;
	}

	~BSTree() { _Destroy(_root); } //析构
public:
	bool Insert(const K& key) { //插入操作
		if (_root == nullptr) {
			_root = new Node(key);
			return true;
		}
		Node* parent = nullptr;
		Node* cur = _root;
		while (cur) {
			if (key > cur->_key) {
				parent = cur;
				cur = cur->_right;	
			}
			else if (key < cur->_key) {
				parent = cur;
				cur = cur->_left;
			}
			else {
				return false;
			}
		}
		cur = new Node(key);
		if (key > parent->_key) parent->_right = cur;
		else parent->_left = cur;
	}

	bool Erase(const K& key) { //删除
		Node* parent = nullptr;
		Node* cur = _root;
		while (cur) { //查找
			if (key > cur->_key) {
				parent = cur;
				cur = cur->_right;
			}
			else if (key < cur->_key) {
				parent = cur;
				cur = cur->_left;
			}
			else { //开始删除
				if (cur->_left == nullptr) { //节点左孩子为空
					if (cur == _root) { //节点是根节点
						_root = cur->_right;
					}
					else {
						if (cur == parent->_left) {
							parent->_left = cur->_right;
						}
						else {
							parent->_right = cur->_right;
						}
					}
					delete cur; //delete目标节点
					cur = nullptr;
				}
				else if (cur->_right == nullptr) { //节点右孩子为空
					if (cur == _root) { //节点是根节点
						_root = cur->_left;
					}
					else {
						if (cur == parent->_left) {
							parent->_left = cur->_left;
						}
						else {
							parent->_right = cur->_left;
						}
					}
					delete cur; //delete目标节点
					cur = nullptr;
				}
				else { //节点左右孩子都不为空
					//替换法删除（我这里统一找右树的最小节点进行替换）（左树的最大节点也可以用）
					Node* prev = cur; //prev是change的父亲节点
					Node* change = cur->_right; //change是(右树的最小节点)
					while (change->_left) {
						prev = change;
						change = change->_left;
					}

					cur->_key = change->_key; //swap(cur->_key, change->_key)

					if(prev->_left == change) prev->_left = change->_right;
					else prev->_right = change->_right; //change节点在父亲节点的右边的特殊情况

					delete change;
					change = nullptr;
				}
				return true;
			}
		}
		return false;
	}

	void Inorder() { _Inorder(_root); } //中序遍历

	bool Find(const K& key) { //查找
		Node* cur = _root;
		while (cur) {
			if (key > cur->_key) {
				cur = cur->_right;
			}
			else if (key < cur->_key) {
				cur = cur->_left;
			}
			else {
				return true;
			}
		}
		return false;
	}
	
	//*******************************************递归***************************************//

	bool FindR(const K& key) { return _FindR(_root, key); }

	bool InsertR(const K& key) { return _InsertR(_root, key); }

	bool EraseR(const K& key) { return _EraseR(_root, key); }

private: //封装方法
	Node* _Copy(Node* root) {
		if (root == nullptr) return nullptr;
		Node* copyNode = new Node(root->_key);
		copyNode->_left = _Copy(root->_left);
		copyNode->_right = _Copy(root->_right);
		return copyNode;
	}

	void _Destroy(Node*& root) {
		if (root == nullptr) return;
		_Destroy(root->_left);
		_Destroy(root->_right);
		delete root;
		root = nullptr;
	}

	bool _EraseR(Node*& root, const K& key) { //这段代码也体现引用的精髓
		if (root == nullptr) return false;
		if (key > root->_key) return _EraseR(root->_right, key);
		else if (key < root->_key) return _EraseR(root->_left, key);
		else { //开始删除
			Node* del = root; //记录要删除的节点
			if (root->_left == nullptr) root = root->_right;
			else if (root->_right == nullptr) root = root->_left;
			else { //如果两个孩子都不为空
				Node* min = root->_right;
				while (min->_left) min = min->_left;
				swap(root->_key, min->_key);
				return _EraseR(root->_right, key); //在这个子树下删除
			}
			delete del;
			return true;
		}
	}

	bool _InsertR(Node*& root, const K& key) {
		if (root == nullptr) {
			root = new Node(key); //因为用了引用：root->right / root->left = new(key) （神之一手）
			return true;
		}
		if (key > root->_key) return _InsertR(root->_right, key);
		else if (key < root->_key) return _InsertR(root->_left, key);
		else return false;
	}

	bool _FindR(Node* root, const K& key) {
		if (root == nullptr) return false;
		if (key > root->_key) return _FindR(root->_right, key);
		else if (key < root->_key) return _FindR(root->_left, key);
		else return true;
	}

	void _Inorder(Node* root) { 
		if (root == nullptr) return;
		_Inorder(root->_left);
		cout << root->_key << " ";
		_Inorder(root->_right); 
	}
private:
	Node* _root = nullptr;
};

//测试
void test_bstree() {
	//BSTree<int> t; 
	//int a[]{ 1, 3, 4, 6, 7, 8, 10, 13, 14 };	
	//for (auto e : a) {
	//	t.Insert(e);
	//}
	//t.Inorder();
	//cout << endl;

	//t.EraseR(3);
	//t.Inorder();
	//cout << endl;

	//t.EraseR(8);
	//t.Inorder();
	//cout << endl;

	//for (auto e : a) {
	//	t.EraseR(e);
	//	t.Inorder();
	//	cout << endl;
	//}

	BSTree<int> t;
	int a[]{ 1, 3, 4, 6, 7, 8, 10, 13, 14 };	
	for (auto e : a) t.Insert(e);
	BSTree<int> t2;
	t2 = t;
}