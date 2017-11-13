#ifndef HGC_H
#define HGC_H 1

/* c/c++ */
#include <iostream>
#include <unordered_map>

/* mylibs */
#include "detId.h"
#include "HGCTC.h"
#include "HGCC2D.h"
#include "HGCROC.h"

/* ROOT */
#include "TMath.h"
#include "TChain.h"
#include "TTree.h"
#include "TCollection.h"
#include "TList.h"

using namespace std;

const unsigned Nlayers = 53; // layers' name starts at 1 


class HGC {

public:

    HGC(TList *fileList, bool flagTCs=true, bool flagC2D=true, int verboselevel=0); 
    ~HGC();

    bool areTCpresent()  { return _flagTCs; }
    bool areC2Dpresent() { return _flagC2D; }

    /* access the events */
    unsigned getEvents();
    void     getEvent( int evt );

    /* add */
    void addTC( HGCTC tc );
    void addC2D( HGCC2D c2d );

    /* get */
    HGCTC  getTC(unsigned ID);
    HGCC2D getC2D(unsigned ID);

    map<unsigned,HGCTC>  *getTCmap();
    map<unsigned,HGCC2D> *getC2Dmap();

    vector<HGCTC*>  getTC_layer( unsigned layer );
    vector<HGCC2D*> getC2D_layer( unsigned layer );
    vector<HGCROC> *getTD( unsigned layer );

    vector<HGCTC*> getTCall();
    vector<HGCC2D*> getC2Dall();
    void getTDall( vector<HGCROC> &data );

    void clear();

private:
 
    /* flags and verbose */
    int _verboselevel;
    bool _flagTCs, _flagC2D;

    /* TChain */
    TChain *_chain;
    int _evt;

    /* Branches */
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
    bool _missing__tc_z         ;
        

    // C2D
    int                       _cl_n            ;
    vector<float>            *_cl_pt       = 0 ;
    vector<float>            *_cl_energy   = 0 ;
    vector<float>            *_cl_eta      = 0 ;
    vector<float>            *_cl_phi      = 0 ;
    vector<float>            *_cl_x        = 0 ;
    vector<float>            *_cl_y        = 0 ;
    vector<float>            *_cl_z        = 0 ;
    vector<int>              *_cl_layer    = 0 ;
    vector<int>              *_cl_cells_n  = 0 ;
    vector<vector<unsigned>> *_cl_cells_id = 0 ;

    bool _missing__cl_n        ;
    bool _missing__cl_pt       ;
    bool _missing__cl_energy   ;
    bool _missing__cl_eta      ;
    bool _missing__cl_phi      ;
    bool _missing__cl_x        ;
    bool _missing__cl_y        ;
    bool _missing__cl_z        ;
    bool _missing__cl_layer    ;
    bool _missing__cl_cells_n  ;
    bool _missing__cl_cells_id ;

    
    /* mapping all the GENPART TC C2D C3D */
    map<unsigned,HGCTC>  _TCs;
    map<unsigned,HGCC2D> _C2Ds;  

    /* ordered preserved, needed fo storage purposes */
    vector<HGCTC*>  _TCtoStore;
    vector<HGCC2D*> _C2DtoStore;
    
    /* layer ordered */
    vector<HGCTC*>  _TC_layer[Nlayers];
    vector<HGCC2D*> _C2D_layer[Nlayers];
    vector<HGCROC> *_TD[Nlayers];

};

#endif





