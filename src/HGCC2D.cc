
/* include the .h */
#include "HGCC2D.h"

ClassImp(HGCC2D)

HGCC2D::HGCC2D()  { ; }
HGCC2D::~HGCC2D() { ; }

/* set C2D parameters */
void             HGCC2D::setId(unsigned id)               { _id = id         ; }
void             HGCC2D::setSubdet(int subdet)            { _subdet = subdet ; }
void             HGCC2D::setPt(float pt)                  { _pt = pt         ; }
void             HGCC2D::setEnergy(float energy)          { _energy = energy ; }
void             HGCC2D::setEta(float eta)                { 
    _eta = eta       ; 
    _theta = 2 * TMath::ATan( TMath::Exp( -_eta ) ) ; 
}
void             HGCC2D::setPhi(float phi)                { _phi = phi       ; }
//void             HGCC2D::setX(float x)                    { _x = x           ; }
//void             HGCC2D::setY(float y)                    { _y = y           ; }
void             HGCC2D::setZ(float z)                    { _z = z           ; }
void             HGCC2D::setLayer(int layer)              { _layer = layer   ; }
void             HGCC2D::setCells(vector<unsigned> cells) { 
    _cells = cells ;
    _ncells = _cells.size() ;
}
        
/* get C2D parameters */                      
unsigned         HGCC2D::id()                             { return _id       ; }
int              HGCC2D::subdet()                         { return _subdet   ; }
float            HGCC2D::Pt()                             { return _pt       ; }
float            HGCC2D::Energy()                         { return _energy   ; }
float            HGCC2D::Eta()                            { return _eta      ; }
float            HGCC2D::Phi()                            { return _phi      ; }
float            HGCC2D::x()                              { return _z * TMath::Cos( _phi ) * TMath::Tan( _theta ) ; }
float            HGCC2D::y()                              { return _z * TMath::Sin( _phi ) * TMath::Tan( _theta ) ; }
float            HGCC2D::z()                              { return _z        ; }
float            HGCC2D::r()                              { return TMath::Sqrt( x()*x() + y()*y() ) ; }
unsigned         HGCC2D::layer()                          { return _layer    ; }
unsigned         HGCC2D::nCells()                         { return _ncells   ; }
vector<unsigned> HGCC2D::cells()                          { return _cells    ; }


int HGCC2D::correctedLayer() { 
    
  if ( _subdet==3 )      return _layer;
  else if ( _subdet==4 ) return ( _layer + 28 );
  else if ( _subdet==5 ) return ( _layer + 40 );
    
  return -1;

}


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


TVector3 HGCC2D::centre() {

    TVector3 vec;
    vec.SetXYZ(this->x(), this->y(), this->z());

    return vec;

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


TLorentzVector HGCC2D::P4(){

  TLorentzVector p4;
  p4.SetPtEtaPhiE(_pt,_eta,_phi,_energy);
  return p4;

}


void HGCC2D::print() {

    cout << "-------------" << endl;
    cout << " pt : " << _pt << endl
         << " ene: " << _pt << endl
         << " eta:" << _eta << endl
         << " phi:" << _phi << endl
         << "  x : " << centre().X() << endl
         << "  y : " << centre().Y() << endl
         << "  z : " << centre().Z() << endl;
    cout << "=============" << endl;

}



