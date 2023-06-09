#pragma once

#include <iostream>
using namespace std;

enum Colour {
	RED,
	BLACK
};

template<class K, class V>
struct RBTreeNode {
	RBTreeNode<K, V>* _left;
	RBTreeNode<K, V>* _right;
	RBTreeNode<K, V>* _parent;
	pair<K, V> _kv;

	Colour _col;

	RBTreeNode(const pair<K, V>& kv)
		: _left(nullptr)
		, _right(nullptr)
		, _parent(nullptr)
		, _col(RED)
		, _kv(kv)
	{}
};

template<class K, class V>
class RBTree {
	typedef RBTreeNode<K, V> Node;
public:
	RBTree()
		:_root(nullptr)
	{}

	bool Insert(const pair<K, V>& kv) {
		if (_root == nullptr) {
			_root = new Node(kv);
			_root->_col = BLACK;
			return true;
		}

		Node* parent = nullptr;
		Node* cur = _root;
		while (cur) {
			if (cur->_kv.first < kv.first) {
				parent = cur;
				cur = cur->_right;
			}
			else if (cur->_kv.first > kv.first) {
				parent = cur;
				cur = cur->_left;
			}
			else {
				return false;
			}
		}

		cur = new Node(kv);
		cur->_col = RED; // 新增节点
		if (parent->_kv.first < kv.first) {
			parent->_right = cur;
			cur->_parent = parent;
		}
		else {
			parent->_left = cur;
			cur->_parent = parent;
		}

		// 控制平衡
		while (parent && parent->_col == RED) {
			Node* grandfather = parent->_parent;
			if (parent == grandfather->_left) {
				Node* uncle = grandfather->_right;
				// 1、uncle存在且为红
				if (uncle && uncle->_col == RED) {
					// 变色+继续向上处理
					parent->_col = uncle->_col = BLACK;
					grandfather->_col = RED;

					cur = grandfather;
					parent = cur->_parent;
				}
				else { // 2 + 3、uncle不存在/ 存在且为黑 
					//        g
					//     p
					//  c

					//        g
					//     p
					//        c

					if (cur == parent->_left) {
						// 单旋
						RotateR(grandfather);
						parent->_col = BLACK;
						grandfather->_col = RED;
					}
					else {
						// 双旋
						RotateL(parent);
						RotateR(grandfather);
						cur->_col = BLACK;
						grandfather->_col = RED;
					}

					break;
				}
			}
			else { // parent == grandfather->_right
				Node* uncle = grandfather->_left;
				if (uncle && uncle->_col == RED) {
					// 变色+继续向上处理
					parent->_col = uncle->_col = BLACK;
					grandfather->_col = RED;

					cur = grandfather;
					parent = cur->_parent;
				}
				else { // 2 + 3、uncle不存在/ 存在且为黑
					//  g    
					//     p
					//        c

					//  g
					//     p
					//  c
					if (cur == parent->_right) {
						RotateL(grandfather);
						parent->_col = BLACK;
						grandfather->_col = RED;
					}
					else {
						RotateR(parent);
						RotateL(grandfather);
						cur->_col = BLACK;
						grandfather->_col = RED;
					}
					break;
				}
			}
		}

		_root->_col = BLACK;

		return true;
	}

	void RotateL(Node* parent) {
		Node* subR = parent->_right;
		Node* subRL = subR->_left;

		parent->_right = subRL;
		if (subRL) {
			subRL->_parent = parent;
		}

		Node* parentParent = parent->_parent;
		subR->_left = parent;
		parent->_parent = subR;

		if (_root == parent) {
			_root = subR;
			subR->_parent = nullptr;
		}
		else {
			if (parentParent->_left == parent)
				parentParent->_left = subR;
			else
				parentParent->_right = subR;
			subR->_parent = parentParent;
		}
	}

	void RotateR(Node* parent) {
		Node* subL = parent->_left;
		Node* subLR = subL->_right;

		parent->_left = subLR;
		if (subLR)
			subLR->_parent = parent;

		Node* parentParent = parent->_parent;

		subL->_right = parent;
		parent->_parent = subL;

		if (parent == _root) {
			_root = subL;
			_root->_parent = nullptr;
		}
		else {
			if (parentParent->_left == parent)
				parentParent->_left = subL;
			else
				parentParent->_right = subL;

			subL->_parent = parentParent;
		}
	}

	void InOrder() {
		_InOrder(_root);
	}

	void _InOrder(Node* root) {
		if (root == NULL)
			return;

		_InOrder(root->_left);
		cout << root->_kv.first << ":" << root->_kv.second << endl;
		_InOrder(root->_right);
	}

	bool IsBalance() {
		if (_root && _root->_col == RED) {
			cout << "根节点不是黑色" << endl;
			return false;
		}

		// 最左路径黑色节点数量做基准值
		int banchmark = 0;
		Node* left = _root;
		while (left) {
			if (left->_col == BLACK)
				++banchmark;

			left = left->_left;
		}

		int blackNum = 0;
		return _IsBalance(_root, banchmark, blackNum);
	}

	bool _IsBalance(Node* root, int banchmark, int blackNum) {
		if (root == nullptr) {
			if (banchmark != blackNum) {
				cout << "存在路径黑色节点的数量不相等" << endl;
				return false;
			}

			return true;
		}

		if (root->_col == RED && root->_parent->_col == RED) {
			cout << "出现连续红色节点" << endl;
			return false;
		}

		if (root->_col == BLACK) {
			++blackNum;
		}

		return _IsBalance(root->_left, banchmark, blackNum)
			&& _IsBalance(root->_right, banchmark, blackNum);
	}

	int Height() {
		return _Height(_root);
	}

	int _Height(Node* root) {
		if (root == NULL)
			return 0;
		int leftHeight = _Height(root->_left);
		int rightHeight = _Height(root->_right);
		return leftHeight > rightHeight ? leftHeight + 1 : rightHeight + 1;
	}

private:
	Node* _root;
};