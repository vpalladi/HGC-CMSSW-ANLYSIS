
/* incllude the .h */
#include <HGCTC.h>

ClassImp(HGCTC)

HGCTC::HGCTC() {;}
HGCTC::~HGCTC() {;}

/* set methods */
void     HGCTC::setZSide(int zside)         { _zside = zside         ; }
void     HGCTC::setWafer(int wafer)         { _wafer = wafer         ; }
void     HGCTC::setWaferType(int wafertype) { _wafertype = wafertype ; }
void     HGCTC::setCell(int cell)           { _cell = cell           ; }
void     HGCTC::setData(int data)           { _data = data           ; }

/* get methods */
int      HGCTC::zside()                     { return _zside          ; }
int      HGCTC::wafer()                     { return _wafer          ; }
int      HGCTC::wafertype()                 { return _wafertype      ; }
int      HGCTC::cell()                      { return _cell           ; }
unsigned HGCTC::data()                      { return _data           ; }

/* transverse quantities */
float HGCTC::MipT() {   

    const float triggerCellLsbBeforeCompression = 100./1024.;
    const int triggerCellTruncationBits = 0;
    const float adcSaturationBH_MIP = 1024.;
    const int adcNbitsBH = 12;
    const float fCperMIP = 2.57;
    const double thicknessCorrection = 1.092;
    

    float LSB_silicon_fC = triggerCellLsbBeforeCompression*(pow(2,triggerCellTruncationBits));
    float LSB_scintillator_MIP = adcSaturationBH_MIP/(pow(2,adcNbitsBH));
  
    /* Convert ADC to charge in fC (in EE+FH) or in MIPs (in BH) */
    float amplitude = _data * (this->subdet()==5 ? LSB_scintillator_MIP :  LSB_silicon_fC);
  
    /* convert the charge amplitude in MIP: */
    float trgCellMipP = amplitude;
    if( this->subdet()!=5 && fCperMIP > 0 ){
        trgCellMipP /= fCperMIP; 
    }
  
    /* compute the transverse-mip */
    float trgCellMipPt = trgCellMipP/cosh( this->Eta() ); 
 
    /* setting pT [mip] */
    return trgCellMipPt;
    
}


/* pseudo HGCROC id */
int HGCTC::third() {

    HGCalDetId id( this->id() );
    return id.thirdId();

}


