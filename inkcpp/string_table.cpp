/*#include "string_table.h"

namespace ink::runtime::internal
{
	// internal structure used to store strings
	struct string_table_node : public hash_tree_node<const char*>
	{
		bool used;

		void clear_usage()
		{
			used = false;
			if (left != nullptr)
				((string_table_node*)left)->clear_usage();
			if (right != nullptr)
				((string_table_node*)right)->clear_usage();
		}
	};


	char* string_table::create(size_t length)
	{
		// allocate a node with extra padding for the string
		string_table_node* node = (string_table_node*)malloc(sizeof(string_table_node) + sizeof(char) * length);
		if (node == nullptr)
			return nullptr;
		
		// String pointer is after the node structure
		char* string = reinterpret_cast<char*>(node) + sizeof(string_table_node);

		// Add to the tree
		bool success = insert(node);
		assert(success, "Duplicate string pointer in string_table. How is this possible???");
		if (!success)
		{
			free(node);
			return nullptr;
		}

		// Return allocated buffer
		return string;
	}

	void string_table::clear_usage()
	{
		// get root
		auto r = reinterpret_cast<string_table_node*>(root());
		if (r == nullptr)
			return;

		// clear recursively
		r->clear_usage();
	}

	void string_table::mark_used(const char* string)
	{
		auto node = reinterpret_cast<string_table_node*>(find(string));
		if (node == nullptr)
			return; // assert??

		// set used flag
		node->used = true;
	}

	void string_table::gc()
	{
		// start at the start
		auto iter = first();

		while (iter != nullptr)
		{
			// If it's not used, delete and continue
			if (!reinterpret_cast<string_table_node*>(iter)->used)
			{
				iter = deleteInline(iter);
				continue;
			}

			// next
			next(iter);
		}
	}

}*/