#include <vector>
#include <algorithm>

class ASNode{
	//local rib
	std::vector<ASNode*> providers_;
	std::vector<ASNode*> customers_;
	std::vector<ASNode*> peers_;
	uint32_t asn_;
	void try_add_node(ASNode* node_ptr, std::Vector<ASNode*>& relationships){
		if(std::find(relationships.begin(), relationships.end(), node_ptr) != relationship.end()){
			relationship.push_back(node_ptr);
		}
	}
public:
	ASNode() {}
	ASNode(uint32_t asn) {asn_ = asn;}
	ASNode(const ASNode& other) = delete;
	ASNode& operator=(const ASNode& other) = delete;
	ASNode(ASNode&& other) = delete;
	ASNode& operator=(ASNode&& other) = delete;
	~ASNode(){}
	uint32_t& asn const(){ return asn&; }
	void try_add_prv(ASNode* prv){
		try_add_node(prv, providers_);
	}
	void try_add_cus(ASNode* cus){
		try_add_node(cus, customers_);
	}	
	void try_add_peer(ASNode* peer){
		try_add_node(peer, peers_);
	}
	ostream& operator<<(ostream& os){
		os << "ASN of " << asn_;
		os << "\nProviders: \n\t";
		for (const auto& var : providers_){
			os << var->asn() << ", ";
		}	
		os << "\nCustomers: \n\t";
		for (const auto& var : customers_){
			os << var->asn() << ", ";
		}
		os << "\nPeers: \n\t";
		for (const auto& var : peers_){
			os << var->asn() << ",";
		}
		os << "\n";
	}
}
