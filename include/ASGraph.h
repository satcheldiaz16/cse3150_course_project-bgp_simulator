#pragma once

#include <string>
#include <unordered_map>
#include <iostream>
#include <memory>
#include<vector>

#include "ASNode.h"

class ASGraph{
    std::vector<std::vector<ASNode*>> flattened_;
	std::unordered_map<uint32_t, std::unique_ptr<ASNode>> as_nodes_;
	uint32_t size_; //estimate size prior to parsing data, when done confirm the size and release any unused memory, or perhaps check the back of the file for the last asn
	//std::vector<ASNode> as_nodes_;
	void tokenize_line(const std::string& line, std::vector<std::string>& vec);
	ASNode& get_or_build_node(uint32_t asn, uint32_t& nodes_created);
	void try_modify_node_relationship(ASNode& prv, ASNode& cus, bool& money_involved);
public:
	ASGraph(){
        flattened_.push_back(std::vector<ASNode*>());
        size_ = 80000;
    }
	ASGraph(uint32_t size) {
		as_nodes_.reserve(size);
        flattened_.push_back(std::vector<ASNode*>());
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
	ASNode& get_node(const uint32_t& asn){
        //in this case, iterator is pointing to a std::pair, this is why we are returning it->second
		return *as_nodes_[asn];
	}
	~ASGraph() {}
	void insert_node_at(uint32_t asn){
		as_nodes_.emplace(asn, std::make_unique<ASNode>(asn));
	}
	uint32_t& size() {
		return size_;
	}

	int build_graph(const std::string& filepath);
	
	friend std::ostream& operator<<(std::ostream& os, const ASGraph& graph) {
		os << "Graph\n";
		for(auto& pair : graph.as_nodes_){
			os << "ASN= " << pair.first << ", Node: " << *pair.second << "\n";
		}
		return os;
	}
};
