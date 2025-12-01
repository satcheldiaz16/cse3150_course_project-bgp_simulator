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

ASNode& ASGraph::get_or_build_node(uint32_t asn, uint32_t& nodes_created){	
		//check for existence, if not there create new
	auto iter = as_nodes_.find(asn);
    
    if(iter == as_nodes_.end()){
        insert_node_at(asn);
        nodes_created++;
    }

    ASNode& temp = get_node(asn);

    return temp;
}

void ASGraph::try_modify_node_relationship(ASNode& prv, ASNode& cus, bool& money_involved){
	if(money_involved){
		prv.try_add_cus(cus.asn());
		cus.try_add_prv(prv.asn());	
	}
	else{
		prv.try_add_peer(cus.asn());
		cus.try_add_peer(prv.asn());
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
	//ASNode& left_node;
	uint32_t right_asn = 0;
	//ASNode& right_node;
	bool money_involved = false; // money involved = customer/provider, not = peers

	while(std::getline(file, cur_line)){
		//std::getline(file, cur_line);
		if (cur_line.empty() || cur_line[0] == '#') {continue;}
		
		tokens.clear();
		tokenize_line(cur_line, tokens);
		
		left_asn = static_cast<uint32_t>(std::stoi(tokens[0]));
		ASNode& left_node = get_or_build_node(left_asn, nodes_created);

		right_asn = static_cast<uint32_t>(std::stoi(tokens[1]));
		ASNode& right_node = get_or_build_node(right_asn, nodes_created);

		money_involved = (std::stoi(tokens[2]) == -1);

		try_modify_node_relationship(left_node, right_node, money_involved);
		
		//eventually will probably need to do something with token[3]
	}

    size_ = nodes_created;

    //set rank 0 nodes
    //ref to unique ptr in this case, kind of gross but whatever
    //
    //iterate through until
    for(auto& pair : as_nodes_){
        if(pair.second->num_customers() == 0){
            flattened_[0].push_back(pair.second->asn());
        }
    }

    uint32_t nodes_processed = 0;
    for(int rank = 0; rank < flattened_.size(); rank++){
        for(int i = 0; i < flattened_[rank].size(); i++){
            ASNode& node = get_node(flattened_[rank][i]);
            for(uint32_t prv : node.providers()){
                ASNode& prv_node = get_node(prv);
                prv_node.process_customer();
                if(prv_node.in_degree() == 0){
                    if(flattened_.size() == rank+1) {
                        flattened_.push_back(std::vector<uint32_t>());
                    }
                    flattened_[rank+1].push_back(prv);
                }
            }
            nodes_processed++;
        }
    }

    std::cout << "num ranks = " << flattened_.size() << std::endl;

    if(nodes_processed < nodes_created){
        return 1;
    }

	return 0;
}
