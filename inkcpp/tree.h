#pragma once

#include "system.h"

// Using code modified from from https://rosettacode.org/wiki/AVL_tree#C.2B.2B

namespace ink::runtime::internal
{
	// base node for hash tree
	template<typename Key>
	struct hash_tree_node
	{
		// unique key
		Key key;

		// balance record
		int balance;

		// branches and parent
		hash_tree_node* left, * right, * parent;

		// constructor
		hash_tree_node(Key k, hash_tree_node* p) : key(k), balance(0), parent(p), left(nullptr), right(nullptr) { }

		// destructor
		virtual ~hash_tree_node()
		{
			if(left != nullptr)
				delete left;
			if(right != nullptr)
				delete right;

			left = right = nullptr;
		}
	};

	// base class for hash trees
	template<typename Key>
	class base_hash_tree
	{
	protected:
		base_hash_tree();
		virtual ~base_hash_tree();

		bool insert(hash_tree_node<Key>*);
		void deleteKey(const Key key);
		hash_tree_node<Key>* deleteInline(hash_tree_node<Key>* node);
		hash_tree_node<Key>* find(const Key key);

		hash_tree_node<Key>* first();
		void next(hash_tree_node<Key>*& iter);

		inline hash_tree_node<Key>* root() { return _root; }
	private:
		// root node (nullptr for an empty tree)
		hash_tree_node<Key>* _root;
		hash_tree_node<Key>* rotateLeft(hash_tree_node<Key>* a);
		hash_tree_node<Key>* rotateRight(hash_tree_node<Key>* a);
		hash_tree_node<Key>* rotateLeftThenRight(hash_tree_node<Key>* n);
		hash_tree_node<Key>* rotateRightThenLeft(hash_tree_node<Key>* n);
		void rebalance(hash_tree_node<Key>* n);
		int height(hash_tree_node<Key>* n);
		void setBalance(hash_tree_node<Key>* n);
	};

	/* AVL class definition */
	template <typename Key>
	void base_hash_tree<Key>::rebalance(hash_tree_node<Key>* n) {
		setBalance(n);

		if (n->balance == -2) {
			if (height(n->left->left) >= height(n->left->right))
				n = rotateRight(n);
			else
				n = rotateLeftThenRight(n);
		}
		else if (n->balance == 2) {
			if (height(n->right->right) >= height(n->right->left))
				n = rotateLeft(n);
			else
				n = rotateRightThenLeft(n);
		}

		if (n->parent != NULL) {
			rebalance(n->parent);
		}
		else {
			_root = n;
		}
	}

	template <typename Key>
	hash_tree_node<Key>* base_hash_tree<Key>::rotateLeft(hash_tree_node<Key>* a) {
		hash_tree_node<Key>* b = a->right;
		b->parent = a->parent;
		a->right = b->left;

		if (a->right != NULL)
			a->right->parent = a;

		b->left = a;
		a->parent = b;

		if (b->parent != NULL) {
			if (b->parent->right == a) {
				b->parent->right = b;
			}
			else {
				b->parent->left = b;
			}
		}

		setBalance(a);
		setBalance(b);
		return b;
	}

	template <typename Key>
	hash_tree_node<Key>* base_hash_tree<Key>::rotateRight(hash_tree_node<Key>* a) {
		hash_tree_node<Key>* b = a->left;
		b->parent = a->parent;
		a->left = b->right;

		if (a->left != NULL)
			a->left->parent = a;

		b->right = a;
		a->parent = b;

		if (b->parent != NULL) {
			if (b->parent->right == a) {
				b->parent->right = b;
			}
			else {
				b->parent->left = b;
			}
		}

		setBalance(a);
		setBalance(b);
		return b;
	}

	template <typename Key>
	hash_tree_node<Key>* base_hash_tree<Key>::rotateLeftThenRight(hash_tree_node<Key>* n) {
		n->left = rotateLeft(n->left);
		return rotateRight(n);
	}

	template <typename Key>
	hash_tree_node<Key>* base_hash_tree<Key>::rotateRightThenLeft(hash_tree_node<Key>* n) {
		n->right = rotateRight(n->right);
		return rotateLeft(n);
	}

