#ifndef GDWG_GRAPH_HPP
#define GDWG_GRAPH_HPP

#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <set>
#include <stdexcept>
#include <string>
#include <vector>

// This will not compile straight away
namespace gdwg {
	template<typename N, typename E>
	class graph {
	public:
		struct value_type {
			N from;
			N to;
			E weight;
		};

		// Constructors
		/*
		Basic Constructor doing value initialization
		*/
		graph() noexcept
		: graph_{std::map<std::unique_ptr<N>, std::set<Edges>>()} {}

		/*
		Given an list of nodes, craete a graph based on that.
		As only nodes are given, there will be no connected edges in this stage
		https://edstem.org/au/courses/8629/discussion/935118
		*/
		graph(std::initializer_list<N> il){
			for(auto const& i : il){
				insert_node(i);
			}
		}

		/*
		Initialises the graphs node collection with the range [first, last)
		*/
		template<typename InputIt>
		graph(InputIt first, InputIt last) {
			std::for_each(first, last, [this](N const& i) { insert_node(i); });
		}

		// Move Constructor
		graph(graph&& other) noexcept
		: graph_{std::exchange(other.graph_,
		                       std::map<std::unique_ptr<N>, std::set<Edges>>())} {}

		// Move Assignment
		auto operator=(graph&& other) noexcept -> graph& {
			std::swap(graph_, other.graph_);
			//other.empty() is true so we will clear other graph
			other.clear();
			return *this;
		}

		// Copy Constructor
		graph(graph const& other) {
			for (auto const& i : other.graph_) {
				graph_.emplace(std::make_unique<N>(*(i.first)), i.second);
			}
		}

		// Copy Assignment
		auto operator=(graph const& other) -> graph& {
			auto clone = graph(other);
			std::swap(clone, *this);
			// As we used clone of other graph,
			// we will clear clone graph once the copy was done.
			clone.clear();
			return *this;
		}

		// Modifiers
		/*
		Given a node, insert it to the graph
		If that given node exist, return false
		*/
		auto insert_node(N const& value) -> bool {
			// get_node returns the node inside the graph
			auto const& value_node = get_node(value);

			// If that node does not exist, it will be equal to the graph pointing
			// to the last element
			if (value_node == graph_.end()) {
				graph_.emplace(std::make_unique<N>(value), std::set<Edges>());
				return true;
			}

			return false;
		}

		/*
		Given a src node, dst node and a weight,
		insert an edge between src and dst given a weight
		If either of is_node(src) or is_node(dst) return false,
		it will throw runtime error.
		*/
		auto insert_edge(N const& src, N const& dst, E const& weight) -> bool {
			if (!is_node(src) || !is_node(dst)){
				auto error_msg = "Cannot call gdwg::graph<N, E>::insert_edge when either src or dst node does not exist";
				throw std::runtime_error(error_msg);
			}

			auto const& src_node = get_node(src);
			auto const& dst_node = get_node(dst);
			auto const& edges = src_node -> second;

			for (auto const& i : edges){
				// If the given edge already exist, return false
				if (*(i.weight) == weight && *(i.dst) == dst) {
					return false;
				}
			}
			auto e1 = Edges();
			e1.dst = dst_node -> first.get();
			e1.weight = std::make_unique<E>(weight);
			src_node -> second.insert(std::move(e1));
			return true;
		}

		/*
		Replace old_data with new_data
		Do nothing if new_data already exist
		If is_node(old_data) return false,
		it will throw runtime error.
		*/
		auto replace_node(N const& old_data, N const& new_data) -> bool {
			if(!is_node(old_data)) {
				auto error_msg = "Cannot call gdwg::graph<N, E>::replace_node on a node that doesn't exist";
				throw std::runtime_error(error_msg);
			}

			// new_data already exist
			if (is_node(new_data)){
				return false;
			}

			insert_node(new_data);

			// Now we need to implement edges related to old_data to new_data
			for (auto const& i : graph_){
				// Case 1: src = old_data
				if(old_data == *(i.first)){
					for (auto const& j : i.second){
						// Case 2: edge was directed to itself
						if (old_data == *(j.dst)){
							insert_edge(new_data, new_data, *(j.weight));
						}
						// Case 3: edge was not directed to itself
						else{
							insert_edge(new_data, *(j.dst), *(j.weight));
						}

					}
				}
				// Case 4: dst = old_data
				else{
					for (auto const& j : i.second){
						if (*(j.dst) == old_data){
							insert_edge(*(i.first), new_data, *(j.weight));
						}
					}
				}
			}
			erase_node(old_data);
			return true;
		}

