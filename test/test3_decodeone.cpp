#include "ADLoader.hpp"
#include "iostream"
#include "AD.hpp"
#include "vector"

int main(){
    ADLoader * l = new ADLoader("../data//08_15T_B1/Vbb/thrscan/thrscan-20220603_000716.raw");

    std::vector<int> nums = {
        98941844,
    };

    for (int i : nums){
        int tmp = i;
        for(int j=0; j<1; j++){
            AD::Event evt = l->DecodeOneEvent(tmp);

            tmp = evt.end;
            if(evt.hits.size()==0){
                j--;
                continue;
            }

            // if(evt.hits.at(0).y==448) continue;

            std::cout<<evt<<std::endl;

            for(AD::Tuple tup : evt.hits) std::cout<<"("<<tup.x<<","<<tup.y<<") ";
            std::cout<<std::endl;
        }

    }
    return 0;
}
