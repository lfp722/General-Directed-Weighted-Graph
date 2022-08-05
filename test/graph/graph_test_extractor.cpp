#include "gdwg/graph.hpp"

#include <catch2/catch.hpp>
#include <vector>
#include <string>
#include <iostream>
#include <sstream>

/*
Effects: Behaves as a formatted output function of os
Return os
*/
TEST_CASE("Extractor"){
	auto const v = std::vector<gdwg::graph<int, int>::value_type>{
		   {4, 1, -4},
		   {3, 6, -8},
		   {3, 2, 2},
		   {2, 4, 2},
		   {2, 1, 1},
		   {6, 3, 10},
		   {6, 2, 5},
		   {1, 5, -1},
		   {4, 5, 3},
		   {5, 2, 7},
		};

		auto g = gdwg::graph<int, int>{1, 2, 3, 4, 5, 6, 64};

		for (const auto& [from, to, weight] : v) {
		  g.insert_node(from);
		  g.insert_node(to);
		  g.insert_edge(from, to, weight);
		}


		g.insert_node(64);
		auto out = std::ostringstream{};
		out << g;
		auto const expected_output = std::string_view(R"(1 (
  5 | -1
)
2 (
  1 | 1
  4 | 2
)
3 (
  2 | 2
  6 | -8
)
4 (
  1 | -4
  5 | 3
)
5 (
  2 | 7
)
6 (
  2 | 5
  3 | 10
)
64 (
)
)");
		CHECK(out.str() == expected_output);

		// Also check for const
		auto const g_const = g;
		out.str("");
		out << g_const;
		CHECK(out.str() == expected_output);
}