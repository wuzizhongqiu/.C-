#pragma once

#include <iostream>
#include <assert.h>
#include <algorithm>
using namespace std;

template<class K, class V>
struct AVLTreeNode {
	AVLTreeNode<K, V>* _left;
	AVLTreeNode<K, V>* _right;
	AVLTreeNode<K, V>* _parent;

	pair<K, V> _kv;
	int _bf; // balance factor 平衡因子

public:
	AVLTreeNode(const pair<K, V>& kv)
		: _left(nullptr)
		, _right(nullptr)
		, _parent(nullptr)
		, _kv(kv)
		, _bf(0)
	{}
};

template<class K, class V>
class AVLTree {
	typedef AVLTreeNode<K, V> Node;

public:
	bool Insert(const pair<K, V>& kv) {
		if (_root == nullptr) {
			_root = new Node(kv);
			return true;
		}
		Node* parent = nullptr;
		Node* cur = _root;
		while (cur) {
			if (cur->_kv.first < kv.first) { //kv大，走右
				parent = cur;
				cur = cur->_right;
			}
			else if (cur->_kv.first > kv.first) { //kv小，走左
				parent = cur;
				cur = cur->_left;
			}
			else return false;
		}
		cur = new Node(kv);
		if (parent->_kv.first < kv.first) {
			parent->_right = cur;
		}
		else {
			parent->_left = cur;
		}
		cur->_parent = parent; //因为是三叉链，所以最后要更新一下他的parent

		//控制平衡
		while (parent) {
			if (cur == parent->_right) {
				parent->_bf++;
			}
			else { //left
				parent->_bf--;
			}
			if (parent->_bf == 0) {
				break;
			}
			else if (abs(parent->_bf) == 1) { //往上翻
				parent = parent->_parent;
				cur = cur->_parent;
			}
			else if (abs(parent->_bf) == 2) { //parent所在子树不平衡，需要旋转
				if (parent->_bf == 2 && cur->_bf == 1) { //左单旋
					RotateL(parent);
				}
				else if (parent->_bf == -2 && cur->_bf == -1) { //右单旋
					RotateR(parent);
				} 
				else if (parent->_bf == -2 && cur->_bf == 1) { //左右双旋	
					RotateLR(parent);
				}
				else if (parent->_bf == 2 && cur->_bf == -1) { //右左双旋
					RotateRL(parent);
				}
				else {
					assert(false);
				}
				break;
			}
			else { //插入之前就有问题
				assert(false);
			}
		}
		return true;
	}

	void Inorder() {
		_Inorder(_root);
		cout << endl;
	}

	int Height() {
		return _Height(_root);
	}
	 
	bool IsBalance() { //看看是否平衡
		return _IsBalance(_root);
	}

private: //封装接口的实现
	bool _IsBalance(Node* root) {
		if (root == nullptr) return true;
		int leftHT = _Height(root->_left);
		int rightHT = _Height(root->_right);
		if ((rightHT - leftHT) != root->_bf) { //检查平衡因子
			cout << root->_kv.first << "平衡因子异常" << endl;
			assert(false);
		}
		return abs(rightHT - leftHT) < 2 && _IsBalance(root->_left) && _IsBalance(root->_right);
	}

	int _Height(Node* root) {
		if (root == nullptr) return 0;
		return max(_Height(root->_left), _Height(root->_right)) + 1;
	}

	void _Inorder(Node* root) {
		if (root == nullptr) return;
		_Inorder(root->_left);
		cout << root->_kv.first << ":" << root->_kv.second << " ";
		_Inorder(root->_right);
	}

private: //下面是旋转
	void RotateL(Node* parent) { //左单旋
		Node* subR = parent->_right;
		Node* subRL = subR->_left;
		Node* ppNode = parent->_parent;
	
		parent->_right = subRL;
		subR->_left = parent;

		//下面是改parent
		if (subRL) subRL->_parent = parent; //小心空指针

		parent->_parent = subR;

		if (_root == parent) { //parent节点就是根节点的情况
			_root = subR;
			subR->_parent = nullptr;
		}
		else { //parent节点还有parent的情况（parent节点是一个子树）
			if (ppNode->_left == parent) {
				ppNode->_left = subR;
			}
			else {
				ppNode->_right = subR;
			}
			subR->_parent = ppNode;
		}
		subR->_bf = parent->_bf = 0;
	}

	void RotateR(Node* parent) { //右单旋 //跟左单旋是一样的
		Node* subL = parent->_left;
		Node* subLR = subL->_right;
		Node* ppNode = parent->_parent;

		parent->_left = subLR;
		subL->_right = parent;

		//下面是改parent
		if (subLR) subLR->_parent = parent; //小心空指针

		parent->_parent = subL;

		if (_root == parent) { //parent节点就是根节点的情况
			_root = subL;
			subL->_parent = nullptr;
		}
		else { //parent节点还有parent的情况（parent节点是一个子树）
			if (ppNode->_left == parent) {
				ppNode->_left = subL;
			}
			else {
				ppNode->_right = subL;
			}
			subL->_parent = ppNode;
		}
		subL->_bf = parent->_bf = 0;
	}

	void RotateLR(Node* parent) { //左右双旋 //主要是平衡因子的转变
		Node* subL = parent->_left;
		Node* subLR = subL->_right;
		int bf = subLR->_bf;

		RotateL(parent->_left);
		RotateR(parent);

		subLR->_bf = 0;
		if (bf == 1) { //插在左孩子
			parent->_bf = 0;
			subL->_bf = -1;
		}
		else if (bf == -1) { //插在右孩子
			parent->_bf = 1;
			subL->_bf = 0;
		}
		else if (bf == 0) { //自己就是插入的
			parent->_bf = 0;
			subL->_bf = 0;
		}
		else {
			assert(false);
		}
	}

	void RotateRL(Node* parent) { //右左双旋 
		Node* subR = parent->_right;
		Node* subRL = subR->_left;
		int bf = subRL->_bf;

		RotateR(parent->_right);
		RotateL(parent);

		subRL->_bf = 0;
		if (bf == 1) {
			parent->_bf = -1;
			subR->_bf = 0;
		}
		else if (bf == -1) {
			parent->_bf = 0;
			subR->_bf = 1;
		}
		else if (bf == 0) {
			parent->_bf = 0;
			subR->_bf = 0;
		}
		else {
			assert(false);
		}
	}

private:
	Node* _root = nullptr;
};