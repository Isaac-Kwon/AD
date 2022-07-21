#ifndef __ADLoader__
#define __ADLoader__

#include "stdint.h"
#include "AD.hpp"
#include "string"
#include "deque"
#include "iostream"

class ADLoader{

    public:
    ADLoader();
    ADLoader(const char * fname);
    void LoadFile(const char * fname);
    void Unload();

    char      Get(const uint64_t& i){return fData[i];}
    AD::Byte1 Get8(const uint64_t& i);
    AD::Byte2 Get16(const uint64_t& i);
    AD::Byte4 Get32(const uint64_t& i);
    AD::Byte8 Get64(const uint64_t& i);

    AD::Event DecodeOneEvent(const uint64_t& i);

    void SaveEventBucket(AD::Event evt){
        fEventBucket.emplace_back(evt);
        if(fEventBucket.size() > 3) fEventBucket.pop_front();
    }

    void PrintBucket(){
        std::cout<<"Printing Buckets"<<std::endl;
        for(AD::Event evt : fEventBucket){
            std::cout<<evt<<std::endl;
            for(AD::Tuple pix : evt.hits){
                std::cout<<" ("<<pix.x<<","<<pix.y<<") ";
            }
            std::cout<<std::endl;
        }
    }
    
    protected:
    private:
    AD::Event EventDone(AD::Event evt, uint64_t i, uint8_t reg=0);
    char     *  fData;
    uint64_t    fDSize = 0;
    std::string fFilename = "";
    std::deque<AD::Event> fEventBucket;
};

#endif