#pragma once

#include <string>

vector<int> arena_as_path;

class Announcement {
public:
	uint32_t prefix_id; //can store our prefixes in a hashmap elsewhere
	//24 bytes
	std::string prefix; // 1.2.0.0/16 or some IPv6 prefix
	uint32_t start_index;
	uint32_t end_index;
	//24 bytes
	std::vector<uint32_t> as_path;
	//1 byte
	short relationship; // 0 for customers, 1 for peers, 2 for providers, 3 for origin
	//4 bytes
	uint32_t next_hop_asn;

	//size of this will be large
	// 53 - 56 bytes
	// all of these things will be moved to an 8 byte boundary
};
