#pragma once

#include <string>
#include <unordered_map>
#include <iostream>

#include "ASNode.h"

class ASGraph{
	uint32_t size_; //estimate size prior to parsing data, when done confirm the size and release any unused memory, or perhaps check the back of the file for the last asn
	//std::vector<ASNode> as_nodes_;
	std::unordered_map<uint32_t, ASNode> as_nodes;
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
	ASGraph(ASGraph&& other) = delete;
	ASGraph& operator=(ASGraph&& other) = delete;

	/*
	ASNode& go_to_asn const (const uint32_t& asn){
		return as_nodes_[asn]&;
	}
	*/
	ASNode* getNode const(uint32_t asn){
		auto it = nodes.find(asn);
		//in this case, iterator is pointing to a std::pair, this is why we are returning it->second
		return (it != nodes.end()) ? &it->second : nullptr;
	}
	~ASGraph() {}
	void insert_node_at(ASNode node, const uint32_t& idx){
		as_nodes_[idx] = std::move(node);
	}
	uint32_t& size const(){
		return size_&;
	}

	void build_graph(const std::string& filepath);
	
	ostream& operator<<(ostream& os){
		os << "Graph\n";
		for(const auto& pair : as_nodes){
			os << "ASN= " << pair.first << ", Node: " << pair.second << "\n";
		}
	}
}
