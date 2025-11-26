#pragma once

#include <vector>

#include "ASNode.h"

class ASGraph{
	std::vector<ASNode> as_nodes_;
public:
	ASNode& go_to_asn(uint32_t asn){
		return as_nodes_[asn]&;
	}
	void insert_node_at(ASNode node, uint32_t idx){
		as_nodes_[idx] = std::move(ASNode);
	}
}
