
/* include the .h */
#include "HGCROC.h"

HGCROC::HGCROC() { ; }
HGCROC::HGCROC( HGCTC tc ) { 

  zside = tc.zside();
  layer = tc.correctedLayer();
  wafer = tc.wafer();
  third = tc.third();

    if( addTC( tc ) == -1 ){
        cout << "----ERROR-TC-not-FOUND----" << endl;
        print();
        tc.print();
        cout << "++++++++++++++++++++++++++" << endl;
    } 

}

int HGCROC::addTC( HGCTC tc ) { 

    if(  tcIsContained( tc ) ) {
        _tcs.push_back( tc );
        return 0;
    }
    else {
        return -1;
    }

}

bool HGCROC::tcIsContained( HGCTC &tc ){

  if( (zside == tc.zside()) and
      (layer == tc.correctedLayer()) and
      (wafer == tc.wafer()) and
      (third == tc.third()) ) {
    return true;
  }
  else {
    return false;    
  }
 
}

/* get TCs related info*/
std::vector<HGCTC> HGCROC::getTCs() { 

    return _tcs; 

}

unsigned HGCROC::getTCsN() {

    return _tcs.size(); 

}

unsigned HGCROC::getTCsNifOverMipT(float thrMipT) { 

    unsigned n=0;

    for( unsigned itc=0; itc<_tcs.size(); itc++ )
        n = ( _tcs.at(itc).MipT() > thrMipT ) ? n+1 : n;

    return n;

}

unsigned HGCROC::getTCsInMipTrange(float min, float max) { 

    unsigned n=0;

    for( unsigned itc=0; itc<_tcs.size(); itc++ )
        n = ( _tcs.at(itc).MipT() > min ) ? n+1 : n;

    return n;

}


/* STDout print */
void HGCROC::print(){

    cout << "----------------------------------------" << endl;
    cout << ">>> HGCROC : "
         << "zside " << zside << "; "
         << "layer " << layer << "; "
         << "wafer " << wafer << "; "
         << "third " << third << endl; 
    cout << "      Contains " << _tcs.size() << " TCs" << endl;
    cout << "      Energy   " << getEnergy() << " GeV" << endl;
    cout << "      Energy   " << getMipT()   << " MipT" << endl;
    cout << "++++++++++++++++++++++++++++++++++++++++" << endl;

}

/* HGCROC energies */
float HGCROC::getEnergy() {
        
    float ene=0;

    for(unsigned itc=0; itc<_tcs.size(); itc++)
      ene += _tcs.at( itc ).energy();

    return ene;

}

float HGCROC::getMipT() { 

    float mipt=0;

    for(unsigned itc=0; itc<_tcs.size(); itc++)
        mipt += _tcs.at( itc ).MipT();

    return mipt;

}