		/*
		Same as replace_node but old_data and new_data both
		already exist on graph
		If either of is_node(old_data) or is_node(new_data) return false,
		throw runtime_error
		*/
		auto merge_replace_node(N const& old_data, N const& new_data) -> void {
			if (!is_node(old_data) || !is_node(new_data)){
				auto error_msg = "Cannot call gdwg::graph<N, E>::merge_replace_node on old or new data if they don't exist in the graph";
				throw std::runtime_error(error_msg);
			}
			// Now we need to implement edges related to old_data to new_data
			for (auto const& i : graph_){
				// Case 1: src = old_data
				if(old_data == *(i.first)){
					for (auto const& j : i.second){
						// Case 2: edge was directed to itself
						if (old_data == *(j.dst)){
							insert_edge(new_data, new_data, *(j.weight));
						}
						// Case 3: edge was not directed to itself
						else{
							insert_edge(new_data, *(j.dst), *(j.weight));
						}

					}
				}
				// Case 4: dst = old_data
				else{
					for (auto const& j : i.second){
						if (*(j.dst) == old_data){
							insert_edge(*(i.first), new_data, *(j.weight));
						}
					}
				}
			}
			erase_node(old_data);

		}

		/*
		Given a node, erase all the edges related to that node and itself
		*/
		auto erase_node(N const& value) -> bool {
			if(!is_node(value)){
				return false;
			}
			for (auto& i : graph_) {
				std::erase_if(i.second, [value](auto const& x) {return *(x.dst) == value;});
			}
			auto const& value_node = get_node(value);
			graph_.erase(value_node);
			return true;
		}

		/*
		Erase edge representing src->dst with weight weight
		throw runtime error if either is_node(src) or is_node(dst) is false
		Time Complexity : O(log(n)+e)
		*/
		auto erase_edge(N const& src, N const& dst, E const& weight) -> bool {
			if (!is_node(src) || !is_node(dst)) {
				auto error_msg = "Cannot call gdwg::graph<N, E>::erase_edge on src or dst if they don't exist in the graph";
				throw std::runtime_error(error_msg);
			}
			auto i = get_node(src);
			auto const& e =
			   std::find_if(i->second.begin(), i->second.end(), [dst, weight](Edges const& i) {
				   return *(i.dst) == dst && *(i.weight) == weight;
			   });
			if (e == i->second.end()) {
				return false;
			}
			i->second.erase(e);
			return true;
		}

		// Erase all nodes from the graph
		auto clear() noexcept -> void {
			graph_.clear();
		}

		// Accessors
		/*
		Given a node, if it exist in graph, return true
		else return false.
		*/
		[[nodiscard]] auto is_node(N const& value) const -> bool {
			for (auto const& i : graph_){
				if (*(i.first) == value){
					return true;
				}
			}
			return false;
		}

		/*
		Check if graph is empty
		if it is, return true
		else, return false
		*/
		[[nodiscard]] auto empty() const -> bool {
			return graph_.empty();
		}

		/*
		Given two nodes, if there is an edge between them, return true
		else, return false
		Throw runtime error if either of is_node(src) or is_node(dst) are false
		*/
		[[nodiscard]] auto is_connected(N const& src, N const& dst) const -> bool {
			if (!is_node(src) || !is_node(dst)) {
				auto error_msg = "Cannot call gdwg::graph<N, E>::is_connected if src or dst node don't exist in the graph";
				throw std::runtime_error(error_msg);
			}
			for (auto const& i : graph_){
				if(*(i.first) == src){
					for(auto const& j : i.second){
						if(*(j.dst) == dst){
							return true;
						}
					}
				}
			}
			return false;
		}

