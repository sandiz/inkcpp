#pragma once

#include <vector>
#include <string>
#include <map>

namespace ink::compiler
{
	typedef std::vector<std::string> error_list;

	// stores results from the compilation process
	struct compilation_results
	{
		error_list warnings;
		error_list errors;

		std::vector<std::string> _all_knots;
		std::vector<std::string> _all_stitches;
		std::vector<std::string> _all_globals;
		std::map<std::string, std::vector<std::string>> _knotlines;
	};
}