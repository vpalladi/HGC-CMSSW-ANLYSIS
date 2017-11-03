
/* include the .h */
#include "HGCC2D.h"


HGCC2D::HGCC2D() {};

   

unsigned HGCC2D::getHGCROCn() {

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


TVector3 HGCC2D::get3VectorCentre() {

    TVector3 vec;
    vec.SetXYZ(_pt, _eta, _phi);

    return vec;

}

void HGCC2D::print() {

    cout << "-------------" << endl;
    cout << " pt : " << _pt << endl
         << " ene: " << _pt << endl
         << " eta:" << _eta << endl
         << " phi:" << _phi << endl
         << "  x : " << get3VectorCentre().X() << endl
         << "  y : " << get3VectorCentre().Y() << endl
         << "  z : " << get3VectorCentre().Z() << endl;
    cout << "=============" << endl;

}


bool HGCC2D::isTCcontained( HGCC2D tc ){

    for(int itc=0; itc<tc._ncells; itc++)
        if( find(_cells.begin(), _cells.end(), tc._cells.at(itc) ) == _cells.end() )
            return false;

    return true;

}


