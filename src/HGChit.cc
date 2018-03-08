
/* include the .h */
#include "HGChit.h"

ClassImp(HGChit)

HGChit::HGChit()  { 
    
    _isTrigger=true; 

}

HGChit::~HGChit() { }

/* set hit parameters */
void             HGChit::setId(unsigned id)               { _id = id         ; }
void             HGChit::setSubdet(int subdet)            { _subdet = subdet ; }
void             HGChit::setEnergy(float energy)          { _energy = energy ; }
void             HGChit::setEta(float eta)                { 
    _eta = eta       ; 
    _theta = 2 * TMath::ATan( TMath::Exp( -_eta ) ) ; 
}
void             HGChit::setPhi(float phi)                { _phi = phi       ; }
void             HGChit::setX(float x)                    { _x = x           ; }
void             HGChit::setY(float y)                    { _y = y           ; }
void             HGChit::setZ(float z)                    { _z = z           ; }
void             HGChit::setLayer(int layer)              { _layer = layer   ; }
void             HGChit::setPt(float pt)                  { _pt = pt         ; }
void             HGChit::setIsTrigger(bool val)           { _isTrigger = val ; }
        
/* get hit parameters */                      
unsigned         HGChit::id()                             { return _id       ; }
int              HGChit::subdet()                         { return _subdet   ; }
float            HGChit::Energy()                         { return _energy   ; }
float            HGChit::Eta()                            { return _eta      ; }
float            HGChit::Phi()                            { return _phi      ; }
float            HGChit::x()                              { return _z * TMath::Cos( _phi ) * TMath::Tan( _theta ) ; }
float            HGChit::y()                              { return _z * TMath::Sin( _phi ) * TMath::Tan( _theta ) ; }
float            HGChit::z()                              { return _z        ; }
float            HGChit::r()                              { return TMath::Sqrt( x()*x() + y()*y() ) ; }
unsigned         HGChit::layer()                          { return _layer    ; }
float            HGChit::Theta()                          { return _theta    ; }
bool             HGChit::isTrigger()                      { return _isTrigger; }


unsigned         HGChit::getEndcapId()                    { if (_eta>0) return 0; return 1; }
unsigned         HGChit::getSectionId() {
 
//    std::cout << " >>> HGChit: serching the correct section for endcap " << this->getEndcapId() << "."<< std::endl;
    for(int isection=0; isection<3; isection++) {
        float sectionStarts = HGCgeom::instance()->getSectionStart( this->getEndcapId(), isection);
        float sectionEnds   = HGCgeom::instance()->getSectionEnd( this->getEndcapId(), isection);

//        std::cout << sectionStarts-0.01 << " <= " << _z << " <= " << sectionEnds+0.01 << std::endl;
        if( _z>(sectionStarts-0.01) && _z<(sectionEnds+0.01) ){
            if( isection == (this->subdet()-3) ){
//                std::cout << " >> HGChit: section " << isection << std::endl;
                return isection;
            }
            else {
                std::cout << " >>> !!! WARNING !!! HGChit: the calculated section ("<< isection << ") mismatches with the expected one ("<< this->subdet()-3 <<  ") "<< std::endl;
                return isection;
            }
        }
    }
    
    std::cout << " >>> HGChit: uknown section detected! ( z : " << _z << " )" << std::endl;
    return 5; // unknown section

}



int HGChit::correctedLayer() { 
    
  if ( _subdet==3 )      return _layer;
  else if ( _subdet==4 ) return ( _layer + 28 );
  else if ( _subdet==5 ) return ( _layer + 40 );
    
  return -1;

}


TVector3 HGChit::centre() {

    TVector3 vec;
    vec.SetXYZ(this->x(), this->y(), this->z());

    return vec;

}


double HGChit::distance( HGChit *hit ){

    return TMath::Sqrt( TMath::Power( hit->x()-this->x(), 2) + TMath::Power( hit->y()-this->y(), 2) + TMath::Power( hit->z()-this->z(), 2) );

}


TLorentzVector HGChit::P4(){

  TLorentzVector p4;
  p4.SetPtEtaPhiE(_pt,_eta,_phi,_energy);
  return p4;

}


float HGChit::Pt(){

  return _pt;

}


void HGChit::print() {

    std::cout << "-------------" << std::endl;
    std::cout << " pt : " << _pt << std::endl
              << " ene: " << _pt << std::endl
              << " eta:" << _eta << std::endl
              << " phi:" << _phi << std::endl
              << "  x : " << centre().X() << std::endl
              << "  y : " << centre().Y() << std::endl
              << "  z : " << centre().Z() << std::endl;
    std::cout << "=============" << std::endl;

}

/* operators overloading */
bool HGChit::operator==( const HGChit &comp ){

    bool ret = (comp._id == _id) ? true : false;
    return ret;

}
