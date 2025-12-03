#include <unordered_map>
#include <string>
#include <vector>
#include <memory>

#include "BGP.h"
#include "Announcement.h"
#include "RecievedAnnouncement.h"
#include "Policy.h"
#include "Relationship.h"
#include "ASNode.h"

void BGP::recieve_announcement(Announcement* ann, Relationship r){
    recieved_queue_[ann->prefix] = RecievedAnnouncement(ann, r);
}
void BGP::process_announcements(ASNode* host){
    for(auto& pair : recieved_queue_){
        bool conflicts = false; //implement this later
        
        if(!conflicts){
            local_rib_.insert({
                pair.first,
                std::make_unique<Announcement>(
                    *pair.second.announcement,
                    pair.second.relationship,
                    host,
                    pair.second.announcement
                )
            }); 
        }
    }

    recieved_queue_.clear();
}
void BGP::send_announcements(std::vector<ASNode*> recipients, Relationship r){
   for(auto& pair : local_rib_){
       for(ASNode* recipient : recipients){
           recipient->policy()->recieve_announcement(pair.second.get(), r);
       }
   }
}
