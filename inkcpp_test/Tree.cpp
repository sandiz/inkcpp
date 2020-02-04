#include "catch.hpp"

#include "..//inkcpp/tree.h"

using ink::runtime::internal::base_hash_tree;
using ink::runtime::internal::hash_tree_node;

struct tree_node : hash_tree_node<int>
{
	static int num_items;

	tree_node(int key) : hash_tree_node<int>(key, nullptr)
	{
		num_items++;
	}
	~tree_node()
	{
		num_items--;
	}
};

int tree_node::num_items = 0;

class test_hash_tree : public base_hash_tree<int>
{
public:
	void add(int key)
	{
		auto node = new tree_node(key);
		if (!insert(node))
			delete node;
	}
	void remove(int key)
	{
		deleteKey(key);
	}
	int count()
	{
		int count = 0;
		auto iter = first();
		while (iter != nullptr)
		{
			count++;
			next(iter);
		}

		return count;
	}
};

SCENARIO("hash trees work", "[tree]")
{
	GIVEN("an empty tree") 
	{
		{
			test_hash_tree tree;

			WHEN("items are added")
			{
				tree.add(2);
				tree.add(4);
				tree.add(9);

				THEN("they enter the tree")
				{
					REQUIRE(tree.count() == 3);
					REQUIRE(tree_node::num_items == 3);
				}

				THEN("they are removed")
				{
					tree.remove(4);
					REQUIRE(tree.count() == 2);
					REQUIRE(tree_node::num_items == 2);
				}
			}
		}
		WHEN("it goes out of scope")
		{
			THEN("all children are deleted")
			{
				REQUIRE(tree_node::num_items == 0);
			}
		}
	}
}