#include "tree.h"

namespace ink::runtime::internal
{
	base_meta_tree::base_meta_tree() : _root(nullptr)
	{
	}

	void base_meta_tree::rebalance(tree_meta_node** node)
	{
		// make sure balance numbers are correct
		update_balance(*node);

		// rebalance if needed
		if ((*node)->balance == -2)
		{
			if (height((*node)->_left->_left) >= height((*node)->_left->_right))
				right_rotate(node);
			else
				left_right_rotate(node);
		}
		else if ((*node)->balance == 2)
		{
			if (height((*node)->_right->_right) >= height((*node)->_right->_left))
				left_rotate(node);
			else
				right_left_rotate(node);
		}

		// move up the tree
		tree_meta_node* parent = (*node)->_parent;
		if (parent != nullptr)
		{
			if (parent->_parent != nullptr)
			{
				if (parent == parent->_parent->_left)
					rebalance(&parent->_parent->_left);
				else
					rebalance(&parent->_parent->_right);
			}
			else
			{
				rebalance(&_root);
			}
		}
		else
		{
			// this is the new root
			_root = *node;
		}
	}

	void base_meta_tree::left_rotate(tree_meta_node** node)
	{
		// Cache current root. It will be the new left branch
		tree_meta_node* new_left = *node;

		// Set the root to the right branch
		*node = (*node)->_right;
		(*node)->_parent = new_left->_parent;

		// Move the new root's left branch into the shifted node's right
		new_left->_right = (*node)->_left;
		if(new_left->_right != nullptr)
			new_left->_right->_parent = new_left;

		// Set the left branch of the new root to the previous root
		(*node)->_left = new_left;
		new_left->_parent = (*node);

		// Update balance
		update_balance(new_left);
		update_balance(*node);
	}
	void base_meta_tree::right_rotate(tree_meta_node** node)
	{
		// Cache current root. It will be the new right branch
		tree_meta_node* new_right = *node;

		// Set the root to the right branch
		*node = (*node)->_left;
		(*node)->_parent = new_right->_parent;

		// Move the new root's right branch into the shifted node's left
		new_right->_left = (*node)->_right;
		if (new_right->_left != nullptr)
			new_right->_left->_parent = new_right;

		// Set the right branch of the new root to the previous root
		(*node)->_right = new_right;
		new_right->_parent = (*node);

		// Update balance
		update_balance(new_right);
		update_balance(*node);
	}

	void base_meta_tree::left_right_rotate(tree_meta_node** node)
	{
		left_rotate(node);
		right_rotate(node);
	}

	void base_meta_tree::right_left_rotate(tree_meta_node** node)
	{
		right_rotate(node);
		left_rotate(node);
	}

	void base_meta_tree::update_balance(tree_meta_node* node)
	{
		// Get child heights
		int left = height(node->_left), right = height(node->_right);

		// Update our height
		node->height = 1 + (left > right ? left : right);

		// Update balance
		node->balance = right - left;
	}
}