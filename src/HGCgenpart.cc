


#include "HGCgenpart.h"

ClassImp(HGCgenpart)


HGCgenpart::HGCgenpart()  { ; }
HGCgenpart::~HGCgenpart() { ; }


void HGCgenpart::setUniqueId     (int           UniqueId     ) { _UniqueId    = UniqueId    ; }
//void HGCgenpart::setEta          (float         Eta          ) { _Eta         = Eta         ; }
//void HGCgenpart::setPhi          (float         Phi          ) { _Phi         = Phi         ; }
//void HGCgenpart::setPt           (float         Pt           ) { _Pt          = Pt          ; }
//void HGCgenpart::setEnergy       (float         Energy       ) { _Energy      = Energy      ; }
void HGCgenpart::setDvx          (float         Dvx          ) { _Dvx         = Dvx         ; }
void HGCgenpart::setDvy          (float         Dvy          ) { _Dvy         = Dvy         ; }
void HGCgenpart::setDvz          (float         Dvz          ) { _Dvz         = Dvz         ; }
void HGCgenpart::setOvx          (float         Ovx          ) { _Ovx         = Ovx         ; }
void HGCgenpart::setOvy          (float         Ovy          ) { _Ovy         = Ovy         ; }
void HGCgenpart::setOvz          (float         Ovz          ) { _Ovz         = Ovz         ; }
void HGCgenpart::setExx          (float         Exx          ) { _Exx         = Exx         ; }
void HGCgenpart::setExy          (float         Exy          ) { _Exy         = Exy         ; }
void HGCgenpart::setMother       (int           Mother       ) { _Mother      = Mother      ; }
void HGCgenpart::setExphi        (float         Exphi        ) { _Exphi       = Exphi       ; }
void HGCgenpart::setExeta        (float         Exeta        ) { _Exeta       = Exeta       ; }
void HGCgenpart::setFbrem        (float         Fbrem        ) { _Fbrem       = Fbrem       ; }
//void HGCgenpart::setPDGid        (int           Pid          ) { _Pid         = Pid         ; }
void HGCgenpart::setGen           (int           Gen          ) { _Gen         = Gen         ; }
void HGCgenpart::setReachedEE    (int           ReachedEE    ) { _ReachedEE   = ReachedEE   ; }
void HGCgenpart::setFromBeamPipe (bool          FromBeamPipe ) { _FromBeamPipe= FromBeamPipe; }
void HGCgenpart::setPosx  (vector<float> Posx  ) { _Posx = Posx ; }
void HGCgenpart::setPosy  (vector<float> Posy  ) { _Posy = Posy ; }
void HGCgenpart::setPosz  (vector<float> Posz  ) { _Posz = Posz ; }

int           HGCgenpart::UniqueId     () { return _UniqueId    ; } 
//float         HGCgenpart::Eta          () { return _Eta         ; } 
//float         HGCgenpart::Phi          () { return _Phi         ; } 
//float         HGCgenpart::Pt           () { return _Pt          ; } 
//float         HGCgenpart::Energy       () { return _Energy      ; } 
float         HGCgenpart::Dvx          () { return _Dvx         ; } 
float         HGCgenpart::Dvy          () { return _Dvy         ; } 
float         HGCgenpart::Dvz          () { return _Dvz         ; } 
float         HGCgenpart::Ovx          () { return _Ovx         ; } 
float         HGCgenpart::Ovy          () { return _Ovy         ; } 
float         HGCgenpart::Ovz          () { return _Ovz         ; } 
float         HGCgenpart::Exx          () { return _Exx         ; } 
float         HGCgenpart::Exy          () { return _Exy         ; } 
int           HGCgenpart::Mother       () { return _Mother      ; } 
float         HGCgenpart::Exphi        () { return _Exphi       ; } 
float         HGCgenpart::Exeta        () { return _Exeta       ; } 
float         HGCgenpart::Fbrem        () { return _Fbrem       ; } 
//int           HGCgenpart::PDGid        () { return _Pid         ; } 
int           HGCgenpart::Gen          () { return _Gen         ; } 
int           HGCgenpart::ReachedEE    () { return _ReachedEE   ; } 
bool          HGCgenpart::FromBeamPipe () { return _FromBeamPipe; } 
vector<float> HGCgenpart::Posx  () { return _Posx ; } 
vector<float> HGCgenpart::Posy  () { return _Posy ; } 
vector<float> HGCgenpart::Posz  () { return _Posz ; } 


bool HGCgenpart::hasPos(){

    if(_Posx.size()>0 ){
        if(_Posz.at(0)>0.01)
            return true;
    }
    return false;

}


double HGCgenpart::getRhoProj(unsigned ilayer){

    ilayer--; // get the correct one    
    double Rho = TMath::Sqrt( _Posx.at(ilayer)*_Posx.at(ilayer) + _Posy.at(ilayer)*_Posy.at(ilayer) );

    return Rho;

}


double HGCgenpart::getPhiProj(unsigned ilayer){

    ilayer--; // get the correct one
    double Phi = TMath::ATan( _Posy.at(ilayer) / _Posx.at(ilayer) );
    
    if( _Posx.at(ilayer) <= 0 && _Posy.at(ilayer) >= 0 )
        Phi = - Phi;
    else if( _Posx.at(ilayer) <= 0 && _Posy.at(ilayer) <= 0 )
        Phi = -TMath::Pi() + Phi;
    else if( _Posx.at(ilayer) >= 0 && _Posy.at(ilayer) <= 0 )
        Phi = - Phi;

    return Phi;

}


int HGCgenpart::getPhiSectorProj(unsigned ilayer, unsigned nPhi, double* minPhi, double* maxPhi){

    double phi = getPhiProj(ilayer);
    
    for(unsigned isector=0; isector<nPhi; isector++){
        if(phi>=minPhi[isector] && phi<maxPhi[isector] )
            return isector;
    }

    return -1;

}


HGChoughPoint HGCgenpart::getHTcoordinate(double zOffset){
    HGChoughPoint p;

    if( _Posx.size()>0 ){
        double rho_0  = getRhoProj(1);
        double z_0    = _Posz.at(0);
        
        double rho_1  = getRhoProj(2);
        double z_1    = _Posz.at(1);
        
        double tan  = (rho_1-rho_0)/(z_1-z_0);
        double rho0 = rho_0 - tan * (z_0-zOffset);
        
        p.setX( tan );
        p.setY( rho0 );
    }
    else{
        p.setX( -1 );
        p.setY( -1 );
    }

        return p;

}


TF1 HGCgenpart::getHT(unsigned ilayer, double zOffset, double zWeight){

    TF1 f("f", "[0]+[1]*x", -1, 1);
    double Rho = getRhoProj(ilayer);
    f.SetParameter( 0, Rho );
    f.SetParameter( 1, -zWeight*(_Posz.at(--ilayer)-zOffset) );

    return f;

}


//int HGCgenpart::getEndcapId(){ 
//
//    if (_Eta>0) return 0; 
//    return 1;
//
//}
