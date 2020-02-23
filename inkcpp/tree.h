#pragma once

//#include "system.h"

namespace ink::runtime::internal
{
	// Tree node
	struct tree_meta_node
	{
	private:
		friend class base_meta_tree;

		// info
		int balance;
		int height;

	protected:
		// children
		tree_meta_node* _left;
		tree_meta_node* _right;

		// parent
		tree_meta_node* _parent;

		tree_meta_node() : balance(0), height(0), _left(nullptr), _right(nullptr), _parent(nullptr) { }
	};

	template<typename Key>
	struct tree_node : public tree_meta_node
	{
	public:
		tree_node(Key k) : tree_meta_node(), key(k) { }

	private:
		template<typename T>
		friend class base_tree;

		// Unique key
		Key key;

		inline tree_node<Key>*& left() { return reinterpret_cast<tree_node<Key>*&>(_left); }
		inline tree_node<Key>*& right() { return reinterpret_cast<tree_node<Key>*&>(_right); }
		inline tree_node<Key>*& parent() { return reinterpret_cast<tree_node<Key>*&>(_parent); }
	};

	class base_meta_tree
	{
	protected:
		base_meta_tree();

		// rebalances the tree rooted at the node
		void rebalance(tree_meta_node**);

		// tree root
		tree_meta_node* _root;
	private:
		// height function
		inline int height(tree_meta_node* node) { return node == nullptr ? -1 : node->height; }

		// == rotations ==
		void left_rotate(tree_meta_node**);
		void right_rotate(tree_meta_node**);
		void left_right_rotate(tree_meta_node**);
		void right_left_rotate(tree_meta_node**);

		// updates a node's balance
		void update_balance(tree_meta_node*);
	};

	template<typename Key>
	class base_tree : public base_meta_tree
	{
	protected:
		typedef tree_node<Key> node_type;
		typedef tree_node<Key>* node_ptr_type;

		// insert pre-allocated node
		bool insert(node_ptr_type node);

		// iteration
		node_ptr_type first();
		void next(node_ptr_type& iter);

	private:
		// root access
		inline node_ptr_type& root() { return reinterpret_cast<node_ptr_type&>(_root); }
	};

	template<typename Key>
	bool base_tree<Key>::insert(node_ptr_type node)
	{
		// Special case: empty tree
		if (_root == nullptr) {
			root() = node;
			return true;
		}

		// Start at the root node
		node_ptr_type* iter = &root();

		// Iterate
		while (true)
		{
			// Duplicate!
			if ((*iter)->key == node->key)
				return false;

			// Go left if the node to insert is less than the iterator
			bool goLeft = node->key < (*iter)->key;
			node_ptr_type* next = goLeft ? &(*iter)->left() : &(*iter)->right();

			// If the spot is empty
			if (*next == nullptr)
			{
				// Insert into that spot
				*next = node;

				// Update parent pointer
				node->parent() = *iter;

				// Rebalance
				rebalance(reinterpret_cast<tree_meta_node**>(iter));
				break;
			}

			// Move iterator
			iter = next;
		}

		return true;
	}
	template<typename Key>
	inline typename base_tree<Key>::node_ptr_type base_tree<Key>::first()
	{
		if (_root == nullptr)
			return nullptr;

		// find left-most node
		auto iter = root();
		while (iter->left() != nullptr)
			iter = iter->left();

		return iter;
	}

	template<typename Key>
	inline void base_tree<Key>::next(node_ptr_type& iter)
	{
		// nothing to dp
		if (iter == nullptr)
			return;

		// if we have a right child
		if (iter->right() != nullptr)
		{
			// fall into the leftmost leaf of that sub-tree
			iter = iter->right();
			while (iter->left() != nullptr)
				iter = iter->left();
		}
		else // no children
		{
			// climb until we find a parent larger than us
			Key key = iter->key;
			iter = iter->parent();
			while (iter != nullptr && iter->key < key)
				iter = iter->parent();
		}
	}
}