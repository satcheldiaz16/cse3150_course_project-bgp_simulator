#pragma once

#include "Announcement.h"
#include "Relationship.h"

class RecievedAnnouncement{
public:
    Announcement* announcement;
    Relationship relationship; 
    RecievedAnnouncement(){}
    RecievedAnnouncement(Announcement* a, Relationship r) : announcement(a), relationship(r) {}; 
    bool operator<(const RecievedAnnouncement& other) const{
        if(relationship != other.relationship) { return relationship < other.relationship; }

        size_t this_path_length = announcement->path().size();
        size_t other_path_length = announcement->path().size();
        if (this_path_length != other_path_length) { return this_path_length < other_path_length; }

        return announcement->next_hop_asn() < other.announcement->next_hop_asn();
    }
    ~RecievedAnnouncement() = default;
};
