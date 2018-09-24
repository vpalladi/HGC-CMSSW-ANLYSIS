#ifndef HGCsubdet_H
#define HGCsubdet_H 1

/* c++ */
#include <iostream>
#include <map>

/* mylibs */
#include "detId.h"
#include "HGCTC.h"
#include "HGCC2D.h"
#include "HGCC3D.h"
#include "HGCROC.h"
#include "HGCht.h"
#include "HGCnorm.h"
#include "HGCforest.h"
#include "HGCsingleC3D.h"
#include "HGCC3Dgen.h"
#include "HGCgenClu.h"
//#include "HGCpolarHisto.h"
#include "HGCpolarHisto_T.h"

/* ROOT */
#include "TMath.h"
#include "TChain.h"
#include "TTree.h"
#include "TCollection.h"
#include "TFile.h"

using namespace std;


class HGCsubdet {

public:

    HGCsubdet( unsigned endcap, unsigned section, int verboselevel=0 ); // EE=0; FH==1; BH==2; ALL=3
    ~HGCsubdet();

    /* add */
    template<class T>
    void add       ( T t );
    void addGen    ( HGCgen gen );

    /* get */
    template<class T>
    int get (unsigned ID, T &t);
    vector<HGCgen*> getGenAll();

    template<class T> 
    vector<const T*> getAll();

//    template<class T> 
//    vector<T*> getAllPtOrdered();

    template<class T> 
    map<unsigned,T> *getMap();

    void getTDall( vector<HGCROC> &data );

    template<class T>
    vector<const T*>  getAllInRegion (double minR, double maxR, double minPhi, double maxPhi) {
        vector<const T*> vecT = this->getAll<T>();
        vector<const T*> vecTselected;
        
        for(unsigned i=0; i<vecT.size(); i++){
            if( vecT.at(i)->Phi()>minPhi && vecT.at(i)->Phi()<maxPhi && 
                vecT.at(i)->r()>minR && vecT.at(i)->r()<maxR            ){
                vecTselected.push_back( vecT.at(i) );
            }
        }
        
        return vecTselected;
    }
    

    /* isPertinent */
    bool isPertinent(float z);

    /* NORM COO */
    HGCnorm getNormTransform_C2D( int nColsTanTheta, double tanThetaMin, double tanThetaMax, 
                                  int nRowsRho, double rhoMin, double rhoMax, 
                                  double minPhi=0, double maxPhi=2*TMath::Pi(), 
                                  bool wheightPt=false  
        );
    
    HGCnorm getNormTransform_C3D( int nColsTanTheta, double tanThetaMin, double tanThetaMax, 
                                  int nRowsRho, double rhoMin, double rhoMax, 
                                  double minPhi=0, double maxPhi=2*TMath::Pi(), 
                                  bool wheightPt=false
//                                  unsigned nNeighboursSearch=9, float radius=0.1, vector<HGCgen*> gens=vector<HGCgen*>()
        );

    HGCforest getNormTransform_C3Dforest( int nColsTanTheta, double tanThetaMin, double tanThetaMax, 
                                          int nRowsRho, double rhoMin, double rhoMax, 
                                          double minPhi=0, double maxPhi=2*TMath::Pi(), 
                                          bool wheightPt=false 
        );
    
    HGCsingleC3D getSingleC3D();
    
    HGCC3Dgen getGenC3D(double radius);

    template<class T> // HGCC2D or HGCTC
    HGCpolarHisto<T> getPolarFwC3D(double radius) {
        HGCpolarHisto<T> polarGrid( 36, 0.1, 0.52, 216, -TMath::Pi(), TMath::Pi() );
        vector<const T*> hits = getAll<T>();
        for(typename std::vector<const T*>::iterator hit=hits.begin(); hit!=hits.end(); hit++) 
            polarGrid.addPoint( *(*hit) );
        return polarGrid;    
    }
    
    /* HOUGH transform */
    HGCht getRhoZtransform_C2D( int nColsTanTheta, double tanThetaMin, double tanThetaMax, 
                                int nRowsRho, double rhoMin, double rhoMax, 
                                double zOffset, double slopeCorrection=1, 
                                double minPhi=0, double maxPhi=2*TMath::Pi(), 
                                bool wheightPt=false  );
    
    /* histos and graphs */
    void getC2DallInPhiRegion (double minPhi, double maxPhi, TGraph &graph);

    /* fill and save tree */
//    void fillTree();    
//    void saveTree(TFile* f);

    vector<HGCgenClu> getGenClusters( float ptCutGen, float clusteringNormRadius );
    
    HGCpolarHisto<HGCgenClu> getPolarGenClu( float ptCutGen, float clusteringNormRadius );

    /* clear */
    void clear();

private:
 
    unsigned nLayers() { return _layerZ.size(); }

    /* flags and verbose */
    unsigned _endcapId, _sectionId;
    int _verboselevel;
    
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
  

    /* mapping all the GENPART TC C2D C3D */
    map<unsigned,HGCTC>      _TCs;
    map<unsigned,HGCC2D>     _C2Ds;  
    map<unsigned,HGCC3D>     _C3Ds;  
    map<unsigned,HGCgen>     _gen;  
    vector<HGCgen*>          _genVec;

    /* ordered preserved, needed fo storage purposes */
    vector<const HGCTC*>      _TCvec;
    vector<const HGCC2D*>     _C2Dvec;
    vector<const HGCC3D*>     _C3Dvec;

    /* layer ordered */
    vector<const HGCTC*>  *_TC_layer;
    vector<const HGCC2D*> *_C2D_layer;
    vector<HGCROC>  **_TD;

    /* layers positions and valid */
    vector<double> _layerZ; // layer id starts at 1
  
    /* subdet trees: fill as you wish */
//    TTree* _tree;
//    vector<HGCC3D> _vC3Dforest;
//    vector<HGCC3D> _vC3Dnorm;
//    vector<HGCC3D> _vC3DnormNN;

};

#endif


