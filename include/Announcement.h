#pragma once

#include <string>

//#include "ASNode.h"
class ASNode;
#include "Relationship.h"

class Announcement {
public:
	uint32_t path_size = 1; //can store our prefixes in a hashmap elsewhere
	//24 bytes
	std::string prefix; // 1.2.0.0/16 or some IPv6 prefix
	//uint32_t start_index;
	//uint32_t end_index;
    ASNode* host;
    Announcement* prev = nullptr; //nullptr denotes origin of path 
	//24 bytes
	//std::vector<uint32_t> as_path;
	//1 byte
	Relationship relationship; // 0 for origin, 1 for customer, 2 for peer, 3 for provider
	//4 bytes
	//uint32_t next_hop_asn;
    bool rov_invalid;
	//size of this will be large
	// 53 - 56 bytes
	// all of these things will be moved to an 8 byte boundary
    Announcement() = delete;
    //this should only ever be called when seeding
    Announcement(const std::string& pref, ASNode* ho, bool rov_inv);
    Announcement(const Announcement& other, Relationship r, ASNode* ho = nullptr, Announcement* p = nullptr);
    Announcement& operator=(const Announcement& other) = delete;
    Announcement(Announcement&& other) = delete;
    Announcement& operator=(Announcement&& other) = delete;
    uint32_t next_hop_asn() const;
    //implement later
    std::string path() const;
    //    std::vector<uint32_t> path() const;
    const std::string format_path();
//    bool operator<(const Announcement& other) const;
};
