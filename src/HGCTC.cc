
/* incllude the .h */
#include <HGCTC.h>

HGCTC::HGCTC() {;}

int HGCTC::getCorrectedLayer() { 
    
    if ( _subdet==3 ) return _layer;
    if ( _subdet==4 ) return ( _layer + 28 );
    if ( _subdet==5 ) return ( _layer + 40 );
    
    return -1;

}

int HGCTC::getThird() {

    HGCalDetId id( _id );
    return id.thirdId();

}

float HGCTC::getMipT() {   

    float dEdX_weights[] = { 0.0, 8.603,  8.0675,  8.0675,  8.0675,  8.0675,  8.0675,  8.0675,  8.0675,  8.0675,  8.9515,  10.135,  10.135,  10.135,  10.135,  10.135,  10.135,  10.135,  10.135,  10.135,  11.682,  13.654,  13.654,  13.654,  13.654,  13.654,  13.654,  13.654,  38.2005,  55.0265,  49.871,  49.871,  49.871,  49.871,  49.871,  49.871,  49.871,  49.871,  49.871,  49.871,  62.005,  83.1675,  92.196,  92.196,  92.196,  92.196,  92.196,  92.196,  92.196,  92.196,  92.196,  92.196,  46.098
    }; 
    int layer = this->getCorrectedLayer();

    return ( (_energy*thicknessCorrection) /TMath::CosH(_eta) )/( dEdX_weights[layer]*0.001 );
    
}

void HGCTC::print(){
    
    cout << ">>> HGCTC : "
         << "zside " << _zside << "; "
         << "layer " << getCorrectedLayer() << "; "
         << "wafer " << _wafer << "; "
         << "third " << getThird() << endl; 
                                 
}

/* operators overloading */
bool HGCTC::operator==( const HGCTC &comp ){

    bool ret = (comp._id == _id) ? true : false;
    return ret;

}



