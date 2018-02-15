
/* incllude the .h */
#include <HGCTC.h>

ClassImp(HGCTC)

HGCTC::HGCTC() {;}
HGCTC::~HGCTC() {;}

const float triggerCellLsbBeforeCompression = 100./1024.;
const int triggerCellTruncationBits = 0;
const float adcSaturationBH_MIP = 1024.;
const int adcNbitsBH = 12;
const float fCperMIP = 2.57;
const double thicknessCorrection = 1.092;


/* set methods */
void     HGCTC::setId(unsigned id)          { _id = id               ; }
void     HGCTC::setSubdet(int subdet)       { _subdet = subdet       ; }
void     HGCTC::setZSide(int zside)         { _zside = zside         ; }
void     HGCTC::setLayer(int layer)         { _layer = layer         ; }
void     HGCTC::setWafer(int wafer)         { _wafer = wafer         ; }
void     HGCTC::setWaferType(int wafertype) { _wafertype = wafertype ; }
void     HGCTC::setCell(int cell)           { _cell = cell           ; }
void     HGCTC::setData(int data)           { 
    _data = data           ; 
    _mipt = this->_MipT()   ;
}
void     HGCTC::setEnergy(float energy)     { 
    _energy = energy         ; 
    _pt = _energy/cosh(_eta) ; 
}
void     HGCTC::setEta(float eta)           { _eta = eta             ; }
void     HGCTC::setPhi(float phi)           { _phi = phi             ; }
void     HGCTC::setX(float x)               { _x = x                 ; }
void     HGCTC::setY(float y)               { _y = y                 ; }
void     HGCTC::setZ(float z)               { _z = z                 ; }

/* get methods */
unsigned HGCTC::id()                        { return _id             ; }
int      HGCTC::subdet()                    { return _subdet         ; }
int      HGCTC::zside()                     { return _zside          ; }
int      HGCTC::layer()                     { return _layer          ; }
int      HGCTC::wafer()                     { return _wafer          ; }
int      HGCTC::wafertype()                 { return _wafertype      ; }
int      HGCTC::cell()                      { return _cell           ; }
unsigned HGCTC::data()                      { return _data           ; }
float    HGCTC::Energy()                    { return _energy         ; }
float    HGCTC::Eta()                       { return _eta            ; }
float    HGCTC::Phi()                       { return _phi            ; }
float    HGCTC::z()                         { return _z              ; }  
                                                                     
/* get Pt and MipT */                                                
float    HGCTC::Pt()                        { return _pt             ; }
float    HGCTC::MipT()                      { return _mipt           ; }

/* get corrected layer */
int HGCTC::correctedLayer() { 
    
    if ( _subdet==3 ) return _layer;
    if ( _subdet==4 ) return ( _layer + 28 );
    if ( _subdet==5 ) return ( _layer + 40 );
    
    return -1;

}


/* pseudo HGCROC id */
int HGCTC::third() {

    HGCalDetId id( _id );
    return id.thirdId();

}


float HGCTC::_MipT() {   

  float LSB_silicon_fC = triggerCellLsbBeforeCompression*(pow(2,triggerCellTruncationBits));
  float LSB_scintillator_MIP = adcSaturationBH_MIP/(pow(2,adcNbitsBH));
  
  /* Convert ADC to charge in fC (in EE+FH) or in MIPs (in BH) */
  float amplitude = _data * (_subdet==5 ? LSB_scintillator_MIP :  LSB_silicon_fC);
  
  /* convert the charge amplitude in MIP: */
  float trgCellMipP = amplitude;
  if( _subdet!=5 && fCperMIP > 0 ){
    trgCellMipP /= fCperMIP; 
  }
  
  /* compute the transverse-mip */
  float trgCellMipPt = trgCellMipP/cosh( _eta ); 
 
  /* setting pT [mip] */
  return trgCellMipPt;
    
}


TLorentzVector HGCTC::P4(){

  TLorentzVector p4;
  p4.SetPtEtaPhiE(this->Pt(),_eta,_phi,_energy);
  return p4;

}


void HGCTC::print(){
    
    cout << ">>> HGCTC : "
         << "zside " << _zside << "; "
         << "layer " << correctedLayer() << "; "
         << "wafer " << _wafer << "; "
         << "third " << third() << endl; 
                                 
}

/* operators overloading */
bool HGCTC::operator==( const HGCTC &comp ){

    bool ret = (comp._id == _id) ? true : false;
    return ret;

}



