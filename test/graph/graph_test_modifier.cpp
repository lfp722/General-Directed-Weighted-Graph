#include "gdwg/graph.hpp"

#include <catch2/catch.hpp>
#include <vector>

/*
Given a node, insert it to the graph
If given node is already present, return false
If successfully inserted, return true
*/
TEST_CASE("insert_node test") {
	auto g = gdwg::graph<int, std::string>{1, 2, 3, 4, 5};

	// As the following nodes are already present in the graph,
	// it will return false
	CHECK(!g.insert_node(1));
	CHECK(!g.insert_node(2));
	CHECK(!g.insert_node(3));
	CHECK(!g.insert_node(4));
	CHECK(!g.insert_node(5));

	// As the following nodes are not present,
	// it will return true, successully inserted.
	CHECK(g.insert_node(6));
	CHECK(g.insert_node(7));

	// As we inserted these nodes,
	// it is_node will return true
	CHECK(g.is_node(6));
	CHECK(g.is_node(7));
}

/*
Given an src, dst and weight, insert it as an edge to the graph
If that edge is already present, return false
If successfully inserted, return true
Throws: Runtime error when either of src or dst are not present in the graph
*/
TEST_CASE("insert_edge test") {
	auto g = gdwg::graph<int, std::string>{1, 2, 3, 4, 5};

	// As we are inserting edges to the fresh graph with no edges,
	// these will return true
	CHECK(g.insert_edge(1, 2, "hello"));
	CHECK(g.insert_edge(3, 4, "world"));

	// As we already inserted edge with weight "hello" from 1 to 2,
	// inserting that edge once again will return false
	CHECK(!g.insert_edge(1, 2, "hello"));
	CHECK(!g.insert_edge(3, 4, "world"));

	// As the following nodes are not present in the graph,
	// it will throw runtime error
	CHECK_THROWS(g.insert_edge(6, 7, "Error"));
	CHECK_THROWS(g.insert_edge(7, 8, "NOO"));
}

/*
Replace old_data with new_data
Do nothing if new_data already exist, in this case, return false
If old_data is not present, it will throw runtime error.
*/
TEST_CASE("replace_node test") {
	auto g = gdwg::graph<int, std::string>{1, 2, 3, 4, 5};

	g.insert_edge(1, 2, "a");
	g.insert_edge(1, 3, "b");

	// As there are already new data exist, it will return false
	CHECK(!g.replace_node(1, 4));

	// As the following old nodes are present, it will return true
	CHECK(g.replace_node(1, 7));



	// Now 1 is replace by 7, there will be edge between 7 to 1 and 7 to 3
	CHECK(g.weights(7, 2) == std::vector<std::string>{"a"});
	CHECK(g.weights(7, 3) == std::vector<std::string>{"b"});

	// As the following old nodes are not present, it will throw runtime error
	CHECK_THROWS(g.replace_node(10, 1));
}

/*
Same as replace_node but old_data and new_data both
already exist on graph
If either of is_node(old_data) or is_node(new_data) return false,
throw runtime_error
*/
TEST_CASE("merge_replace_node test") {
	auto g = gdwg::graph<int, std::string>{1, 2, 3, 4, 5};

	g.insert_edge(1, 2, "a");
	g.insert_edge(1, 3, "b");

	// Replace node 1 by 4
	g.merge_replace_node(1, 4);

	// Replace node 3 by 5
	g.merge_replace_node(3, 5);

	// As we merge_replaced 1 by 4,
	// there will be edge between 4 and 2
	CHECK(g.weights(4, 2) == std::vector<std::string>{"a"});

	// As we merge_replaced 3 by 5,
	// there will be edge between 4 and 5
	CHECK(g.weights(4, 5) == std::vector<std::string>{"b"});

	// As the following nodes are not present, it will throw runtime error
	CHECK_THROWS(g.merge_replace_node(10, 1)); //   is_node(old)) = false
	CHECK_THROWS(g.merge_replace_node(4, 10)); //    is_node(new)) = false
	CHECK_THROWS(g.merge_replace_node(100, 101)); //  is_node(old) and is_node(new) = false
}

/*
Given a node, erase all the edges related to that node and itself
Return true if successful,
if not, return false
*/
TEST_CASE("Erase node"){
	auto g = gdwg::graph<int, std::string>{1, 2, 3, 4, 5};

	// erase node 1
	CHECK(g.erase_node(1));

	// Should return false as node 1 was removed
	CHECK(!g.is_node(1));
}

/*
Erase edge representing src->dst with weight weight
Return true if successful, false if not
throw runtime error if either is_node(src) or is_node(dst) is false
Time Complexity : O(log(n)+e)
*/
TEST_CASE("erase_edge"){
	auto g = gdwg::graph<int, std::string>{1, 2, 3, 4, 5};

	g.insert_edge(1, 2, "a");

	// As such edge exist, it will return true
	CHECK(g.erase_edge(1, 2, "a"));

	// As such edge not exist, it will return false
	CHECK(!g.erase_edge(1, 2, "b"));

	// As the following nodes are not present, it will throw runtime error
	CHECK_THROWS(g.erase_edge(6, 1, "a")); //   is_node(src)) = false
	CHECK_THROWS(g.erase_edge(1, 10, "a")); //    is_node(dst)) = false
	CHECK_THROWS(g.erase_edge(100, 101, "a")); //  is_node(src) and is_node(dst) = false
}

/*
Erase all nodes from the graph
Graph should be empty after
*/
TEST_CASE("clear"){
	auto g = gdwg::graph<int, std::string>{1, 2, 3, 4, 5};
	g.clear();
	CHECK(g.empty());
}