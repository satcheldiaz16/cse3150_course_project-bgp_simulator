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
    recieved_queue_[ann->prefix].push_back(RecievedAnnouncement(ann, r));
}
void BGP::seed_announcement(const std::string& prefix, ASNode* host, bool rov_invalid){
    local_rib_.insert({
            prefix,
            std::make_unique<Announcement>(
                    prefix,
                    host,
                    rov_invalid
            )
    });
}
void BGP::process_announcements(ASNode* host){
    for(auto& pair : recieved_queue_){
        std::vector<RecievedAnnouncement>& bucket = pair.second;
        while(bucket.size() > 1){
            bucket[0] < bucket[1] ? bucket.erase(bucket.begin()+1) : bucket.erase(bucket.begin());
        }

        local_rib_.insert({
            pair.first,
            std::make_unique<Announcement>(
                *pair.second[0].announcement,
                pair.second[0].relationship,
                host,
                pair.second[0].announcement
            )
        }); 
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
const std::unordered_map<std::string, std::unique_ptr<Announcement>>& BGP::get_rib() const {
    return local_rib_;
}
