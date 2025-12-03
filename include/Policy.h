#pragma once

//#include <unordered_map>
#include <memory>
#include <vector>

class ASNode;//#include "ASNode.h"
class Announcement;//#include "Announcement.h"
enum class Relationship;

class Policy{
    //std::unordered_map<uint32_t, std::unique_ptr<Announcement>> local_rib_;
public:
    virtual void recieve_announcement(Announcement* ann, Relationship r) = 0;
    virtual void process_announcements(ASNode* host) = 0;
    virtual void send_announcements(std::vector<ASNode*> recipients, Relationship r) = 0;
    virtual ~Policy() {}
};
