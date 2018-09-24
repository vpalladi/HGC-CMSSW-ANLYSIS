
#include "HGCgen.h"
#include "HGCgeom.h"

ClassImp(HGCgen)


HGCgen::HGCgen()  { ; }
HGCgen::~HGCgen() { ; }

void  HGCgen::setPDGid   ( int   PDGid  ) { _PDGid  = PDGid ; }
void  HGCgen::setId      ( int   id     ) { _id     = id    ; }
void  HGCgen::setStatus  ( float Status ) { _Status = Status; }
void  HGCgen::setEnergy  ( float Energy ) { _Energy = Energy; }
void  HGCgen::setPt      ( float Pt     ) { _Pt     = Pt    ; }
void  HGCgen::setEta     ( float Eta    ) { _Eta    = Eta   ; }
void  HGCgen::setPhi     ( float Phi    ) { _Phi    = Phi   ; }
void  HGCgen::setGenjetId( int genjetId ) { _genjetId = genjetId; }

int   HGCgen::PDGid   () { return _PDGid ; }
int   HGCgen::id      () { return _id    ; }
float HGCgen::Status  () { return _Status; }
float HGCgen::Energy  () { return _Energy; }
float HGCgen::Pt      () { return _Pt    ; }
float HGCgen::Eta     () { return _Eta   ; }
float HGCgen::Phi     () { return _Phi   ; }
int   HGCgen::genjetId() { return _genjetId; }

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

    TDatabasePDG* PDGdb = TDatabasePDG::Instance();
    TParticlePDG* particle = PDGdb->GetParticle( _PDGid );
    
    ROOT::Math::RhoEtaPhiPoint proj( 0, 0, 0 );
    
    if( particle == NULL )
        return proj;

    double px = _Pt*sin(_Phi);
    double py = _Pt*cos(_Phi);
    double pz = sqrt( pow( _Energy, 2 ) - pow( particle->Mass(), 2 ) - pow( _Pt, 2 ) );
//    cout << " ------ " << endl;
//    cout << "nm " << particle->GetName() << endl;
//    cout << "en " << _Energy << endl;
//    cout << "ms " << particle->Mass() << endl;
//    cout << "pt " << _Pt << endl;
//    cout << "px " << px << endl;
//    cout << "py " << py << endl;
//    cout << "pz " << pz << endl;
    
    TVector3 fMomentum(px, py, pz);

    const double fBField=0.001*2.998*3.8; // 0.01 (m->cm) x c x B/10^9 (GeV->eV)
    
    int q = round( 3*particle->Charge() ); // Charge of particle
    
    double dz = z; // Change of z
    
    double x = 0; // Original x
    double y = 0; // Original y

    TVector3 fPosition(0, 0, 0);

    if( q == 0 ) {
        
        // Increment x and y
        x+=dz*fMomentum.X()/fMomentum.Z();
        y+=dz*fMomentum.Y()/fMomentum.Z();
        
        // Set new position
        fPosition = TVector3(x, y, z);
        
        // The momentum is unchanged
        
    } else {
        
        // Find angle of bend
        double dPhi = (dz * q * fBField ) / fMomentum.z();
        
        // Increment x and y
        x += (fMomentum.X() * sin(dPhi) + fMomentum.Y() * (1.0-cos(dPhi)) )/(q*fBField);
        y += (fMomentum.Y() * sin(dPhi) - fMomentum.X() * (1.0-cos(dPhi)) )/(q*fBField);
        
        // Set new position
        fPosition = TVector3(x, y, z);
        
        // Find new momentum px and py
//        double px = fMomentum.x()*cos(dPhi)+fMomentum.y()*sin(dPhi);
//        double py = fMomentum.y()*cos(dPhi)-fMomentum.x()*sin(dPhi);
        
        // Set new momentum
//        fMomentum = TVector3( px, py, fMomentum.z() );

    }


    /*must translate xyz in rhoetaphi*/
    proj = ROOT::Math::RhoEtaPhiPoint( z*TMath::Tan(2*TMath::ATan(TMath::Exp(-_Eta))), _Eta, _Phi );
    //ROOT::Math::RhoEtaPhiPoint proj( 0, 0, 0 );
    
    return proj;

}


float HGCgen::distanceNorm(HGCgen* gen) {

    float d = sqrt( pow(gen->xNorm()-this->xNorm(), 2) + pow(gen->yNorm()-this->yNorm(), 2) ); 
    return d;

}


float HGCgen::distanceEtaPhi(HGCgen* gen) {

    float d = sqrt( pow(gen->Eta()-this->Eta(), 2) + pow(gen->Phi()-this->Phi(), 2) ); 
    return d;

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

//int HGCgen::myCharge() {
//
//    int q(0);
//    
//    // Charged leptons
//    if(_PDGid==   11) q=-1;
//    if(_PDGid==  -11) q=+1;
//    if(_PDGid==   13) q=-1;
//    if(_PDGid==  -13) q=+1;
//    if(_PDGid==   15) q=-1;
//    if(_PDGid==  -15) q=+1;
//    
//    // Mesons
//    if(abs(_PDGid)>=100 && abs(_PDGid)<=999) {
//        
//        unsigned quark(abs(_PDGid)/100);
//        unsigned antiq((abs(_PDGid)/10)%10);
//        
//        q=( (quark%2) == 0 ? 2 : -1 )-( (antiq%2) == 0 ? 2 : -1 );
//        
//        // For mesons, all positive PDG id particles have positive charge
//        if( q != 0 ) {
//            if( _PDGid > 0 ) q=+1;
//            if( _PDGid < 0 ) q=-1;
//        }
//        
//        if( _PDGid ==  211 ) assert( q == 1);
//        if( _PDGid == -211 ) assert( q ==-1);
//        if( _PDGid ==  321 ) assert( q == 1);
//        if( _PDGid == -321 ) assert( q ==-1);
//    }
//    
//    // Baryons
//    if( abs(_PDGid) >= 1000 && abs(_PDGid) <= 9999 ) {
//        unsigned quark1( abs(_PDGid)/1000 );
//        unsigned quark2( (abs(_PDGid)/100)%10 );
//        unsigned quark3( (abs(_PDGid)/10)%10 );
//        
//        q = ( (quark1%2)==0 ? 2 : -1 ) + ( (quark2%2) == 0 ? 2 : -1 ) + ( (quark3%2) == 0 ? 2 : -1 );
//        assert( (q%3) == 0 );
//        q/=3;
//        if(_PDGid<0) q=-q;
//        
//        if( _PDGid == 2212 ) assert( q== 1 );
//        if( _PDGid ==-2212 ) assert( q==-1 );
//    }
//    
//    return q;
//
//}
