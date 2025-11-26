#pragma once

#include <vector>

#include "ASNode.h"

class ASGraph{
	uint32_t size_; //estimate size prior to parsing data, when done confirm the size and release any unused memory, or perhaps check the back of the file for the last asn
	std::vector<ASNode> as_nodes_;
public:
	ASGraph(){}
	ASGraph(uint32_t size) {
		as_nodes_.reserve(size);
		size_ = size;
	}
	ASGraph(const ASGraph& other) = delete;
	ASGraph& operator=(const ASGraph& other) = delete;
	ASGraph(ASGraph&& other) = delete;
	ASGraph& operator=(ASGraph&& other) = delete;


	ASNode& go_to_asn const (const uint32_t& asn){
		return as_nodes_[asn]&;
	}
	~ASGraph() {}
	void insert_node_at(ASNode node, const uint32_t& idx){
		as_nodes_[idx] = std::move(ASNode);
	}
	uint32_t& size(){
		return size_&;
	}
}
