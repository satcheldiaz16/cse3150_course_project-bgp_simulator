#include "BGP.h"
#include "ROV.h"
void ROV::recieve_announcement(Announcement* ann, Relationship r){
    if(ann->rov_invalid) { return; }

    BGP::recieve_announcement(ann, r);
}
