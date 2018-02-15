

#include "HGCgenpart.h"

ClassImp(HGCgenpart)


HGCgenpart::HGCgenpart()  { ; }
HGCgenpart::~HGCgenpart() { ; }


void HGCgenpart::setEta          (float         Eta          ) { _Eta         = Eta         ; }
void HGCgenpart::setPhi          (float         Phi          ) { _Phi         = Phi         ; }
void HGCgenpart::setPt           (float         Pt           ) { _Pt          = Pt          ; }
void HGCgenpart::setEnergy       (float         Energy       ) { _Energy      = Energy      ; }
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
void HGCgenpart::setPDGid        (int           Pid          ) { _Pid         = Pid         ; }
void HGCgenpart::setGen           (int           Gen          ) { _Gen         = Gen         ; }
void HGCgenpart::setReachedEE    (int           ReachedEE    ) { _ReachedEE   = ReachedEE   ; }
void HGCgenpart::setFromBeamPipe (bool          FromBeamPipe ) { _FromBeamPipe= FromBeamPipe; }
void HGCgenpart::setPosx  (vector<float> Posx  ) { _Posx = Posx ; }
void HGCgenpart::setPosy  (vector<float> Posy  ) { _Posy = Posy ; }
void HGCgenpart::setPosz  (vector<float> Posz  ) { _Posz = Posz ; }

float         HGCgenpart::Eta          () { return _Eta         ; } 
float         HGCgenpart::Phi          () { return _Phi         ; } 
float         HGCgenpart::Pt           () { return _Pt          ; } 
float         HGCgenpart::Energy       () { return _Energy      ; } 
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
int           HGCgenpart::PDGid        () { return _Pid         ; } 
int           HGCgenpart::Gen          () { return _Gen         ; } 
int           HGCgenpart::ReachedEE    () { return _ReachedEE   ; } 
bool          HGCgenpart::FromBeamPipe () { return _FromBeamPipe; } 
vector<float> HGCgenpart::Posx  () { return _Posx ; } 
vector<float> HGCgenpart::Posy  () { return _Posy ; } 
vector<float> HGCgenpart::Posz  () { return _Posz ; } 
