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
	int _bf; // balance factor ƽ������

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
			if (cur->_kv.first < kv.first) { //kv������
				parent = cur;
				cur = cur->_right;
			}
			else if (cur->_kv.first > kv.first) { //kvС������
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
		cur->_parent = parent; //��Ϊ�����������������Ҫ����һ������parent

		//����ƽ��
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
			else if (abs(parent->_bf) == 1) { //���Ϸ�
				parent = parent->_parent;
				cur = cur->_parent;
			}
			else if (abs(parent->_bf) == 2) { //parent����������ƽ�⣬��Ҫ��ת
				if (parent->_bf == 2 && cur->_bf == 1) { //����
					RotateL(parent);
				}
				else if (parent->_bf == -2 && cur->_bf == -1) { //�ҵ���
					RotateR(parent);
				} 
				else if (parent->_bf == -2 && cur->_bf == 1) { //����˫��	
					RotateLR(parent);
				}
				else if (parent->_bf == 2 && cur->_bf == -1) { //����˫��
					RotateRL(parent);
				}
				else {
					assert(false);
				}
				break;
			}
			else { //����֮ǰ��������
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
	 
	bool IsBalance() { //�����Ƿ�ƽ��
		return _IsBalance(_root);
	}

private: //��װ�ӿڵ�ʵ��
	bool _IsBalance(Node* root) {
		if (root == nullptr) return true;
		int leftHT = _Height(root->_left);
		int rightHT = _Height(root->_right);
		if ((rightHT - leftHT) != root->_bf) { //���ƽ������
			cout << root->_kv.first << "ƽ�������쳣" << endl;
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

private: //��������ת
	void RotateL(Node* parent) { //����
		Node* subR = parent->_right;
		Node* subRL = subR->_left;
		Node* ppNode = parent->_parent;
	
		parent->_right = subRL;
		subR->_left = parent;

		//�����Ǹ�parent
		if (subRL) subRL->_parent = parent; //С�Ŀ�ָ��

		parent->_parent = subR;

		if (_root == parent) { //parent�ڵ���Ǹ��ڵ�����
			_root = subR;
			subR->_parent = nullptr;
		}
		else { //parent�ڵ㻹��parent�������parent�ڵ���һ��������
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

	void RotateR(Node* parent) { //�ҵ��� //��������һ����
		Node* subL = parent->_left;
		Node* subLR = subL->_right;
		Node* ppNode = parent->_parent;

		parent->_left = subLR;
		subL->_right = parent;

		//�����Ǹ�parent
		if (subLR) subLR->_parent = parent; //С�Ŀ�ָ��

		parent->_parent = subL;

		if (_root == parent) { //parent�ڵ���Ǹ��ڵ�����
			_root = subL;
			subL->_parent = nullptr;
		}
		else { //parent�ڵ㻹��parent�������parent�ڵ���һ��������
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

	void RotateLR(Node* parent) { //����˫�� //��Ҫ��ƽ�����ӵ�ת��
		Node* subL = parent->_left;
		Node* subLR = subL->_right;
		int bf = subLR->_bf;

		RotateL(parent->_left);
		RotateR(parent);

		subLR->_bf = 0;
		if (bf == 1) { //��������
			parent->_bf = 0;
			subL->_bf = -1;
		}
		else if (bf == -1) { //�����Һ���
			parent->_bf = 1;
			subL->_bf = 0;
		}
		else if (bf == 0) { //�Լ����ǲ����
			parent->_bf = 0;
			subL->_bf = 0;
		}
		else {
			assert(false);
		}
	}

	void RotateRL(Node* parent) { //����˫�� 
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