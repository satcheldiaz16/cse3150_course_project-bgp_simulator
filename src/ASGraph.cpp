#include <fstream>
#include <string>
#include <stdexcept>
#include <string>
#include <vector>
#include <sstream>
#include <cstdint>
#include <algorithm>
#include <cctype> //for isdigit()
#include <unordered_set>

#include "ASGraph.h"

//splits input into array of [cur_node, other_node, relationship] for easier usage
void ASGraph::tokenize_line(const std::string& line, std::vector<std::string>& vec){
	std::string token;
	std::stringstream ss(line);
	for(int i = 0; i < 3; i++){
		std::getline(ss, token, '|');
		vec.push_back(token);
	}
}

//gets or builds node with ASN asn
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

//adds to either provider/customer relations or peer->peer relations
void ASGraph::try_modify_node_relationship(ASNode& prv, ASNode& cus, bool& money_involved){
	if(money_involved){
		prv.try_add_cus(&cus);
		cus.try_add_prv(&prv);	
	}
	else{
		prv.try_add_peer(&cus);
		cus.try_add_peer(&prv);
	}
}

const std::string input_clique_prefix = "# input clique:";

void ASGraph::build_input_clique(const std::string& cur_line, uint32_t& nodes_created){
    std::cout << "we are in here" << std::endl;
    //std::string token;
    std::string line = cur_line.substr(input_clique_prefix.size());
    std::istringstream iss(line);
    uint32_t asn;
    while(iss >> asn){
        //if(!isdigit(token[0])) {continue;}
        //asn = std::stoi(token);
        ASNode& as = get_or_build_node(asn, nodes_created);
        flattened_[0].push_back(&as);
    } 
}

void ASGraph::flatten_bottom_up(uint32_t& nodes_processed){ 
    std::cout << "flattening bottom-up" << std::endl;
    //set rank 0 nodes
    //ref to unique ptr in this case, kind of gross but whatever
    //
    //iterate through until
    for(auto& pair : as_nodes_){
        if(pair.second->num_customers() == 0){
            flattened_[0].push_back(&(*pair.second));
        }
    }

    //build out rank 0 first, then i can iterate all the way through one rank and move on to the next
    for(int rank = 0; rank < flattened_.size(); rank++){
        
        for(int node = 0; node < flattened_[rank].size(); node++){
       
            //ASNode& node = get_node(flattened_[rank][i]);
       
            for(ASNode* prv_node : flattened_[rank][node]->providers()){
       
               // ASNode& prv_node = get_node(prv);
       
                prv_node->decrement_in_degree();
       
                if(prv_node->in_degree() == 0){
       
                    if(flattened_.size() == rank+1) {
       
                        flattened_.push_back(std::vector<ASNode*>());
                    }
       
                    flattened_[rank+1].push_back(prv_node);
                }
            }
       
            nodes_processed++;
        }
    }
}

void ASGraph::flatten_top_down(uint32_t &nodes_processed) {
    std::cout << "flattening top-down" << std::endl;

    // 1. Identify all ASes with NO providers → rank 0
    std::vector<ASNode*> queue;

    for (auto &pair : as_nodes_) {
        ASNode *node = pair.second.get();
        node->remaining_degree_ = node->num_providers(); // incoming edges = providers

        if (node->remaining_degree_ == 0) {
            flattened_[0].push_back(node);
            queue.push_back(node);
        }
    }

    if (queue.empty()) {
        std::cerr << "No root ASes (provider-less). Graph invalid or has cycle.\n";
        return;
    }

    // 2. BFS downward through customers
    size_t rank = 0;

    while (!queue.empty()) {
        std::vector<ASNode*> next_queue;

        for (ASNode *node : queue) {

            // consume this node
            nodes_processed++;

            // for each outgoing edge (node → customer)
            for (ASNode *cus : node->customers()) {

                // decrement their provider dependency count
                cus->remaining_degree_--;

                // if all providers have been processed, assign rank and enqueue
                if (cus->remaining_degree_ == 0) {
                    if (flattened_.size() <= rank + 1)
                        flattened_.push_back({});
                    flattened_[rank + 1].push_back(cus);
                    next_queue.push_back(cus);
                }
            }
        }

        queue = std::move(next_queue);
        rank++;
    }

    std::cout << "Top-down ranks: " << flattened_.size() << std::endl;

    std::reverse(flattened_.begin(), flattened_.end());
}
/*
void ASGraph::flatten_top_down(uint32_t& nodes_processed){
    std::cout << "flattening top-down" << std::endl;
    
    if (flattened_[0].empty()) return;

    for(int rank = 0; rank < flattened_.size(); rank++){
        
        for(int node = 0; node < flattened_[rank].size(); node++){
       
            //ASNode& node = get_node(flattened_[rank][i]);
       
            for(ASNode* cus_node : flattened_[rank][node]->customers()){
       
               // ASNode& prv_node = get_node(prv);
       
                cus_node->decrement_in_degree();
       
                //false denotes flattening top down
                if(cus_node->in_degree(false) == 0){
       
                    if(flattened_.size() == rank+1) {
       
                        flattened_.push_back(std::vector<ASNode*>());
                    }
       
                    flattened_[rank+1].push_back(cus_node);
                }
            }
       
            nodes_processed++;
        }
    }

    //reverse flattened_ since flattening top down
    std::reverse(flattened_.begin(), flattened_.end());
}
*/
void ASGraph::seed_announcement(uint32_t asn, std::string& prefix, bool rov_invalid){
    ASNode* node = &get_node(asn);
    
    node->policy()->seed_announcement(prefix, node, rov_invalid);
}

