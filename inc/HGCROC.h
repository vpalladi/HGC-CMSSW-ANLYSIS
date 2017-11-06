#ifndef HGCROC_H
#define HGCROC_H 1

/* c++ */
#include <iostream>
#include <vector>

/* myIncludes */
#include "HGCTC.h"

using namespace std;

class HGCROC {

public:

    HGCROC();
    HGCROC( HGCTC tc );

    int addTC( HGCTC tc );

    bool tcIsContained( HGCTC &tc );

    /* get TCs related info*/
    std::vector<HGCTC> getTCs();

    unsigned getTCsN();

    unsigned getTCsNifOverMipT(float thrMipT);

    unsigned getTCsInMipTrange(float min, float max);


    /* STDout print */
    void print();

    /* HGCROC energies */
    float Energy();
    float MipT();

private:
    
    std::vector<HGCTC> _tcs;
    int zside;
    int layer;
    int wafer;
    int third;

};

#endif 





