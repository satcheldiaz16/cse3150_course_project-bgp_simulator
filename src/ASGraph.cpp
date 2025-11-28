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
		vec.push_back(token);
	}
}

//returns 0 if node is found, returns 1 if node is created
int ASGraph::get_or_build_node(ASNode*& node_ptr, uint32_t& asn){	
    int temp = 0;
	if(node_ptr==nullptr || node_ptr->asn() != asn){
		//check for existence, if not there create new
		auto iter = as_nodes_.find(asn);
		if(iter != as_nodes_.end()) { node_ptr = &(iter->second); }
		else{
			as_nodes_[asn] = ASNode(asn);
			node_ptr = &as_nodes_[asn];
            temp = 1;
		}	
	}
    return temp;
}

void ASGraph::try_modify_node_relationship(ASNode*& prv, ASNode*& cus, bool& money_involved){
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
		std::cerr << "Error: Unable to open file at " << filepath << std::endl;
		return 1;
	}

	std::string cur_line;
	//0 = left as, 1 = right as, 2 = relationship, 3 = policy
	std::vector<std::string> tokens;
	tokens.reserve(4);
    
    uint32_t nodes_created = 0;

	uint32_t left_asn = 0;
	ASNode* left_node = nullptr;
	uint32_t right_asn = 0;
	ASNode* right_node = nullptr;
	bool money_involved = false; // money involved = customer/provider, not = peers

	while(std::getline(file, cur_line)){
		//std::getline(file, cur_line);
		if (cur_line.empty() || cur_line[0] == '#') {continue;}
		
		tokens.clear();
		tokenize_line(cur_line, tokens);
		
		left_asn = static_cast<uint32_t>(std::stoi(tokens[0]));
		nodes_created += get_or_build_node(left_node, left_asn);

		right_asn = static_cast<uint32_t>(std::stoi(tokens[1]));
		nodes_created += get_or_build_node(right_node, right_asn);

		money_involved = (std::stoi(tokens[2]) == -1);

		try_modify_node_relationship(left_node, right_node, money_involved);
		
		//eventually will probably need to do something with token[3]
	}

    size_ = nodes_created;

    //set rank 0 nodes
    for(auto& pair : as_nodes_){
        if(pair.second.num_customers() == 0){
            flattened_[0].push_back(&pair.second);
        }
    }

    uint32_t nodes_processed = 0;
    for(int rank = 0; rank < flattened_.size(); rank++){
        for(int i = 0; i < flattened_[rank].size(); i++){
            for(auto& prv : flattened_[rank][i]->providers()){
                prv->process_customer();
                if(prv->in_degree() == 0){
                    flattened_[rank+1].push_back(prv);
                }
            }
            nodes_processed++;
        }
    }

    if(nodes_processed < nodes_created){
        return 1;
    }

	return 0;
}
