#ifndef HGCGENPART_H
#define HGCGENPART_H 1

/* c/c++ */
#include <iostream>
#include <map>     // needed to correctly genreate dictionary
#include <vector>
#include <utility> // pair

/* mylibs */
#include "detId.h"

/* ROOT */
#include "TObject.h"


using namespace std;

class HGCgenpart : public TObject {


public:

    HGCgenpart();
    ~HGCgenpart();

    void setEta          (float         Eta          ) ; // track momentum eta
    void setPhi          (float         Phi          ) ; // track momentum phi
    void setPt           (float         Pt           ) ; // track momentum pt
    void setEnergy       (float         Energy       ) ; // track momentum energy
    void setDvx          (float         Dvx          ) ; // track end (destination) x
    void setDvy          (float         Dvy          ) ; // track end (destination) y
    void setDvz          (float         Dvz          ) ; // track end (destination) z
    void setOvx          (float         Ovx          ) ; // track origin x 
    void setOvy          (float         Ovy          ) ; // track origin y 
    void setOvz          (float         Ovz          ) ; // track origin z 
    void setExx          (float         Exx          ) ; // hit the HGCal x (layer 1)
    void setExy          (float         Exy          ) ; // hit the HGCal y (layer 1)
    void setMother       (int           Mother       ) ; // index of the mother in the vectors    
    void setExphi        (float         ExPhi        ) ; // hit the HGCal phi (layer 1)
    void setExeta        (float         ExEta        ) ; // hit the HGCal eta (layer 1)
    void setFbrem        (float         Fbrem        ) ; // percentage of bremstrahlung for photons
    void setPDGid        (int           Pid          ) ; // PDG id
    void setGen          (int           Gen          ) ; // gen part index
    void setReachedEE    (int           ReachedEE    ) ; // notReach = 0; outsideEESurface = 1; onEESurface = 2
    void setFromBeamPipe (bool          FromBeamPipe ) ; // ??? seems always true
    void setPosx  (vector<float> Posx  ) ; // projection for different layers; filled only fir there is no end vertex
    void setPosy  (vector<float> Posy  ) ; // projection for different layers; filled only fir there is no end vertex
    void setPosz  (vector<float> Posz  ) ; // projection for different layers; filled only fir there is no end vertex

    float         Eta          () ; 
    float         Phi          () ; 
    float         Pt           () ; 
    float         Energy       () ; 
    float         Dvx          () ; 
    float         Dvy          () ; 
    float         Dvz          () ; 
    float         Ovx          () ; 
    float         Ovy          () ; 
    float         Ovz          () ; 
    float         Exx          () ; 
    float         Exy          () ; 
    int           Mother       () ; 
    float         Exphi        () ; 
    float         Exeta        () ; 
    float         Fbrem        () ; 
    int           PDGid        () ; 
    int           Gen          () ; 
    int           ReachedEE    () ; 
    bool          FromBeamPipe () ; 
    vector<float> Posx  () ; 
    vector<float> Posy  () ; 
    vector<float> Posz  () ; 


private:

    float         _Eta         ;
    float         _Phi         ;
    float         _Pt          ;
    float         _Energy      ;
    float         _Dvx         ;
    float         _Dvy         ;
    float         _Dvz         ;
    float         _Ovx         ;
    float         _Ovy         ;
    float         _Ovz         ;
    float         _Exx         ;
    float         _Exy         ;
    int           _Mother      ;
    float         _Exphi       ;
    float         _Exeta       ;
    float         _Fbrem       ;
    int           _Pid         ;
    int           _Gen         ;
    int           _ReachedEE   ;
    bool          _FromBeamPipe;
    vector<float> _Posx ;
    vector<float> _Posy ;
    vector<float> _Posz ;
    
    ClassDef(HGCgenpart, 1);

};

#endif

