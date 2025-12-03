#pragma once

#include <iostream>
#include <vector>
#include <algorithm>
#include "Policy.h"//class Policy; //used to resolve errors
class Announcement;//#include "Announcement.h"
enum class Relationship;

class ASNode{
	std::vector<ASNode*> providers_;
	std::vector<ASNode*> customers_;
	std::vector<ASNode*> peers_;
    std::unique_ptr<Policy> policy_;
	uint32_t asn_;
    uint32_t inverse_in_degree_;
	void try_add_node(ASNode* asn, std::vector<ASNode*>& relationships);
    void announce(std::vector<ASNode*>& listeners, Relationship r);
public:
	ASNode() {}
	ASNode(uint32_t asn, bool use_rov = false);
	ASNode(const ASNode& other) = delete;
	ASNode& operator=(const ASNode& other) = delete;
	ASNode(ASNode&& other) = default;
	ASNode& operator=(ASNode&& other) = default;
	~ASNode(){}
	uint32_t& asn();
	void try_add_prv(ASNode* prv);
	void try_add_cus(ASNode* cus);
	void try_add_peer(ASNode* peer);
    size_t num_customers() const;
    size_t num_providers() const;
    uint32_t in_degree(bool bottom_up = true);
    void decrement_in_degree();
    std::vector<ASNode*>& providers();
    std::vector<ASNode*>& customers();
    Policy* policy();
    void announce_up();
    void announce_across();
    void announce_down();
	friend std::ostream& operator<<(std::ostream& os, const ASNode& node);
};
