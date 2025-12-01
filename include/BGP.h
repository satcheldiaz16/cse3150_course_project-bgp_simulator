#pragma once

#include <unordered_map>
#include <string>
#include <vector>
#include <memory>

#include "Announcement.h"
#include "Policy.h"

class BGP : public Policy{
    std::unordered_map<std::string, std::unique_ptr<Announcement>> local_rib_;
    std::unordered_map<std::string, Announcement*> recieved_queue_;
public:
    void recieve_announcement(Announcement* ann, Relationship r) override {
        recieved_queue[ann->prefix] = {ann, r};
    }
    void process_announcements(ASNode* host) final override {
        for(auto& pair : recieved_queue_){
            bool conflicts = false; //implement this later
            
            if(!conflicts){
                local_rib.insert({
                    pair.first,
                    std::make_unique<Announcement>(
                        *pair.second.announcement,
                        pair.second.relationship,
                        host
                    )
                }); 
            }
        }
    }
    void send_announcements(std::vector<ASNode*> recipients, Relationship r) final override {
       for(auto& pair : local_rib_){
           for(ASNode* recipient : recipients){
               recipient->policy()->recieve_announcement(*pair.second, r);
           }
       }
    }
};
