#include <vector>

class ASNode{
	//local rib
	std::vector<uint32_t> providers_;
	std::vector<uint32_t> customers_;
	std::vector<uint32_t> peers_;
	uint32_t asn_;
public:
	//ASNode() {}
	uint32_t asn(){ return asn; }
	
}
