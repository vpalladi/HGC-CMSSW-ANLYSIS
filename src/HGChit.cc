
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
unsigned         HGChit::id()    const                    { return _id       ; }
int              HGChit::subdet()const                    { return _subdet   ; }
float            HGChit::Energy()const                    { return _energy   ; }
float            HGChit::Eta()   const                    { return _eta      ; }
float            HGChit::Phi()   const                    { return _phi      ; }
float            HGChit::x()     const                    { return _z * TMath::Cos( _phi ) * TMath::Tan( _theta ) ; }
float            HGChit::xNorm() const                    { return ( this->x()/fabs(this->z()) );    }
uint32_t         HGChit::xNormDAC(float min, float max, unsigned nbit) const { 
    uint32_t ret = ( (this->xNorm()-min) / ((max-min)/pow(2,nbit)) );
    return ret; }
float            HGChit::y()     const                    { return _z * TMath::Sin( _phi ) * TMath::Tan( _theta ) ; }
float            HGChit::yNorm() const                    { return ( this->y()/fabs(this->z()) );    }
uint32_t         HGChit::yNormDAC(float min, float max, unsigned nbit) const { 
    uint32_t ret = ( (this->yNorm()-min) / ((max-min)/pow(2,nbit)) );
    return ret; }
float            HGChit::z()     const                    { return _z        ; }
float            HGChit::r()     const                    { return TMath::Sqrt( x()*x() + y()*y() ) ; }
unsigned         HGChit::layer() const                    { return _layer    ; }
float            HGChit::Theta() const                    { return _theta    ; }
bool             HGChit::isTrigger() const                { return _isTrigger; }


uint32_t         HGChit::distanceNormDAC( const HGChit* hit, float min, float max, unsigned nbit ) const {
    
    uint32_t dist, xHit, yHit, x, y;

    xHit = hit->xNormDAC(min, max, nbit);
    yHit = hit->yNormDAC(min, max, nbit);
    x = this->xNormDAC(min, max, nbit);
    y = this->yNormDAC(min, max, nbit);

    dist = sqrt( pow(int(x-xHit), 2) + pow(int(y-yHit), 2) );

    return dist;

}


unsigned         HGChit::getEndcapId() const { 
    
    if (_eta>0) 
        return 0; 
    return 1; 

}


unsigned         HGChit::getSectionId() const {
 
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



int HGChit::correctedLayer() const { 
    
  if ( _subdet==3 )      return _layer;
  else if ( _subdet==4 ) return ( _layer + 28 );
  else if ( _subdet==5 ) return ( _layer + 40 );
    
  return -1;

}


TVector3 HGChit::centre() const {

    TVector3 vec;
    vec.SetXYZ(this->x(), this->y(), this->z());

    return vec;

}


double HGChit::distance( const HGChit *hit ) const {

    return TMath::Sqrt( TMath::Power( hit->x()-this->x(), 2) + TMath::Power( hit->y()-this->y(), 2) + TMath::Power( hit->z()-this->z(), 2) );

}


TLorentzVector HGChit::P4() const {

  TLorentzVector p4;
  p4.SetPtEtaPhiE(_pt,_eta,_phi,_energy);
  return p4;

}


float HGChit::Pt() const {

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

//void GetKeys( const HGChit* ahit , uint32_t& aKey1 , uint32_t& aKey2 ) {
//
//    aKey1 = ahit->xNormDAC(-0.6, 0.6, 13);
//    aKey2 = ahit->yNormDAC(-0.6, 0.6, 13);
//
//}


