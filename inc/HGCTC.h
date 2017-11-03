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

    HGCTC();

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

    int getCorrectedLayer();
    int getThird();
    float getMipT();

    void print();

    
    /* operators overloading */
    bool operator==( const HGCTC &comp );

};

#endif 





