#include "catch.hpp"

#include "..//inkcpp/tree.h"

using ink::runtime::internal::base_tree;
using ink::runtime::internal::tree_node;

struct my_tree_node : tree_node<int>
{
	static int num_items;

	my_tree_node(int key) : tree_node<int>(key)
	{
		num_items++;
	}
	~my_tree_node()
	{
		num_items--;
	}
};

int my_tree_node::num_items = 0;

class test_hash_tree : public base_tree<int>
{
public:
	void add(int key)
	{
		auto node = new my_tree_node(key);
		if (!insert(node))
			delete node;
	}
	void remove(int key)
	{
		// deleteKey(key);
		// DO NOTHING
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

			WHEN("many items are added")
			{
				for(int i = 9; i >= 0; i--)
					tree.add(i);

				THEN("they enter the tree")
				{
					REQUIRE(tree.count() == 10);
					REQUIRE(my_tree_node::num_items == 10);
				}

				THEN("they are removed")
				{
					tree.remove(4);
					REQUIRE(tree.count() == 2);
					REQUIRE(my_tree_node::num_items == 2);
				}
			}
		}
		WHEN("it goes out of scope")
		{
			THEN("all children are deleted")
			{
				REQUIRE(my_tree_node::num_items == 0);
			}
		}
	}
}