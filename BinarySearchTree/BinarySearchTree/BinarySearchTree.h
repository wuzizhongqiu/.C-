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
	//BSTree() {} //��Ϊд�˹��캯�������Ա������Ͳ�������Ĭ�ϵĹ����ˣ�����Ҫ�Լ�дһ��
	BSTree() = default; //C++11 ǿ�Ʊ���������һ��Ĭ�Ϲ���

	BSTree(const BSTree<K>& k) { _root = _Copy(k._root); } //��������

	BSTree<K>& operator=(BSTree<K> k) { //��ֵ����
		swap(_root, k._root); //��*this���ο����������β�k
		return *this;
	}

	~BSTree() { _Destroy(_root); } //����
public:
	bool Insert(const K& key) { //�������
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

	bool Erase(const K& key) { //ɾ��
		Node* parent = nullptr;
		Node* cur = _root;
		while (cur) { //����
			if (key > cur->_key) {
				parent = cur;
				cur = cur->_right;
			}
			else if (key < cur->_key) {
				parent = cur;
				cur = cur->_left;
			}
			else { //��ʼɾ��
				if (cur->_left == nullptr) { //�ڵ�����Ϊ��
					if (cur == _root) { //�ڵ��Ǹ��ڵ�
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
					delete cur; //deleteĿ��ڵ�
					cur = nullptr;
				}
				else if (cur->_right == nullptr) { //�ڵ��Һ���Ϊ��
					if (cur == _root) { //�ڵ��Ǹ��ڵ�
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
					delete cur; //deleteĿ��ڵ�
					cur = nullptr;
				}
				else { //�ڵ����Һ��Ӷ���Ϊ��
					//�滻��ɾ����������ͳһ����������С�ڵ�����滻�������������ڵ�Ҳ�����ã�
					Node* prev = cur; //prev��change�ĸ��׽ڵ�
					Node* change = cur->_right; //change��(��������С�ڵ�)
					while (change->_left) {
						prev = change;
						change = change->_left;
					}

					cur->_key = change->_key; //swap(cur->_key, change->_key)

					if(prev->_left == change) prev->_left = change->_right;
					else prev->_right = change->_right; //change�ڵ��ڸ��׽ڵ���ұߵ��������

					delete change;
					change = nullptr;
				}
				return true;
			}
		}
		return false;
	}

	void Inorder() { _Inorder(_root); } //�������

	bool Find(const K& key) { //����
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
	
	//*******************************************�ݹ�***************************************//

	bool FindR(const K& key) { return _FindR(_root, key); }

	bool InsertR(const K& key) { return _InsertR(_root, key); }

	bool EraseR(const K& key) { return _EraseR(_root, key); }

private: //��װ����
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

	bool _EraseR(Node*& root, const K& key) { //��δ���Ҳ�������õľ���
		if (root == nullptr) return false;
		if (key > root->_key) return _EraseR(root->_right, key);
		else if (key < root->_key) return _EraseR(root->_left, key);
		else { //��ʼɾ��
			Node* del = root; //��¼Ҫɾ���Ľڵ�
			if (root->_left == nullptr) root = root->_right;
			else if (root->_right == nullptr) root = root->_left;
			else { //����������Ӷ���Ϊ��
				Node* min = root->_right;
				while (min->_left) min = min->_left;
				swap(root->_key, min->_key);
				return _EraseR(root->_right, key); //�����������ɾ��
			}
			delete del;
			return true;
		}
	}

	bool _InsertR(Node*& root, const K& key) {
		if (root == nullptr) {
			root = new Node(key); //��Ϊ�������ã�root->right / root->left = new(key) ����֮һ�֣�
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

//����
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