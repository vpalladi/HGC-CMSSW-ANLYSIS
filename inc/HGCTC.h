#ifndef HGCTC_H
#define HGCTC_H 1

/* c/c++ */
#include <iostream>

/* mylibs */
#include "detId.h"

/* ROOT */
#include "TMath.h"

using namespace std;


const double thicknessCorrection = 1.092;

class HGCTC {

public:

    HGCTC() {};

    unsigned int _id        ;
    int          _subdet    ;
    int          _zside     ;
    int          _layer     ;
    int          _wafer     ;
    int          _wafertype ;
    int          _cell      ;
    unsigned int _data      ;
    float        _energy    ;
    float        _eta       ;
    float        _phi       ;
    float        _z         ;

    int getCorrectedLayer() { 
        if ( _subdet==3 ) return _layer;
        if ( _subdet==4 ) return ( _layer + 28 );
        if ( _subdet==5 ) return ( _layer + 40 );
        return -1;
    }

    int getThird() {
       HGCalDetId id( _id );
       return id.thirdId();
    }

    float getMipT() {   
        float dEdX_weights[] = { 0.0, 8.603,  8.0675,  8.0675,  8.0675,  8.0675,  8.0675,  8.0675,  8.0675,  8.0675,  8.9515,  10.135,  10.135,  10.135,  10.135,  10.135,  10.135,  10.135,  10.135,  10.135,  11.682,  13.654,  13.654,  13.654,  13.654,  13.654,  13.654,  13.654,  38.2005,  55.0265,  49.871,  49.871,  49.871,  49.871,  49.871,  49.871,  49.871,  49.871,  49.871,  49.871,  62.005,  83.1675,  92.196,  92.196,  92.196,  92.196,  92.196,  92.196,  92.196,  92.196,  92.196,  92.196,  46.098
        }; 
        int layer = this->getCorrectedLayer();
        return ( (_energy*thicknessCorrection) /TMath::CosH(_eta) )/( dEdX_weights[layer]*0.001 );
        
    }

    void print(){
        
        cout << ">>> HGCTC : "
             << "zside " << _zside << "; "
             << "layer " << getCorrectedLayer() << "; "
             << "wafer " << _wafer << "; "
             << "third " << getThird() << endl; 
                                 
    }


};

#endif 





