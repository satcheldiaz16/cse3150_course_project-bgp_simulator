#include <string>
#include <vector>

#include "Announcement.h"
#include "ASNode.h"
#include "Relationship.h"
Announcement::Announcement(std::string& pref, ASNode* ho, bool rov_inv){
    prefix = pref;
    host = ho;
    relationship = Relationship::ORIGIN;
    rov_invalid = rov_inv;
}
Announcement::Announcement(const Announcement& other, Relationship r, ASNode* ho, Announcement* p){
    prefix = other.prefix;
    host = ho;
    relationship = r;
    prev = p;
    //next_hop_asn = prev->host->asn();
    rov_invalid = other.rov_invalid;
}
uint32_t Announcement::next_hop_asn(){
    if(prev == nullptr) return host->asn();

    return prev->host->asn();
}
//implement later
std::vector<uint32_t> Announcement::path(){
    std::vector<uint32_t> vec = prev == nullptr ? std::vector<uint32_t>() : prev->path();
    
    vec.push_back(ho->asn());
    
    return std::move(vec);
}

const std::string& Announcement::format_path(){
    std::string path = "(";
    std::vector<uint32_t> path = path();

    for(int i = 0; i < path.size(); i++){
        path += std::to_string(path[i]);
        
        if (i != path.size()-1){
            path += ", ";
        }
    }

    path += ")";

    return path;
}
