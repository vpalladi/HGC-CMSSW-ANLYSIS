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

using namespace std;



class HGCC2D {

public:

    HGCC2D() {};

    float                _pt     ;
    float                _energy ;
    float                _eta    ;
    float                _phi    ;
    int                  _layer  ;
    int                  _ncells ;
    vector<unsigned int> _cells  ; // this is the HGCDetId

    unsigned getHGCROCn() {
        vector<pair<int,int>> thirds;
        for( int itc=0; itc<_ncells; itc++ ) {
            bool found = false;
            HGCalDetId tcId( _cells.at(itc) );
            pair<int,int> newP( tcId.wafer(), tcId.thirdId() );
            for( unsigned i=0; i<thirds.size(); i++ ) 
                if( find(thirds.begin(), thirds.end(), newP) != thirds.end() )
                    found=true;
            
            if( !found )
                thirds.push_back( newP );
        }
        return thirds.size();
    }


};

#endif 





