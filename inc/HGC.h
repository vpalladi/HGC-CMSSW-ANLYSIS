#ifndef HGC_H
#define HGC_H 1

/* c/c++ */
#include <iostream>
#include <map>

/* mylibs */
#include "detId.h"
#include "HGCsubdet.h"
#include "HGCgen.h"
#include "HGCgenpart.h"

/* ROOT */
#include "TMath.h"
#include "TChain.h"
#include "TTree.h"
#include "TCollection.h"
#include "TList.h"

using namespace std;

//const unsigned Nlayers = 53; // must be nlayers+1, layers' name starts at 1 

class HGC {

public:

    HGC( TList *fileList, bool flagTCs=true, bool flagC2D=true, bool flagC3D=true, bool flagGen=true, bool flagGenpart=true, bool triggerLayersOnly=true, int verboselevel=0 ); 
    ~HGC();

    bool areTCpresent()       { return _flagTCs; }
    bool areC2Dpresent()      { return _flagC2D; }
    bool areC3Dpresent()      { return _flagC3D; } 
    bool areGenpartPresent()  { return _flagGenpart; }
    bool areGenPresent()      { return _flagGen; }

    /* access the events */
    unsigned getEvents();
    void     getEvent( int evt );

    /* access the subdet */
    // subdet : 0 positive z, 1 negative z; 
    // section: 0 EE, 1 FH, 2 BH, 3 ALL. 
    HGCsubdet* getSubdet(unsigned endcap, unsigned section) { return _subdet[endcap][section]; } 

    /* add */
    void addGen    ( HGCgen gen );
    void addGenpart( HGCgenpart genpart );
  
    /* get */

    vector<HGCgen*> getGenAll();
    vector<HGCgenpart*> getGenpartAll();

    void getTDall( vector<HGCROC> &data );

    vector<HGCgenpart*> getGenpartAllInPhiRegion(double minPhi, double maxPhi);

    void clear();

private:
 
    /* SUBDETS */
    HGCsubdet* _subdet[2][4];

    /* flags and verbose */
    int _verboselevel;
    bool _flagTCs, _flagC2D, _flagC3D, _flagGen, _flagGenpart;

    /* TChain */
    TChain *_chain;
    int _evt;

    /* Branches */
  
    // Generated Particles (reco)
    int             _gen_n            ;
    vector<int>    *_gen_id        = 0;
    vector<int>    *_gen_status    = 0;
    vector<float>  *_gen_energy    = 0;
    vector<float>  *_gen_pt        = 0;
    vector<float>  *_gen_eta       = 0;
    vector<float>  *_gen_phi       = 0;
    int             _gen_PUNumInt     ;
    float           _gen_TrueNumInt   ;

    bool _missing__gen_n              ;
    bool _missing__gen_id             ;
    bool _missing__gen_status         ;
    bool _missing__gen_energy         ;
    bool _missing__gen_pt             ;
    bool _missing__gen_eta            ;
    bool _missing__gen_phi            ;
    bool _missing__gen_PUNumInt       ;
    bool _missing__gen_TrueNumInt     ;

    // Generated Particles (tracks)
    vector<float> *_genpart_eta           = 0 ; // track momentum eta
    vector<float> *_genpart_phi           = 0 ; // track momentum phi
    vector<float> *_genpart_pt            = 0 ; // track momentum pt
    vector<float> *_genpart_energy        = 0 ; // track momentum energy
    vector<float> *_genpart_dvx           = 0 ; // track end x
    vector<float> *_genpart_dvy           = 0 ; // track end y
    vector<float> *_genpart_dvz           = 0 ; // track end z
    vector<float> *_genpart_ovx           = 0 ; // track origin x 
    vector<float> *_genpart_ovy           = 0 ; // track origin y 
    vector<float> *_genpart_ovz           = 0 ; // track origin z 
    vector<float> *_genpart_exx           = 0 ; // hit the HGCal x (layer 1)
    vector<float> *_genpart_exy           = 0 ; // hit the HGCal y (layer 1)
    vector<int>   *_genpart_mother        = 0 ; // index of the mother in the vectors    
    vector<float> *_genpart_exphi         = 0 ; // hit the HGCal phi (layer 1)
    vector<float> *_genpart_exeta         = 0 ; // hit the HGCal eta (layer 1)
    vector<float> *_genpart_fbrem         = 0 ; // percentage of bremstrahlung for photons
    vector<int>   *_genpart_pid           = 0 ; // PDG id
    vector<int>   *_genpart_gen           = 0 ; // gen part index
    vector<int>   *_genpart_reachedEE     = 0 ; // notReach = 0; outsideEESurface = 1; onEESurface = 2
    vector<bool>  *_genpart_fromBeamPipe  = 0 ; // ??? seems always true
    vector<vector<float>> *_genpart_posx  = 0; // projection for different layers; filled only fir there is no end vertex
    vector<vector<float>> *_genpart_posy  = 0; // projection for different layers; filled only fir there is no end vertex
    vector<vector<float>> *_genpart_posz  = 0; // projection for different layers; filled only fir there is no end vertex

