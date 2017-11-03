
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


TVector3 HGCC2D::get3Vector() {

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
         << "  x : " << get3Vector().X() << endl
         << "  y : " << get3Vector().Y() << endl
         << "  z : " << get3Vector().Z() << endl;
    cout << "=============" << endl;

}
