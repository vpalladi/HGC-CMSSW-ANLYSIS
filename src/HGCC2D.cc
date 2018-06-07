
/* include the .h */
#include "HGCC2D.h"

ClassImp(HGCC2D)

HGCC2D::HGCC2D()  { 
//    _sectionStart[0] = HGCgeom::instance()->layerZ[1 ]; // EE
//    _sectionStart[1] = HGCgeom::instance()->layerZ[29]; // FH
//    _sectionStart[2] = HGCgeom::instance()->layerZ[41]; // BH
//    
//    _sectionEnd[0]   = HGCgeom::instance()->layerZ[28];
//    _sectionEnd[1]   = HGCgeom::instance()->layerZ[40];
//    _sectionEnd[2]   = HGCgeom::instance()->layerZ[52];
     
}


HGCC2D::~HGCC2D() { ; }

/* set C2D parameters */
void             HGCC2D::setCells(vector<unsigned> cells) { 
    _cells = cells ;
    _ncells = _cells.size() ;
}
        
/* get C2D parameters */                      
unsigned         HGCC2D::nCells()                         { return _ncells   ; }
vector<unsigned> HGCC2D::cells()                          { return _cells    ; }


unsigned HGCC2D::HGCROCn() {

    vector<pair<int,int>> thirds;
/* to be reviewed
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
*/
    return thirds.size();

}


bool HGCC2D::isTCcontained( HGCC2D tc ){

    for(unsigned itc=0; itc<tc._ncells; itc++)
        if( find(_cells.begin(), _cells.end(), tc._cells.at(itc) ) == _cells.end() )
            return false;

    return true;

}


void HGCC2D::getEtaSpan( double &minEta, double &maxEta ){
    
    minEta  = 100;
    maxEta  = 0;
//    
//    for(int icell=0; icell<_ncells; icell++){
//        if( _cells[icell].Eta() > maxEta ) maxEta = _cells.Eta();
//        if( _cells[icell].Eta() < minEta ) minEta = _cells.Eta();
//    }
    
}


void HGCC2D::getPhiSpan( double &minPhi, double &maxPhi ){
    
    minPhi  = 10;
    maxPhi  = 0;
    
//    for(int icell=0; icell<_ncells; icell++){
//        if( _cells[icell].Phi() > maxPhi ) maxPhi = _cells.Phi();
//        if( _cells[icell].Phi() < minPhi ) minPhi = _cells.Phi1();
//    }

}


void GetKeys( const HGCC2D &ahit , uint32_t& aKey1 , uint32_t& aKey2 ) {

    aKey1 = ahit.xNormDAC(-0.6, 0.6, 13);
    aKey2 = ahit.yNormDAC(-0.6, 0.6, 13);

}
