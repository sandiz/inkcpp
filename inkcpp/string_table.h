#pragma once

#include "tree.h"
#include "value.h"

namespace ink::runtime::internal
{
	/*// hash tree sorted by string pointers
	class string_table : public base_hash_tree<const char*>
	{
	public:
		// Creates a new dynamic string of a particular length
		char* create(size_t length);

		// zeroes all usage values
		void clear_usage();

		// marks a string as used
		void mark_used(const char* string);

		// deletes all unused strings
		void gc();
	};*/
}