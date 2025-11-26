#include <vector>

class ASNode{
	//local rib
	std::vector<uint32_t> providers_;
	std::vector<uint32_t> customers_;
	std::vector<uint32_t> peers_;
	uint32_t asn_;
public:
	ASNode() {}
	ASNode(uint32_t asn) {asn_ = asn;}
	ASNode(const ASNode& other) = delete;
	ASNode& operator=(const ASNode& other) = delete;
	ASNode(ASNode&& other) = delete;
	ASNode& operator=(ASNode&& other) = delete;
	~ASNode(){}
	uint32_t& asn const(){ return asn&; }
	void add_prv(uint32_t prv){
		providers_.push_back(prv);
	}
	void add_cus(uint32_t cus){
		customers_.push_back(cus);
	}	
	void add_peer(uint32_t peer){
		peers_.push_back(peer_;
	}
}