		/*
		Return a sequence of nodes in ascending order.
		*/
		[[nodiscard]] auto nodes() const -> std::vector<N> {
			auto ret = std::vector<N>{};
			for_each(graph_.begin(), graph_.end(), [&ret](auto const& i) { ret.push_back(*(i.first)); });
			std::sort(ret.begin(), ret.end());
			return ret;
		}

		/*
		Return a sequence of edges from src to dst in ascending order
		Throw runtime error if either of is_node(src) or is_node(dst) are false
		*/
		[[nodiscard]] auto weights(N const& src, N const& dst) const -> std::vector<E> {
			if (!is_node(src) || !is_node(dst)) {
				auto error_msg = "Cannot call gdwg::graph<N, E>::is_connected if src or dst node don't exist in the graph";
				throw std::runtime_error(error_msg);
			}
			auto ret = std::vector<E>{};
			auto const& src_node = get_node(src);

			for (auto const& e : src_node->second){
				if(*(e.dst) == dst){
					ret.push_back(*(e.weight));
				}
			}
			return ret;
		}

		/*
		Return sequence of nodes in ascending order connected to src
		Throw runtime error if either of is_node(src) is false
		*/
		[[nodiscard]] auto connections(N const& src) const -> std::vector<N> {
			if (!is_node(src)) {
				auto error_msg = "Cannot call gdwg::graph<N, E>::connections if src doesn't exist in the graph";
				throw std::runtime_error(error_msg);
			}
			auto ret = std::vector<N>{};
			N node;
			for (auto const& i : graph_){
				if(*(i.first) == src){
					for (auto const& j : i.second){
						node = *(j.dst);
						ret.push_back(node);
					}
				}
			}
			std::sort(ret.begin(), ret.end());
			return ret;
		}

		// Comparisons
		/*
		Given other graph, compare it with this* and return true if they are same,
		else, return false.
		*/
		[[nodiscard]] auto operator==(graph const& other) const -> bool {
			if (graph_.size() != other.graph_.size()){
				return false;
			}

			auto j = other.graph_.begin();
			for (auto const& i : graph_){
				// Check node value
				if(*(i.first) != *(j->first)){
					return false;
				}
				// Check node's edge size
				if(i.second.size() != j->second.size()){
					return false;
				}
				// Check edge
				auto j_edge = j->second.begin();
				for (auto const& i_edge: i.second){
					// Check edge dst
					if(*(i_edge.dst) != *(j_edge->dst)){
						return false;
					}
					// Check edge weight
					if(*(i_edge.weight) != *(j_edge->weight)){
						return false;
					}
					j_edge++;
				}
				j++;
			}
			return true;
		}

		// Extractor
		friend auto operator<<(std::ostream& os, graph const& g) -> std::ostream& {
			for (auto const& i : g.graph_) {
				os << *(i.first) << " ";
				os << "(" << "\n";
				for (auto const& j : i.second) {
					os << "  " << *(j.dst) << " | " << *(j.weight) << "\n";
				}
				os << ")\n";
			}
			return os;
		}



	private:
		struct Edges {
			N* dst;
			std::unique_ptr<E> weight;

			// Edge Initialization
			Edges() {}

			// Given edge, create an edge corresponds to that
			Edges(Edges const& other)
			: dst{other.dst}, weight{std::make_unique<E>(*(other.weight))} {}

			// Compare two edge in constant time in order to achieve time complexity
			auto operator<(const Edges& other) const {
				if (*(dst) != *(other.dst)){
					return *(dst) < *(other.dst);
				}
				return *(weight) < *(other.weight);
			}

		};

		// graph initialization here
		std::map<std::unique_ptr<N>, std::set<Edges>> graph_;

		//helper functions to find the node
		auto get_node(N const& value) const ->
		   typename std::map<std::unique_ptr<N>, std::set<Edges>>::const_iterator {
			return std::find_if(graph_.begin(), graph_.end(), [&value](auto const& i) {
				return *(i.first) == value;
			});
		}

		//helper functions to find the node
		auto get_node(N const& value) ->
		   typename std::map<std::unique_ptr<N>, std::set<Edges>>::iterator {
			return std::find_if(graph_.begin(), graph_.end(), [&value](auto const& i) {
				return *(i.first) == value;
			});
		}


	};
}  //namespace gdgw

#endif // GDGW_GRAPH_HPP