#pragma once

#include <memory>
#include "Announcement.h"
#include "Relationship.h"

class RecievedAnnouncement{
public:
    std::shared_ptr<Announcement> announcement;
    Relationship relationship;
    RecievedAnnouncement() = default;
    RecievedAnnouncement(std::shared_ptr<Announcement> a, Relationship r) : announcement(a), relationship(r) {}
    bool operator<(const RecievedAnnouncement& other) const{
        if(relationship != other.relationship) { return relationship < other.relationship; }

        size_t this_path_length = announcement->path_size;
        size_t other_path_length = other.announcement->path_size;
        if (this_path_length != other_path_length) { return this_path_length < other_path_length; }

        // Tiebreaker: compare the announcing peer (sender), which is the last AS in the path
        uint32_t this_sender = announcement->path[announcement->path.size()-1];
        uint32_t other_sender = other.announcement->path[other.announcement->path.size()-1];
        return this_sender < other_sender;
    }
    ~RecievedAnnouncement() = default;
};
