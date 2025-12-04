#pragma once

#include <unordered_map>
#include <memory>
#include <vector>
#include <string>

class ASNode;//#include "ASNode.h"
class Announcement;//#include "Announcement.h"
enum class Relationship;

class Policy{
    //std::unordered_map<uint32_t, std::shared_ptr<Announcement>> local_rib_;
public:
    virtual void recieve_announcement(std::shared_ptr<Announcement> ann, Relationship r) = 0;
    virtual void seed_announcement(const std::string& prefix, ASNode* host, bool rov_invalid) = 0;
    virtual void process_announcements(ASNode* host) = 0;
    virtual void send_announcements(std::vector<ASNode*> recipients, Relationship r) = 0;
    virtual const std::unordered_map<std::string, std::shared_ptr<Announcement>>& get_rib() const = 0;
    virtual ~Policy() {}
};
