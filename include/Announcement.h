#pragma once

#include <string>

#include "ASNode.h"

enum class Relationship{
    ORIGIN,
    CUSTOMER,
    PEER,
    PROVIDER
};

struct RecievedAnnouncement{
    Announcement* announcement;
    Relationship relationship;
};

class Announcement {
public:
	uint32_t prefix_id; //can store our prefixes in a hashmap elsewhere
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
    Announcement(std::string& pref, ASNode* ho, bool rov_inv){
        prefix = pref;
        host = ho;
        relationship = Relationship::ORIGIN;
        //next_hop_asn = host->asn();
        rov_invalid = rov_inv;
    }
    Announcement(const Announcement& other, Relationship r, ASNode* ho = nullptr){
        prefix = other.prefix;
        host = ho;
        relationship = r;
        prev = *other;
        //next_hop_asn = prev->host->asn();
        rov_invalid = other.rov_invalid;
    }
    Announcement& operator=(const Announcement& other) = delete;
    Announcement(Announcement&& other) = delete;
    Announcement& operator=(Announcement&& other) = delete;
    uint32_t next_hop_asn(){
        if(prev == nullptr) return host->asn();

        return prev->host->asn();
    }
    //implement later
    void path(){
        return prev->path();
    }
};
