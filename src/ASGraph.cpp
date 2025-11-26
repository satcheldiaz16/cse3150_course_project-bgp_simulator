#include <fstream>
#include <string>
#include <stdexcept>
#include <string>
#include <vector>
#include <sstream>
#include <cstdint>
#include <algorithm>

#include "ASGraph.h"

void ASGraph::tokenize_line(const std::string& line, std::vector<std::string>& vec){
	std::string token;
	std::stringstream ss(line);
	for(int i = 0; i < 4; i++){
		std::getline(ss, token, '|');
		vec[i] = token;
	}
}

void ASGraph::get_or_build_node(ASNode* node_ptr, uint32_t& asn){	
	if(node_ptr==nullptr || node_ptr->asn() != asn){
		//check for existence, if not there create new
		auto iter = as_nodes_.find(asn);
		if(iter != as_nodes_.end()) { node_ptr = &(iter->second); }
		else{
			as_nodes_[asn] = ASNode(asn);
			node_ptr = &as_nodes_[asn];
		}	
	}
}

void ASGraph::try_modify_node_relationship(ASNode* prv, ASNode* cus, bool& money_involved){
	if(money_involved){
		prv->try_add_cus(cus);
		cus->try_add_prv(prv);	
	}
	else{
		prv->try_add_peer(cus);
		cus->try_add_peer(prv);
	}
}

int ASGraph::build_graph(const std::string& filepath){
	std::fstream file(filepath, std::ios::in);

	if(!file.is_open()){
		std::cerr << "Error: Unable to open file at" << filepath << std::endl;
		return 1;
	}

	std::string cur_line;
	//0 = left as, 1 = right as, 2 = relationship, 3 = policy
	std::vector<std::string> tokens;
	tokens.reserve(4);
	
	uint32_t left_asn = 0;
	ASNode* left_node = nullptr;
	uint32_t right_asn = 0;
	ASNode* right_node = nullptr;
	bool money_involved = false; // money involved = customer/provider, not = peers

	while(std::getline(file, cur_line)){
		//std::getline(file, cur_line);
		if (cur_line.empty() || cur_line[0] == '#') {continue;}

		tokenize_line(cur_line, tokens);
		
		left_asn = static_cast<uint32_t>(std::stoi(tokens[0]));
		get_or_build_node(left_node, left_asn);

		right_asn = static_cast<uint32_t>(std::stoi(tokens[1]));
		get_or_build_node(right_node, right_asn);

		money_involved = (std::stoi(tokens[2]) == -1);

		try_modify_node_relationship(left_node, right_node, money_involved);
		
		//eventually will probably need to do something with token[3]
	}

	return 0;
}
