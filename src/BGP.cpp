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

void BGP::recieve_announcement(std::shared_ptr<Announcement> ann, Relationship r){
    recieved_queue_[ann->prefix].push_back(RecievedAnnouncement(ann, r));
}
void BGP::seed_announcement(const std::string& prefix, ASNode* host, bool rov_invalid){
    local_rib_.insert({
            prefix,
            std::make_shared<Announcement>(
                    prefix,
                    host,
                    rov_invalid
            )
    });
}
void BGP::process_announcements(ASNode* host){
    for(auto& pair : recieved_queue_){
        std::vector<RecievedAnnouncement>& bucket = pair.second;
        auto best = bucket.begin();
        for(auto it = bucket.begin() + 1; it != bucket.end(); ++it){
            if(*it < *best){  // if it is better than best
                   best = it;
            }
       }
           // Keep only the best
       RecievedAnnouncement best_ann = *best;
       bucket.clear();
       bucket.push_back(best_ann);
        while(bucket.size() > 1){
            bucket[0] < bucket[1] ? bucket.erase(bucket.begin()+1) : bucket.erase(bucket.begin());
        }

        //now compare against announcement in RIB

        // Create the new announcement first
        auto new_ann = std::make_shared<Announcement>(
            *pair.second[0].announcement,
            pair.second[0].relationship,
            host
        );

        bool is_new = local_rib_.find(pair.first) == local_rib_.end();
        bool is_better = !is_new && (*new_ann < *local_rib_[pair.first]);
        if(is_new || is_better){
            local_rib_[pair.first] = new_ann;
    }
    }
    recieved_queue_.clear();
}

void BGP::send_announcements(std::vector<ASNode*> recipients, Relationship r){
   for(auto& pair : local_rib_){
       for(ASNode* recipient : recipients){
           recipient->policy()->recieve_announcement(pair.second, r);
       }
   }
}
const std::unordered_map<std::string, std::shared_ptr<Announcement>>& BGP::get_rib() const {
    return local_rib_;
}
