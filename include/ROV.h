#pragma once

#include "BGP.h"

class ROV : public BGP {
public:
    void recieve_announcement(Announcement* ann, Relationship r) final override;
};
