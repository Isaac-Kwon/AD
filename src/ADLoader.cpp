#include "ADLoader.hpp"

#include "stdint.h"

#include "ios"
#include "iostream"
#include "fstream"

#include "AD.hpp"


using namespace AD;

ADLoader::ADLoader(){

}

ADLoader::ADLoader(const char * fname){
    LoadFile(fname);

}

void ADLoader::LoadFile(const char * fname){
    if(fData){
        Unload();
    }

    fFilename = fname;

    std::ifstream file;
    file.open(fname, std::ios::in | std::ios::binary);

    uint64_t begin = file.tellg();
    file.seekg (0, std::ios::end);
    uint64_t end = file.tellg();
    file.seekg (0, std::ios::beg);
    fDSize = end-begin;

    fData = (char *) malloc(fDSize*sizeof(char));
    file.read(fData, fDSize);
    std::cout<<"File size\t"<<fDSize<<std::endl;

}

void ADLoader::Unload(){
    if(! fData ){
        std::cerr << "ADLoader::Unload - No file to unload " << fFilename <<std::endl; 
    }
}

Byte1 ADLoader::Get8(const uint64_t& i){
    Byte1 a;
    for(uint64_t j=0; j<1; j++) a.bytes[j] = fData[i+j]; 
    return a;
}

Byte2 ADLoader::Get16(const uint64_t& i){
    Byte2 a;
    for(uint64_t j=0; j<2; j++) a.bytes[j] = fData[i+j]; 
    return a;
}

Byte4 ADLoader::Get32(const uint64_t& i){
    Byte4 a;
    for(uint64_t j=0; j<4; j++) a.bytes[j] = fData[i+j]; 
    return a;
}

Byte8 ADLoader::Get64(const uint64_t& i){
    Byte8 a;
    for(uint64_t j=0; j<8; j++) a.bytes[j] = fData[i+j]; 
    return a;
}

Event ADLoader::EventDone(Event evt, uint64_t i, uint8_t reg){
    if(! (Get32(i).value==0xBBBBBBBB)){
        std::cout<<"ADLoader::EventDone - ERR - "<<std::hex<<Get64(i).value<<" is not 0xBBBBBBBB"<<std::dec<<std::endl;
        evt.reg  = reg;
        evt.code = ERR_5;
        evt.end  = i;
        return evt;
    }
    evt.reg  = reg;
    evt.code = OK;
    evt.end  = i+4;

    if(evt.hits.size() > 0) SaveEventBucket(evt);

    return evt;
}

Event ADLoader::DecodeOneEvent(const uint64_t& i_){

    Event evt;
    uint64_t i=i_;
    evt.start = i;
    evt.code=UNDONE;

    if( Get32(i).value != 0xAAAAAAAA){
        evt.code = ERR_0;
        evt.end  = i;

        return evt;
    }

    evt.tev = Get64(i+8).value;
    evt.iev = Get32(i+4).value;

    i += 16; // 4+8+4 = 16

    //////////////////////////////////////////////

    uint8_t reg = 0;
    
    if( (Get8(i).value & 0xF0) == 0xE0){
        if(Get8(i+2).value != 0xFF){evt.code=ERR_1; return evt;}
        if(Get8(i+3).value != 0xFF){evt.code=ERR_2; return evt;}
        i+=4;
        return EventDone(evt, i, reg);
    }else if( (Get8(i).value & 0xF0) == 0xA0 ){
        i+=2;
        while(i<fDSize){
            uint8_t data0 = Get8(i).value;
            if((data0&0xC0)==0x00) {// data long
                uint32_t d=reg<<14|(data0&0x3F)<<8|Get8(i+1).value;
                uint16_t x=d>>9&0x3FE|(d^d>>1)&0x1;
                uint16_t y=d>>1&0x1FF;
                evt.hits.emplace_back(Tuple(x,y));
                uint8_t data2=Get8(i+2).value;
                d+=1;
                while(data2) {
                    if(data2&1) {
                        x=d>>9&0x3FE|(d^d>>1)&0x1;
                        y=d>>1&0x1FF;
                        evt.hits.emplace_back(Tuple(x,y));
                    }
                    data2>>=1;
                    d+=1;
                }
                i+=3;
            } else if((data0&0xC0)==0x40) {// data short
                uint32_t d=reg<<14|(data0&0x3F)<<8|Get8(i+1).value;
                uint16_t x=d>>9&0x3FE|(d^d>>1)&0x1;
                uint16_t y=d>>1&0x1FF;
                // evt.hits.emplace_back(Tuple(x,y));
                i+=2;
            } else if((data0&0xE0)==0xC0) {// region header
                reg=data0&0x1F;
                i+=1;
            } else if((data0&0xF0)==0xB0) {// chip trailer
                i+=1;
                i=(i+3)/4*4;
                return EventDone(evt, i, reg);
                //   goto evtdone;
            } else if(data0==0xFF) {// IDLE (why?)
                i+=1;
            } else {
                std::cout<<"ERR_3\t"<<std::hex<<((uint32_t)data0)<<std::endl;
                evt.code = ERR_3;
                evt.end  = i;
                return evt;
            }
        }
    }else{
        evt.code = ERR_4; //value error
        evt.end  = i;
        std::cout<<"ERR_4\t c="<<i<<"\t"<<std::hex<< (uint64_t) Get8(i).value <<std::dec<<std::endl;
        std::cout<< std::hex<< (Get8(i).value & 0xF0)
                 << std::dec<<" (0xA0 or 0xE0)" <<std::endl;
        return evt;
    }
    

    // std::cout<<"EVTDONE1"<<std::endl;
    // return EventDone(evt, i);
    return evt;
    
}

