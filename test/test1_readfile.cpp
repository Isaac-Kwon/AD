#include "ADLoader.hpp"
#include "iostream"
#include "AD.hpp"

int main(){
    ADLoader * l = new ADLoader("../data//08_15T_B1/Vbb/thrscan/thrscan-20220603_000716.raw");


    int tmp = 0;
    int i=0;
    while (i<30){
        AD::Event evt = l->DecodeOneEvent(tmp);
        tmp = evt.end;
        if(evt.hits.size()==0){
            continue;
        }
        std::cout<<evt<<std::endl;
        i++;
    }
    return 0;
}
