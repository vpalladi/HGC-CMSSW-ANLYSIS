
/* incllude the .h */
#include <HGCTC.h>

HGCTC::HGCTC() {;}

const float triggerCellLsbBeforeCompression = 100./1024.;
const int triggerCellTruncationBits = 0;
const float adcSaturationBH_MIP = 1024.;
const int adcNbitsBH = 12;
const float fCperMIP = 2.57;
const double thicknessCorrection = 1.092;


int HGCTC::correctedLayer() { 
    
    if ( _subdet==3 ) return _layer;
    if ( _subdet==4 ) return ( _layer + 28 );
    if ( _subdet==5 ) return ( _layer + 40 );
    
    return -1;

}

int HGCTC::third() {

    HGCalDetId id( _id );
    return id.thirdId();

}

float HGCTC::MipT() {   

  float LSB_silicon_fC = triggerCellLsbBeforeCompression*(pow(2,triggerCellTruncationBits));
  float LSB_scintillator_MIP = adcSaturationBH_MIP/(pow(2,adcNbitsBH));
  
  // Convert ADC to charge in fC (in EE+FH) or in MIPs (in BH)
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



