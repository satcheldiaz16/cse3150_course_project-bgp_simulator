#pragma once

#include <unordered_map>
#include <string>
#include <vector>
#include <memory>

#include "Announcement.h"
#include "RecievedAnnouncement.h"
#include "Policy.h"
class ASNode;//#include "ASNode.h"

class BGP : public Policy{
    std::unordered_map<std::string, std::unique_ptr<Announcement>> local_rib_;
    std::unordered_map<std::string, std::vector<RecievedAnnouncement>> recieved_queue_;
public:
    virtual void recieve_announcement(Announcement* ann, Relationship r) override;
    void process_announcements(ASNode* host) final override;        
    void send_announcements(std::vector<ASNode*> recipients, Relationship r) final override;
    const std::unordered_map<std::string, std::unique_ptr<Announcement>>& get_rib() const final override;
};
