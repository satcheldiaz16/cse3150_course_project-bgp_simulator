#pragma once

#include "Announcement.h"
#include "Relationship.h"

class RecievedAnnouncement{
public:
    Announcement* announcement;
    Relationship relationship; 
    RecievedAnnouncement(){}
    RecievedAnnouncement(Announcement* a, Relationship r) : announcement(a), relationship(r) {};
    ~RecievedAnnouncement() = default;
};
