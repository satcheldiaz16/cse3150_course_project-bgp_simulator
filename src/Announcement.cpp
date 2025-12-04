#include <string>
#include <vector>
#include <iostream>

#include "Announcement.h"
#include "ASNode.h"
#include "Relationship.h"
Announcement::Announcement(const std::string& pref, ASNode* ho, bool rov_inv){
    prefix = pref;
    host = ho;
    relationship = Relationship::ORIGIN;
    rov_invalid = rov_inv;
    path.push_back(ho->asn());
}
Announcement::Announcement(const Announcement& other, Relationship r, ASNode* ho){
    prefix = other.prefix;
    path_size = other.path_size + 1;
    host = ho;
    relationship = r;
    path = other.path;
    path.push_back(host->asn());
//    prev = p;
//    next_hop_as = next_hop;
    rov_invalid = other.rov_invalid;
}
uint32_t Announcement::next_hop_asn() const{
    return path[path.size() == 1 ? 0 : path.size()-2];
}
/*
std::string Announcement::path() const{
    if(prev == nullptr) return std::to_string(host->asn());
    return std::to_string(host->asn()) + ", " + prev->path(); 
}

std::vector<uint32_t> Announcement::path() const{
    std::vector<uint32_t> vec = prev == nullptr ? std::vector<uint32_t>() : prev->path();
    
    vec.push_back(host->asn());
    
    return std::move(vec);
}
*/
const std::string Announcement::format_path(){
    std::string str_path = "(";

    for(int i = path.size()-1; i >= 0; i--){
        str_path += std::to_string(path[i]);
       // str_path += ",";

        if (i != 0){
            str_path += ", ";
        }
        if(path.size() == 1){
            str_path+=",";
        }
    }

    str_path += ")";


    return std::move(str_path);
}
bool Announcement::operator<(const Announcement& other) const{
    if(relationship != other.relationship) { return relationship < other.relationship; }

    size_t this_path_length = path_size;
    size_t other_path_length = other.path_size;
    if (this_path_length != other_path_length) { return this_path_length < other_path_length; }

    // Tiebreaker: compare the announcing peer (sender), which is the second-to-last AS in the path
    // (last AS is always the current node for announcements in the RIB)
    uint32_t this_sender = path.size() == 1 ? path[0] : path[path.size()-2];
    uint32_t other_sender = other.path.size() == 1 ? other.path[0] : other.path[other.path.size()-2];
    return this_sender < other_sender;
}