	template <typename Key>
	int base_hash_tree<Key>::height(hash_tree_node<Key>* n) {
		if (n == NULL)
			return -1;

		int leftHeight = height(n->left), rightHeight = height(n->right);
		return 1 + (leftHeight > rightHeight ? leftHeight : rightHeight); //std::max(height(n->left), height(n->right));
	}

	template <typename Key>
	void base_hash_tree<Key>::setBalance(hash_tree_node<Key>* n) {
		n->balance = height(n->right) - height(n->left);
	}

	template <typename Key>
	base_hash_tree<Key>::base_hash_tree() : _root(NULL) {}

	template <typename Key>
	base_hash_tree<Key>::~base_hash_tree(void) {
		delete _root;
	}

	template <typename Key>
	hash_tree_node<Key>* base_hash_tree<Key>::find(const Key key)
	{
		// search
		hash_tree_node<Key> *n = _root;
		while (n != nullptr)
		{
			// found!
			if (n->key == key)
				return n;
			
			// search left or right based on key
			bool goLeft = n->key > key;
			n = goLeft ? n->left : n->right;
		}

		// nothing found
		return nullptr;
	}

	template<typename Key>
	inline hash_tree_node<Key>* base_hash_tree<Key>::first()
	{
		if (_root == nullptr)
			return nullptr;

		auto iter = _root;
		while (iter->left != nullptr)
		{
			iter = iter->left;
		}

		return iter;
	}

	template<typename Key>
	inline void base_hash_tree<Key>::next(hash_tree_node<Key>*& iter)
	{
		// nothing to do
		if (iter == nullptr)
			return;

		// if we have a right child
		if (iter->right != nullptr)
		{
			// fall into the leftmost leaf of that sub-tree
			iter = iter->right;
			while (iter->left != nullptr)
				iter = iter->left;
		}
		else // no children
		{
			// climb until we find a parent larger than us
			Key key = iter->key;
			iter = iter->parent;
			while (iter != nullptr && iter->key < key)
				iter = iter->parent;
		}
	}

	template <typename Key>
	bool base_hash_tree<Key>::insert(hash_tree_node<Key>* node) {
		if (_root == NULL) {
			_root = node;
		}
		else {
			hash_tree_node<Key>
				* n = _root,
				* parent;

			while (true) {
				if (n->key == node->key)
					return false;

				parent = n;

				bool goLeft = n->key > node->key;
				n = goLeft ? n->left : n->right;

				if (n == NULL) {
					if (goLeft) {
						parent->left = node;
						node->parent = parent;
					}
					else {
						parent->right = node;
						node->parent = parent;
					}

					rebalance(parent);
					break;
				}
			}
		}

		return true;
	}

	template <typename Key>
	void base_hash_tree<Key>::deleteKey(const Key delKey) {
		if (_root == NULL)
			return;

		hash_tree_node<Key>
			* n = _root,
			* parent = _root,
			* delNode = NULL,
			* child = _root;

		while (child != NULL) {
			parent = n;
			n = child;
			child = delKey >= n->key ? n->right : n->left;
			if (delKey == n->key)
			{
				delNode = n;
				break;
			}
		}

		if (delNode != nullptr)
		{
			deleteInline(delNode);
		}
	}

	template<typename Key>
	inline hash_tree_node<Key>* base_hash_tree<Key>::deleteInline(hash_tree_node<Key>* node)
	{
		// cache the next node for iteration
		auto cont = node;
		next(cont);

		// ?????

		// find successor
		auto n = node;
		auto child = node->right;
		while (child != nullptr && child->left != nullptr)
		{
			n = child;
			child = child->left;
		}
		auto parent = n->parent;

		// change deleted nodes key to successor and grab child
		node->key = n->key;
		child = n->left != NULL ? n->left : n->right;

		// deleting root with only one child? fuck idk
		if (_root->key == node->key) {
			auto old = _root;
			_root = child;

			// delete old root
			old->left = old->right = nullptr;
			delete old;
		}
		else {
			// deleted node needs to be replaced by n
			if (node->parent->left == node)
				node->parent->left = n;
			else
				node->parent->right = n;
			n->left = node->left; n->right = node->right; n->parent = node->parent;

			// n needs to be replaced by its child
			if (parent->left == n) {
				parent->left = child;
			}
			else {
				parent->right = child;
			}

			// remove deleted node
			node->left = node->right = nullptr;
			delete node;

			// rebalance
			rebalance(parent);
		}
		
		return cont;
	}
}