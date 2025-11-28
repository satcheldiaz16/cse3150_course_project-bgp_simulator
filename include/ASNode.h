#pragma once

#include <iostream>
#include <vector>
#include <algorithm>

class ASNode{
	//local rib
	std::vector<ASNode*> providers_;
	std::vector<ASNode*> customers_;
	std::vector<ASNode*> peers_;
	uint32_t asn_;
    uint32_t inverse_in_degree_;
	void try_add_node(ASNode*& node_ptr, std::vector<ASNode*>& relationships){
		if(std::find(relationships.begin(), relationships.end(), node_ptr) == relationships.end()){
			relationships.push_back(node_ptr);
		}
	}
public:
	ASNode() {}
	ASNode(uint32_t asn) {asn_ = asn;}
	ASNode(const ASNode& other) = delete;
	ASNode& operator=(const ASNode& other) = delete;
	ASNode(ASNode&& other) = default;
	ASNode& operator=(ASNode&& other) = default;
	~ASNode(){}
	uint32_t& asn(){ return asn_; }
	void try_add_prv(ASNode*& prv){
		try_add_node(prv, providers_);
	}
	void try_add_cus(ASNode*& cus){
		try_add_node(cus, customers_);
	}	
	void try_add_peer(ASNode*& peer){
		try_add_node(peer, peers_);
	}
    size_t num_customers() const{
        return customers_.size();
    }
    uint32_t in_degree(){
        return num_customers() - inverse_in_degree_;
    }
    void process_customer(){
        inverse_in_degree_++;
    }
    std::vector<ASNode*>& providers() {return providers_;} 
	friend std::ostream& operator<<(std::ostream& os, const ASNode& node){
		os << "ASN of " << node.asn_;
		os << "\nProviders: \n\t";
		for (const auto& var : node.providers_){
			os << var->asn() << ", ";
		}	
		os << "\nCustomers: \n\t";
		for (const auto& var : node.customers_){
			os << var->asn() << ", ";
		}
		os << "\nPeers: \n\t";
		for (const auto& var : node.peers_){
			os << var->asn() << ",";
		}
		os << "\n";
		return os;
	}
};
