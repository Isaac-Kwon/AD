#ifndef __AD__
#define __AD__

#include "stdint.h"
#include "deque"

namespace AD{

    typedef union BYTE1{
    uint8_t value;
    uint8_t bytes[1];
    }Byte1;

    typedef union BYTE2{
    uint16_t value;
    uint8_t bytes[2];
    }Byte2;

    typedef union BYTE4{
        uint32_t value;
        uint8_t bytes[4];
    }Byte4;

    typedef union BYTE8{
        uint64_t value;
        uint8_t bytes[8];
    }Byte8;

    struct Tuple{
        uint16_t x;
        uint16_t y;
        Tuple(uint16_t x_, uint16_t y_): x(x_), y(y_){;}
    };

    enum EVENTCODE { OK, 
                    MPT, EMPTY_ERROR,
                    ERR_0, ERR_1, ERR_2, ERR_3, ERR_4, ERR_5,
                    UNDONE } ;

    struct Event{
        EVENTCODE         code;
        union {
            uint32_t value;
            uint8_t bytes[4];
        } val;
        uint8_t           reg   = 0;
        uint64_t          tev   = 0;
        uint32_t          iev   = 0;
        std::deque<Tuple> hits;
        uint64_t          start = -1;
        uint64_t          end   = -1;
    };
};

std::ostream& operator<<(std::ostream& os, const AD::EVENTCODE& dt);
std::ostream& operator<<(std::ostream& os, const AD::Event& evt);

#endif