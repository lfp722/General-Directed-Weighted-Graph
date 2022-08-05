#include "gdwg/graph.hpp"

#include <catch2/catch.hpp>
#include <vector>
#include <string>
#include <iostream>
#include <sstream>

/*
Given a node, if it exist in graph, return true
else return false.
*/
TEST_CASE("is_node") {
	auto g = gdwg::graph<int, std::string>{1, 2, 3, 4, 5};

	// As the following nodes are present, should return true
	CHECK(g.is_node(1));
	CHECK(g.is_node(2));
	CHECK(g.is_node(3));
	CHECK(g.is_node(4));
	CHECK(g.is_node(5));

	// As the following nodes are not present, should return false
	CHECK(!g.is_node(6));
	CHECK(!g.is_node(7));
	CHECK(!g.is_node(8));
}

/*
Check if graph is empty
if it is, return true
else, return false
*/
TEST_CASE("empty test") {
	auto g = gdwg::graph<int, std::string>{1, 2, 3, 4, 5};

	// As the graph g is not empty, should return false
	CHECK(!g.empty());

	g.clear();

	// As we cleared graph g, should return true
	CHECK(g.empty());
}

/*
Given two nodes, if there is an edge between them, return true
else, return false
Throw runtime error if either of is_node(src) or is_node(dst) are false
*/
TEST_CASE("is_connected test") {
	auto g = gdwg::graph<int, std::string>{1, 2, 3, 4, 5};

	g.insert_edge(1, 2, "a");
	g.insert_edge(2, 3, "b");
	g.insert_edge(3, 4, "c");

	// As the following nodes are connected to each other,
	// should return true
	CHECK(g.is_connected(1, 2));
	CHECK(g.is_connected(2, 3));
	CHECK(g.is_connected(3, 4));

	// As the following nodes are not connected to each other,
	// should return false
	CHECK(!g.is_connected(3, 5));
	CHECK(!g.is_connected(5, 1));
	CHECK(!g.is_connected(5, 2));

	// As the following nodes are not present, it will throw runtime error
	CHECK_THROWS(g.is_connected(6, 1)); //   is_node(src)) = false
	CHECK_THROWS(g.is_connected(1, 10)); //    is_node(dst)) = false
	CHECK_THROWS(g.is_connected(100, 101)); //  is_node(src) and is_node(dst) = false
}

/*
Return a sequence of nodes in ascending order.
*/
TEST_CASE("nodes") {
	auto g = gdwg::graph<int, std::string>{1, 3, 2, 5, 4};
	auto v = std::vector<int>{1,2,3,4,5};

	// As we have nodes in ascending order, should return true
	CHECK(g.nodes() == v);

	// Check for self as well
	CHECK(g.nodes() == g.nodes());
}

/*
Return a sequence of edges from src to dst in ascending order
Throw runtime error if either of is_node(src) or is_node(dst) are false
*/
TEST_CASE("weights function test") {
	auto g = gdwg::graph<int, std::string>{1, 3, 2, 5, 4};

	g.insert_edge(1, 3, "a");
	g.insert_edge(1, 3, "aa");
	g.insert_edge(1, 4, "b");
	g.insert_edge(2, 4, "c");

	CHECK(g.weights(1, 3) == std::vector<std::string>{"a", "aa"});
	CHECK(g.weights(1, 4) == std::vector<std::string>{"b"});
	CHECK(g.weights(2, 4) == std::vector<std::string>{"c"});

	// As the following nodes are not present, it will throw runtime error
	CHECK_THROWS(g.weights(6, 1)); //   is_node(src)) = false
	CHECK_THROWS(g.weights(1, 10)); //    is_node(dst)) = false
	CHECK_THROWS(g.weights(100, 101)); //  is_node(src) and is_node(dst) = false
}


/*
Return sequence of nodes in ascending order connected to src
Throw runtime error if either of is_node(src) is false
*/
TEST_CASE("connections") {
	auto g = gdwg::graph<int, std::string>{1, 3, 2, 5, 4};

	g.insert_edge(1, 3, "a");
	g.insert_edge(1, 3, "aa");
	g.insert_edge(1, 4, "b");
	g.insert_edge(2, 4, "c");

	// As node 1 has 2 incoming edge from 3 and 1 incoming edge from 4
	// it will have 3,3,4
	CHECK(g.connections(1) == std::vector<int>{3, 3, 4});

	// As node 2 has 1 incoming edge from node 4,
	// it will only have 4
	CHECK(g.connections(2) == std::vector<int>{4});


	// As the following src nodes are not present, it will throw runtime error
	CHECK_THROWS(g.connections(6)); //   is_node(src)) = false
}
