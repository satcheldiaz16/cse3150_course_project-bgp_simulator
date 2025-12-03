#include <string>

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
void Announcement::path(){
    return prev->path();
}
