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

    HGCC2D() {};

    float                _pt     ;
    float                _energy ;
    float                _eta    ;
    float                _phi    ;
    float                _x    ;
    float                _y    ;
    float                _z    ;
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

    TVector3 get3Vector() {
        TVector3 vec;
        vec.SetXYZ(_pt, _eta, _phi);
        return vec;
    }

    void print() {
        cout << "-------------" << endl;
        cout << " pt : " << _pt << endl
             << " ene: " << _pt << endl
             << " eta:" << _eta << endl
             << " phi:" << _phi << endl
             << "  x : " << get3Vector().X() << endl
             << "  y : " << get3Vector().Y() << endl
             << "  z : " << get3Vector().Z() << endl;
        cout << "=============" << endl;
    }


};

#endif 





