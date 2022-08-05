#include "gdwg/graph.hpp"

#include <catch2/catch.hpp>
#include <vector>

/*
Given other graph, compare it with this* and return true if they are same,
else, return false.
*/
TEST_CASE("Comparisons"){
	auto g1 = gdwg::graph<int, std::string>{1, 3, 2, 5, 4};
	auto g2 = gdwg::graph<int, std::string>{1, 3, 2, 5, 4};

	// As they are exactly the same, it will return true
	CHECK(g1 == g2);

	g1.insert_edge(1,2,"a");
	g2.insert_edge(1,2,"a");

	// As they are exactly the same, it will return true
	CHECK(g1 == g2);

	g1.insert_edge(1, 4, "bb");

	// As g1 has one more edge, it will not be the same so,
	// false
	CHECK(!(g1 == g2));
}