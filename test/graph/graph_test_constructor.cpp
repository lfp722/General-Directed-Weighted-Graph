#include "gdwg/graph.hpp"

#include <catch2/catch.hpp>
#include <vector>


/*
Basic constructor that value initializes for all members.
Throws: Nothing
*/
TEST_CASE("Basic constructor"){
	auto g = gdwg::graph<int, std::string>{};
	// We didn't insert any edges so it should be empty
	CHECK(g.empty());
}

/*
Given a list of nodes, create a graph based on it.
Effect equivalent to graph(il.begin(), il.end());
Throws: Nothing
*/
TEST_CASE("Constructor, Given: List of Nodes"){
	SECTION("Type of node = int"){
		auto g = gdwg::graph<int, std::string>{1, 2, 3, 4, 5};
		// These nodes are in the graph so should return true
		CHECK(g.is_node(1));
		CHECK(g.is_node(2));
		CHECK(g.is_node(3));
		CHECK(g.is_node(4));
		CHECK(g.is_node(5));

		// These nodes are not in the graph so should return false
		CHECK(!g.is_node(16));
		CHECK(!g.is_node(153));
	}
	SECTION("Type of node = string"){
		auto g = gdwg::graph<std::string, std::string>{"a", "b", "c"};
		// These nodes are in the graph so should return true
		CHECK(g.is_node("a"));
		CHECK(g.is_node("b"));
		CHECK(g.is_node("c"));

		// These nodes are not in the graph so should return false
		CHECK(!g.is_node("d"));
		CHECK(!g.is_node("e"));
	}
}

/*
Given InputIt, create a graph corresponds to that
Effects: Initialises the graph?s node collection with the range [first, last)
*/

TEST_CASE("Constructor, given InputIt"){
	SECTION("Type of node = int"){
		auto v = std::vector<int>{1, 2, 3, 4, 5};
		auto g = gdwg::graph<int, std::string>(v.begin(), v.end());
		// These nodes are in the graph so should return true
		CHECK(g.is_node(1));
		CHECK(g.is_node(2));
		CHECK(g.is_node(3));
		CHECK(g.is_node(4));
		CHECK(g.is_node(5));

		// These nodes are not in the graph so should return false
		CHECK(!g.is_node(16));
		CHECK(!g.is_node(153));
	}
	SECTION("Type of node = string"){
		auto v = std::set<std::string>{"a", "b", "c"};
		auto g = gdwg::graph<std::string, int>{v.begin(), v.end()};
		// These nodes are in the graph so should return true
		CHECK(g.is_node("a"));
		CHECK(g.is_node("b"));
		CHECK(g.is_node("c"));

		// These nodes are not in the graph so should return false
		CHECK(!g.is_node("d"));
		CHECK(!g.is_node("e"));
	}
}

/*
Move constructor
Given other graph, move *this graph to that other graph
and clear given other graph
*/
TEST_CASE("Move Constructor"){
	auto g = gdwg::graph<int, std::string>{1, 2, 3, 4, 5};
	auto moved = gdwg::graph<int, std::string>(std::move(g));

	// as g was used by this* graph to move, it should be empty
	CHECK(g.empty());

	// as *this graph have all nodes from graph g, these nodes should be present
	CHECK(moved.is_node(1));
	CHECK(moved.is_node(2));
	CHECK(moved.is_node(3));
	CHECK(moved.is_node(4));
	CHECK(moved.is_node(5));

	// These nodes are not in the graph so should return false
	CHECK(!moved.is_node(16));
	CHECK(!moved.is_node(153));
}

/*
Move Assignment
Given other graph, move *this graph to that other graph
and clear given other graph
Return: *this
*/
TEST_CASE("Move Assignment"){
	auto g = gdwg::graph<int, std::string>{1, 2, 3, 4, 5};
	auto moved = std::move(g);

	// as g was used by this* graph to move, it should be empty
	CHECK(g.empty());

	// as *this graph have all nodes from graph g, these nodes should be present
	CHECK(moved.is_node(1));
	CHECK(moved.is_node(2));
	CHECK(moved.is_node(3));
	CHECK(moved.is_node(4));
	CHECK(moved.is_node(5));

	// These nodes are not in the graph so should return false
	CHECK(!moved.is_node(16));
	CHECK(!moved.is_node(153));
}

/*
Given a graph, copy that graph to *this so that
both of them have same nodes and edges
*/
TEST_CASE("Copy Constructor"){
	auto g = gdwg::graph<int, std::string>{1, 2, 3, 4, 5};
	auto copied = g;

	//Both of them should have same nodes
	CHECK(g.is_node(1));
	CHECK(copied.is_node(1));
	CHECK(g.is_node(3));
	CHECK(copied.is_node(3));

	// Now insert edge
	g.insert_edge(1, 2, "weight");
	auto copied_1 = g;

	//Both of them should have same edges
	CHECK(g.is_connected(1, 2));
	CHECK(copied_1.is_connected(1, 2));
}

/*
Given a graph, copy that graph to *this so that
both of them have same nodes and edges
Return: *this
*/
TEST_CASE("Copy Assignment"){
	auto g = gdwg::graph<int, std::string>{1, 2, 3, 4, 5};
	auto copied = gdwg::graph<int, std::string>{4, 5, 6};
	copied = g;

	//Both of them should have same nodes
	CHECK(g.is_node(1));
	CHECK(copied.is_node(1));
	CHECK(g.is_node(3));
	CHECK(copied.is_node(3));

	// Now insert edge
	g.insert_edge(1, 2, "weight");
	auto copied_1 = g;

	//Both of them should have same edges
	CHECK(g.is_connected(1, 2));
	CHECK(copied_1.is_connected(1, 2));
}