#include <memory>
#include "BGP.h"
#include "ROV.h"
void ROV::recieve_announcement(std::shared_ptr<Announcement> ann, Relationship r){
    if(ann->rov_invalid) { return; }

    BGP::recieve_announcement(ann, r);
}
