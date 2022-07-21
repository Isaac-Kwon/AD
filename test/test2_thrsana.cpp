#include "iostream"
#include "cstdlib"

#include "ADLoader.hpp"
#include "AD.hpp"
#include "vector"
#include "math.h"
#include "TH2D.h"
#include "TCanvas.h"

struct pixresult{
    float threshold;
    float noise;
};

pixresult fastana(int* data, int nd, bool verbose=false){
    float sum1 = 0;
    float sum2 = 0;

    for(int i=1; i<nd; i++){
        sum1 += (data[i]-data[i-1])*(i-0.5);
        sum2 += (data[i]-data[i-1])*(i-0.5)*(i-0.5);

        if(verbose) std::cout<<data[i-1]<<" ";
    }
    if(verbose) std::cout<<std::endl;

    pixresult res;
    res.threshold = sum1;
    res.noise     = sqrt(sum2-sum1*sum1);

    return res;
}


int test2_thrsana(){
    // ADLoader * loader = new ADLoader("../data//08_15T_B1/Vbb/thrscan/thrscan-20220603_000716.raw");

    // ADLoader * loader = new ADLoader("../data/08_15T_B1/woVbb/thrscan/thrscan-20220602_233759.raw");

    ADLoader * loader = new ADLoader("../data/komac_220628/thrscan-20220628_024610.raw");

    int ntrg  = 50 ;
    int dvmin = 0  ;
    int dvmax = 30 ;

    float threshold[512][1024];
    float noise[512][1024];

    std::vector<AD::Tuple> dead;
    std::vector<AD::Tuple> bad;

    int i=0;
    int warningCNT = 0;
    for(int iy = 0; iy<512; iy++){
        std::cout<<"IY="<<iy<<"/512"<<std::endl;
        int * rowhits = (int*) std::malloc( (dvmax-dvmin+1) * 1024 * sizeof(int) );

        for(int ii=0; ii<(dvmax-dvmin+1) * 1024; ii++){
            rowhits[ii] = 0;
        }

        for(int dv = 0; dv<(dvmax-dvmin+1); dv++){

            for(int itrg=0; itrg<ntrg; itrg++){

                AD::Event evt = loader->DecodeOneEvent(i);

                if(evt.code != AD::OK){
                    std::cout<<"EVT NOK at line" << i <<" with error "<< evt.code <<std::endl;
                    return 1;
                }
                // std::cout<<evt<<std::endl;
                i = evt.end;

                for( AD::Tuple pix: evt.hits){
                    if( pix.y != iy ){
                        std::cerr<<"warning: hit from bad row: hit=("
                                 <<pix.x<<","
                                 <<pix.y<<")"
                                 <<" row=" << iy
                                 <<" dv="<<dv<<" at line "<<i<< std::endl;
                        std::cerr << evt <<std::endl;
                        // std::cerr << "with fired pixels" << std::endl;
                        // for(AD::Tuple pix: evt.hits){
                        //     std::cerr<<"("<<pix.x<<","<<pix.y<<") ";
                        // }
                        std::cerr<<std::endl;
                        warningCNT ++;
                        // if(warningCNT > 1) return 1;
                    }else{
                        rowhits[pix.x*(dvmax-dvmin+1) + (dv-dvmin)] += 1;
                    }

                }
            }
        }

        for(int ix=0; ix<1024; ix++){
            pixresult res = fastana(
                &(rowhits[ix*(dvmax-dvmin+1)]), (dvmax-dvmin+1),
                ix==256);
            threshold[iy][ix] = res.threshold;
            noise[iy][ix]     = res.noise;
        }

        std::free(rowhits);
    }

    TH2D * hthres = new TH2D("hthres","Threshold Map", 1024, -0.5, 1023.5, 512, -0.5, 511.5);
    TH2D * hnoise = new TH2D("hnoise","Noise Map", 1024, -0.5, 1023.5, 512, -0.5, 511.5);

    for(int ix=0; ix<1024; ix++){
        for(int iy=0; iy<512; iy++){
            hthres->SetBinContent(ix,iy,threshold[iy][ix]);
            hnoise->SetBinContent(ix,iy,noise[iy][ix]);
        }
    }

    TCanvas *c1 = new TCanvas("c1", "c1", 1800, 3600);
    c1->Draw();
    c1->Divide(1,2);
    c1->cd(1);
    hthres->Draw("COLZ");
    c1->cd(2);
    hnoise->Draw("COLZ");

    c1->SaveAs("./test2_c1.png");

    
    std::free(threshold);
    std::free(noise);

    return 0;
}

int main(){
    test2_thrsana();
}