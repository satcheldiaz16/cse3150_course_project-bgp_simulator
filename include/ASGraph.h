#pragma once

#include <string>
#include <unordered_map>
#include <iostream>

#include "ASNode.h"

class ASGraph{
	uint32_t size_; //estimate size prior to parsing data, when done confirm the size and release any unused memory, or perhaps check the back of the file for the last asn
	//std::vector<ASNode> as_nodes_;
	std::unordered_map<uint32_t, ASNode> as_nodes_;
	void tokenize_line(const std::string& line, std::vector<std::string>& vec);
	void get_or_build_node(ASNode* noder_ptr, uint32_t& asn);
	void try_modify_node_relationship(ASNode* prv, ASNode* cus, bool& money_involved);
public:
	ASGraph(){}
	ASGraph(uint32_t size) {
		as_nodes_.reserve(size);
		size_ = size;
	}
	ASGraph(const ASGraph& other) = delete;
	ASGraph& operator=(const ASGraph& other) = delete;
	//using unordered_map without moving it means I can safely work with raw pointers without worrying
	//about managing them, thus don't even allow movement
	//I was capping my ass off here
	ASGraph(ASGraph&& other) = default;
	ASGraph& operator=(ASGraph&& other) = default;

	/*
	ASNode& go_to_asn const (const uint32_t& asn){
		return as_nodes_[asn]&;
	}
	*/
	ASNode* getNode(uint32_t asn){
		auto it = as_nodes_.find(asn);
		//in this case, iterator is pointing to a std::pair, this is why we are returning it->second
		return (it != as_nodes_.end()) ? &it->second : nullptr;
	}
	~ASGraph() {}
	void insert_node_at(ASNode node, const uint32_t& idx){
		as_nodes_[idx] = std::move(node);
	}
	uint32_t& size() {
		return size_;
	}

	int build_graph(const std::string& filepath);
	
	friend std::ostream& operator<<(std::ostream& os, const ASGraph& graph) {
		os << "Graph\n";
		for(auto& pair : graph.as_nodes_){
			os << "ASN= " << pair.first << ", Node: " << pair.second << "\n";
		}
		return os;
	}
};