    bool missing__genpart_eta ; 
    bool missing__genpart_phi ; 
    bool missing__genpart_pt  ; 
    bool missing__genpart_energy; 
    bool missing__genpart_dvx ; 
    bool missing__genpart_dvy ; 
    bool missing__genpart_dvz ; 
    bool missing__genpart_ovx ; 
    bool missing__genpart_ovy ; 
    bool missing__genpart_ovz ; 
    bool missing__genpart_exx ; 
    bool missing__genpart_exy ; 
    bool missing__genpart_mother; 
    bool missing__genpart_exphi; 
    bool missing__genpart_exeta; 
    bool missing__genpart_fbrem; 
    bool missing__genpart_pid ; 
    bool missing__genpart_gen ;   
    bool missing__genpart_reachedEE;   
    bool missing__genpart_fromBeamPipe;
    bool missing__genpart_posx; 
    bool missing__genpart_posy; 
    bool missing__genpart_posz; 

    // Trigger Cells
    int                 _tc_n;
    vector<unsigned>   *_tc_id        = 0;
    vector<int>        *_tc_subdet    = 0;
    vector<int>        *_tc_zside     = 0;
    vector<int>        *_tc_layer     = 0;
    vector<int>        *_tc_wafer     = 0;
    vector<int>        *_tc_wafertype = 0;
    vector<int>        *_tc_cell      = 0;
    vector<unsigned>   *_tc_data      = 0;
    vector<float>      *_tc_energy    = 0;
    vector<float>      *_tc_eta       = 0;
    vector<float>      *_tc_phi       = 0;
    vector<float>      *_tc_x         = 0;
    vector<float>      *_tc_y         = 0;
    vector<float>      *_tc_z         = 0;

    bool _missing__tc_n         ;
    bool _missing__tc_id        ;
    bool _missing__tc_subdet    ;
    bool _missing__tc_zside     ;
    bool _missing__tc_layer     ;
    bool _missing__tc_wafer     ;
    bool _missing__tc_wafertype ;
    bool _missing__tc_cell      ;
    bool _missing__tc_data      ;
    bool _missing__tc_energy    ;
    bool _missing__tc_eta       ;
    bool _missing__tc_phi       ;
    bool _missing__tc_x         ;
    bool _missing__tc_y         ;
    bool _missing__tc_z         ;

    // C2D
    int                       _cl_n           ;
    vector<float>            *_cl_pt       = 0;
    vector<float>            *_cl_energy   = 0;
    vector<float>            *_cl_eta      = 0;
    vector<float>            *_cl_phi      = 0;
    vector<float>            *_cl_x        = 0;
    vector<float>            *_cl_y        = 0;
    vector<float>            *_cl_z        = 0;
    vector<int>              *_cl_layer    = 0;
    vector<int>              *_cl_cells_n  = 0;
    vector<vector<unsigned>> *_cl_cells_id = 0;

    bool _missing__cl_n        ;
    bool _missing__cl_pt       ;
    bool _missing__cl_energy   ;
    bool _missing__cl_eta      ;
    bool _missing__cl_phi      ;
    bool _missing__cl_z        ;
    bool _missing__cl_layer    ;
    bool _missing__cl_cells_n  ;
    bool _missing__cl_cells_id ;

    // C3D
    vector<unsigned>         *_cl3d_id       = 0;
    vector<float>            *_cl3d_pt       = 0;
    vector<float>            *_cl3d_energy   = 0;
    vector<float>            *_cl3d_eta      = 0;
    vector<float>            *_cl3d_phi      = 0;      
    vector<vector<unsigned>> *_cl3d_clusters = 0;

    bool _missing__cl3d_id      ;
    bool _missing__cl3d_pt      ;
    bool _missing__cl3d_energy  ;
    bool _missing__cl3d_eta     ;
    bool _missing__cl3d_phi     ;      
    bool _missing__cl3d_clusters;

    /* mapping all the GEN and GENPART */
    map<unsigned,HGCgen>     _gen;  
    map<unsigned,HGCgenpart> _genpart;  
    
    /* ordered preserved, needed fo storage purposes */
    vector<HGCgen*>     _genVec;
    vector<HGCgenpart*> _genpartVec;

    /* layers positions */
    double _layerZ[53]; // layer id starts at 1

};

#endif