int ASGraph::build_rov_invalid_list(const std::string& filepath){
    std::fstream file(filepath, std::ios::in);
    
    if(!file.is_open()){
        std::cerr << "Error: Unable to open rov invalid file" << std::endl;
        return 1;
    }

    std::string cur_line;

    while(std::getline(file, cur_line)){
        rov_invalid_.push_back(std::stoi(cur_line));
    }     

    return 0;
}

int ASGraph::build_graph(const std::string& filepath){
	std::fstream file(filepath, std::ios::in);

	if(!file.is_open()){
		std::cerr << "Error: Unable to open file at " << filepath << std::endl;
		return 1;
	}

	std::string cur_line;
	//0 = left as, 1 = right as, 2 = relationship, only reserving 3 cause policy is unused
	std::vector<std::string> tokens;
	tokens.reserve(3);
    
    uint32_t nodes_created = 0;

	uint32_t left_asn = 0;
	//ASNode& left_node;
	uint32_t right_asn = 0;
	//ASNode& right_node;
	bool money_involved = false; // money involved = customer/provider, not = peers

	while(std::getline(file, cur_line)){
        
		if (cur_line.empty() || cur_line[0] == '#') {      
	        // input clique scenario
            if(cur_line.rfind(input_clique_prefix, 0) == 0){
                build_input_clique(cur_line, nodes_created);
            }
            
            continue;
        }
		
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
    
    uint32_t nodes_processed = 0;
    
    //if input clique was obtained, flatten top down, else bottom up
    flattened_[0].size() > 0 ? flatten_top_down(nodes_processed) : flatten_bottom_up(nodes_processed);

    std::cout << "num ranks = " << flattened_.size() << std::endl;

    //if there is a cycle error out
    if(nodes_processed < nodes_created){
        return 1;
    }

	return 0;
}

int ASGraph::seed_announcements(const std::string& filepath){
    std::ifstream file(filepath);

    if(!file.is_open()){
        std::cerr << "Error reading seeded announcements" << std::endl;
        return 1;
    }
    
    std::string line;
    
    std::getline(file, line);

    while(std::getline(file, line)){
        std::stringstream ss(line);
        std::string cell;
        std::vector<std::string> row_data;

        while(std::getline(ss, cell, ',')){
            row_data.push_back(cell);
        }
        
        bool b;
        std::istringstream(row_data[2]) >> std::boolalpha >> b;

        seed_announcement(std::stoi(row_data[0]), row_data[1], b);
    }

    return 0;
}

int ASGraph::propogate_announcements(){
    for(auto rank = flattened_.begin(); rank != flattened_.end(); rank++){
       
        for(auto node = rank->begin(); node != rank->end(); node++){
            (*node)->policy()->process_announcements(*node);    
        }
        for(auto node = rank->begin(); node != rank->end(); node++){
            (*node)->announce_up();    
        }
    }
    for(auto rank = flattened_.begin(); rank != flattened_.end(); rank++){
        
        for(auto node = rank->begin(); node != rank->end(); node++){
            (*node)->policy()->process_announcements(*node);    
        }
        for(auto node = rank->begin(); node != rank->end(); node++){
            (*node)->announce_across();
        }
    }
    for(auto rank = flattened_.rbegin(); rank != flattened_.rend(); rank++){
        
        for(auto node = rank->rbegin(); node != rank->rend(); node++){
            (*node)->policy()->process_announcements(*node);    
        }
        for(auto node = rank->rbegin(); node != rank->rend(); node++){
            (*node)->announce_down();
        }
    }
    return 0;
}

int ASGraph::output_graph(const std::string& filepath){
    std::ofstream output_file(filepath);

    if(!output_file.is_open()){
        std::cerr << "Couldn't open output file" << std::endl;
        return 1;
    }

    output_file << "asn,prefix,as_path\n";


    for(auto& node_pair : as_nodes_){
        for(auto& rib_entry : node_pair.second->policy()->get_rib()){
            output_file << node_pair.first << ","
                << rib_entry.first << ",\""
                << rib_entry.second->format_path() << "\"\n";
        }
    }

    output_file.close();

    return 0;
}
