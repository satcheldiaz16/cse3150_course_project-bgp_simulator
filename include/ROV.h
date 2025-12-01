#pragma once


#include "BGP.h"

class ROV final: public BGP{
public:
    void recieve_announcement(Announcement* ann) final override {
        if(ann.rov_invalid) { return; }

        BGP::recieve_announcement(ann);
    }
};
