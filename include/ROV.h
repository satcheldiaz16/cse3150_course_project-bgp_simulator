#pragma once

#include <memory>
#include "BGP.h"

class ROV : public BGP {
public:
    void recieve_announcement(std::shared_ptr<Announcement> ann, Relationship r) final override;
};
