
#include "HGCgen.h"
#include "HGCgeom.h"


ClassImp(HGCgen)


//int HGCgen::charge(int id){
//    int q(0);
//        
//    // Charged leptons
//    if(i==   11) q=-1;
//    if(i==  -11) q=+1;
//    if(i==   13) q=-1;
//    if(i==  -13) q=+1;
//    // NOT taus! Want direction of production
//        
//    // Mesons
//    if(abs(i)>=100 && abs(i)<=999) {
//            
//        unsigned quark(abs(i)/100);
//        unsigned antiq((abs(i)/10)%10);
//            
//        q=((quark%2)==0?2:-1)-((antiq%2)==0?2:-1);
//            
//        // For mesons, all positive PDG id particles have positive charge
//        if(q!=0) {
//            if(i>0) q=+1;
//            if(i<0) q=-1;
//        }
//            
//        if(i==  211) assert(q== 1);
//        if(i== -211) assert(q==-1);
//        if(i==  321) assert(q== 1);
//        if(i== -321) assert(q==-1);
//            
//        /*
//          if(i==  211) q= 1;
//          if(i== -211) q=-1;
//          if(i==  321) q= 1;
//          if(i== -321) q=-1;
//        */
//    }
//        
//    // Baryons
//    if(abs(i)>=1000 && abs(i)<=9999) {
//        unsigned quark1(abs(i)/1000);
//        unsigned quark2((abs(i)/100)%10);
//        unsigned quark3((abs(i)/10)%10);
//            
//        q=((quark1%2)==0?2:-1)+((quark2%2)==0?2:-1)+((quark3%2)==0?2:-1);
//        assert((q%3)==0);
//        q/=3;
//        if(i<0) q=-q;
//            
//        if(i== 2212) assert(q== 1);
//        if(i==-2212) assert(q==-1);
//    }
//        
//    return q;
//}
    

HGCgen::HGCgen()  { ; }
HGCgen::~HGCgen() { ; }

void HGCgen::setPDGid ( int   PDGid  ) { _PDGid  = PDGid    ; }
void HGCgen::setId    ( int   id     ) { _id     = id    ; }
void HGCgen::setStatus( float Status ) { _Status = Status; }
void HGCgen::setEnergy( float Energy ) { _Energy = Energy; }
void HGCgen::setPt    ( float Pt     ) { _Pt     = Pt    ; }
void HGCgen::setEta   ( float Eta    ) { _Eta    = Eta   ; }
void HGCgen::setPhi   ( float Phi    ) { _Phi    = Phi   ; }

int   HGCgen::PDGid () { return _PDGid  ; }
int   HGCgen::id    () { return _id    ; }
float HGCgen::Status() { return _Status; }
float HGCgen::Energy() { return _Energy; }
float HGCgen::Pt    () { return _Pt    ; }
float HGCgen::Eta   () { return _Eta   ; }
float HGCgen::Phi   () { return _Phi   ; }
float HGCgen::xNorm () { 
    double layerZ = HGCgeom::instance()->layerZ(this->getEndcapId(), 1);
    double proj = this->getZprojection( layerZ ).X() / fabs( this->getZprojection( layerZ ).Z() );
    return proj;
}
float HGCgen::yNorm () { 
    double layerZ = HGCgeom::instance()->layerZ(this->getEndcapId(), 1);
    double proj = this->getZprojection( layerZ ).Y() / fabs( this->getZprojection( layerZ ).Z() );
    return proj;
}

ROOT::Math::RhoEtaPhiPoint HGCgen::getZprojection(double z){
    
    ROOT::Math::RhoEtaPhiPoint proj( z*TMath::Tan(2*TMath::ATan(TMath::Exp(-_Eta))), _Eta, _Phi );
    //ROOT::Math::RhoEtaPhiPoint proj( 0, 0, 0 );
    
    return proj;

}


int HGCgen::getEndcapId(){ 

    if (_Eta>0) return 0; 
    return 1;

}


int HGCgen::getPhiSectorProj(unsigned nPhi, double* minPhi, double* maxPhi, double z){
    
    double phi = getZprojection( z ).Phi();
    
    for(unsigned isector=0; isector<nPhi; isector++)
        if(phi>minPhi[isector] && phi<maxPhi[isector] )
            return isector;
    
    return -1;
    
}

TF1 HGCgen::getHT(double z, double zOffset, double zWeight){
    
    TF1 f("f", "[0]+[1]*x", -1, 1);
    f.SetParameter( 0, this->getZprojection( z ).Rho() );
    f.SetParameter( 1, -zWeight*(z-zOffset) );

    return f;

}

