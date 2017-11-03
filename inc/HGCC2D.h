#ifndef HGCC2D_H
#define HGCC2D_H 1

/* c/c++ */
#include <iostream>
#include <vector>
#include <utility> // pair

/* mylibs */
#include "detId.h"

/* ROOT */
#include "TMath.h"
#include "TVector3.h"


using namespace std;


class HGCC2D {

public:

    HGCC2D();

    float                _pt     ;
    float                _energy ;
    float                _eta    ;
    float                _phi    ;
    float                _x    ;
    float                _y    ;
    float                _z    ;
    int                  _layer  ;
    int                  _ncells ;
    vector<unsigned int> _cells  ; // vector of HGCDetId for all the TC 

    unsigned getHGCROCn();
    TVector3 get3VectorCentre();

    bool isTCcontained( HGCC2D tc );

    void print();


};

#endif 
