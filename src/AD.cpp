#include "iostream"
#include "AD.hpp"

std::ostream& operator<<(std::ostream& os, const AD::EVENTCODE& dt)
{
    if(dt == AD::OK) os << "OK";
    if(dt == AD::MPT) os << "MPT";
    if(dt == AD::EMPTY_ERROR) os << "EMPTY_ERROR";
    if(dt == AD::ERR_0) os << "ERR_0";
    if(dt == AD::ERR_1) os << "ERR_1";
    if(dt == AD::ERR_2) os << "ERR_2";
    if(dt == AD::ERR_3) os << "ERR_3";
    if(dt == AD::ERR_4) os << "ERR_4";
    if(dt == AD::ERR_5) os << "ERR_5";
    if(dt == AD::UNDONE) os << "UNDONE";

    return os;
}

std::ostream& operator<<(std::ostream& os, const AD::Event& evt){
    
    os << evt.code;
    if(evt.code != AD::OK) return os;

    os<<"\treg="  <<evt.reg;
    os<<"\tnhit=" <<evt.hits.size();
    os<<"\tiev="  <<evt.iev<<"\ttev="<<evt.tev;
    os<<"\trng=[" <<evt.start<<","<<evt.end<<"]";

    return os;
}