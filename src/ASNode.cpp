#include <iostream>
#include <vector>
#include <algorithm>

#include "ASNode.h"
#include "ROV.h"
#include "BGP.h"
#include "Policy.h"
#include "Announcement.h"
void ASNode::try_add_node(ASNode* asn, std::vector<ASNode*>& relationships){
     if(std::find(relationships.begin(), relationships.end(), asn) == relationships.end()){
         relationships.push_back(asn);
     }
}
void ASNode::announce(std::vector<ASNode*>& listeners, Relationship r){
//    policy_->process_announcements(this);

    policy_->send_announcements(listeners, r);
}
ASNode::ASNode(uint32_t asn, bool use_rov) {
    asn_ = asn;
    policy_ = use_rov ? std::make_unique<ROV>() : std::make_unique<BGP>();
    inverse_in_degree_ = 0;
}
uint32_t& ASNode::asn(){ return asn_; }
void ASNode::try_add_prv(ASNode* prv){
    try_add_node(prv, providers_);
}
void ASNode::try_add_cus(ASNode* cus){
    try_add_node(cus, customers_);
}	
void ASNode::try_add_peer(ASNode* peer){
    try_add_node(peer, peers_);
}
size_t ASNode::num_customers() const{
    return customers_.size();
} 
size_t ASNode::num_providers() const{
    return providers_.size();
}
uint32_t ASNode::in_degree(bool bottom_up){
    return (bottom_up ? num_customers() : num_providers()) - inverse_in_degree_;
}
void ASNode::decrement_in_degree(){
    inverse_in_degree_++;
}
std::vector<ASNode*>& ASNode::providers() {return providers_;} 
std::vector<ASNode*>& ASNode::customers() {return customers_;}
Policy* ASNode::policy() { return policy_.get(); }
void ASNode::announce_up(){
    announce(providers_, Relationship::CUSTOMER);
}
void ASNode::announce_across(){
    announce(peers_, Relationship::PEER);
}
void ASNode::announce_down(){
    announce(customers_, Relationship::PROVIDER);
}
std::ostream& operator<<(std::ostream& os, const ASNode& node){
    os << "ASN of " << node.asn_;
    os << "\nProviders: \n\t";
    for (const auto& var : node.providers_){
        os << var << ", ";
    }	
    os << "\nCustomers: \n\t";
    for (const auto& var : node.customers_){
        os << var << ", ";
    }
    os << "\nPeers: \n\t";
    for (const auto& var : node.peers_){
        os << var << ",";
    }
    os << "\n";
    return os;
}
