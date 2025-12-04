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

        return announcement->next_hop_asn() < other.announcement->next_hop_asn();
    }
    ~RecievedAnnouncement() = default;
};
