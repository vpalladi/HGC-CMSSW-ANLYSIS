
/* mylibs */
#include "HGC.h"

ClassImp(HGC)

HGC::HGC( TList *fileList, 
          bool flagTCs, bool flagC2D, bool flagC3D, bool flagGen, bool flagGenpart, 
          bool triggerLayersOnly, 
          int verboselevel ) {
    
    /* init */
    _evt = 0;
    _verboselevel = verboselevel;
    _flagTCs = flagTCs;
    _flagC2D = flagC2D;
    _flagC3D = flagC3D;
    _flagGen = flagGen;
    _flagGenpart = flagGenpart;
    _triggerLayersOnly = triggerLayersOnly;

    /* Get the Chain */
    _chain = new TChain("hgcalTriggerNtuplizer/HGCalTriggerNtuple");
    TIter next(fileList);
    while ( TObject *file = next() ){
        if( _verboselevel >= 3 )
            cout << " HGC >> Adding file '" << ((TObjString*)file)->GetString() << "' to the chain." << endl; 
        _chain->Add( ((TObjString*)file)->GetString() );
    }
    
    if( _verboselevel >= 3 )
        cout << " HGC >> Chain contains " << _chain->GetEntries() << " events." << endl; 

    /* initialize the subdets */
    // positive z
    _subdet[0][0] = new HGCsubdet(0, 0, _verboselevel ); // EE
    _subdet[0][1] = new HGCsubdet(0, 1, _verboselevel ); // FH
    _subdet[0][2] = new HGCsubdet(0, 2, _verboselevel ); // BH
    _subdet[0][3] = new HGCsubdet(0, 3, _verboselevel ); // all
    // negative z                       
    _subdet[1][0] = new HGCsubdet(1, 0, _verboselevel ); // EE
    _subdet[1][1] = new HGCsubdet(1, 1, _verboselevel ); // FH
    _subdet[1][2] = new HGCsubdet(1, 2, _verboselevel ); // BH
    _subdet[1][3] = new HGCsubdet(1, 3, _verboselevel ); // all

    // Generated Particles (reco)
    if( _flagGen ){
        if( _chain->GetBranchStatus("gen_n"                ) ) { _missing__gen_n               = false; _chain->SetBranchAddress("gen_n"               , &_gen_n               ); } else { _missing__gen_n              = true; cout << " HGC >> leaf GEN 'n         ' not found." << endl; }
        if( _chain->GetBranchStatus("gen_pdgid"            ) ) { _missing__gen_id              = false; _chain->SetBranchAddress("gen_pdgid"           , &_gen_id              ); } else { _missing__gen_id             = true; cout << " HGC >> leaf GEN 'id        ' not found." << endl; }
        if( _chain->GetBranchStatus("gen_status"           ) ) { _missing__gen_status          = false; _chain->SetBranchAddress("gen_status"          , &_gen_status          ); } else { _missing__gen_status         = true; cout << " HGC >> leaf GEN 'status    ' not found." << endl; }
        if( _chain->GetBranchStatus("gen_energy"           ) ) { _missing__gen_energy          = false; _chain->SetBranchAddress("gen_energy"          , &_gen_energy          ); } else { _missing__gen_energy         = true; cout << " HGC >> leaf GEN 'energy    ' not found." << endl; }
        if( _chain->GetBranchStatus("gen_pt"               ) ) { _missing__gen_pt              = false; _chain->SetBranchAddress("gen_pt"              , &_gen_pt              ); } else { _missing__gen_pt             = true; cout << " HGC >> leaf GEN 'pt        ' not found." << endl; }
        if( _chain->GetBranchStatus("gen_eta"              ) ) { _missing__gen_eta             = false; _chain->SetBranchAddress("gen_eta"             , &_gen_eta             ); } else { _missing__gen_eta            = true; cout << " HGC >> leaf GEN 'eta       ' not found." << endl; }
        if( _chain->GetBranchStatus("gen_phi"              ) ) { _missing__gen_phi             = false; _chain->SetBranchAddress("gen_phi"             , &_gen_phi             ); } else { _missing__gen_phi            = true; cout << " HGC >> leaf GEN 'phi       ' not found." << endl; }
        if( _chain->GetBranchStatus("gen_PUNumInt"         ) ) { _missing__gen_PUNumInt        = false; _chain->SetBranchAddress("gen_PUNumInt"        , &_gen_PUNumInt        ); } else { _missing__gen_PUNumInt       = true; cout << " HGC >> leaf GEN 'PUNumInt  ' not found." << endl; }
        if( _chain->GetBranchStatus("gen_TrueNumInt"       ) ) { _missing__gen_TrueNumInt      = false; _chain->SetBranchAddress("gen_TrueNumInt"      , &_gen_TrueNumInt      ); } else { _missing__gen_TrueNumInt     = true; cout << " HGC >> leaf GEN 'TrueNumInt' not found." << endl; }
    }

    // Generated Particles (track)
    if( flagGenpart ){
        if( _chain->GetBranchStatus("genpart_eta"          ) ) { _missing__genpart_eta         = false; _chain->SetBranchAddress("genpart_eta"         , &_genpart_eta         ); } else { _missing__genpart_eta        = true; cout << " HGC >> leaf genpart 'eta        ' not found." << endl; }
        if( _chain->GetBranchStatus("genpart_phi"          ) ) { _missing__genpart_phi         = false; _chain->SetBranchAddress("genpart_phi"         , &_genpart_phi         ); } else { _missing__genpart_phi        = true; cout << " HGC >> leaf genpart 'phi        ' not found." << endl; }
        if( _chain->GetBranchStatus("genpart_pt"           ) ) { _missing__genpart_pt          = false; _chain->SetBranchAddress("genpart_pt"          , &_genpart_pt          ); } else { _missing__genpart_pt         = true; cout << " HGC >> leaf genpart 'pt         ' not found." << endl; }
        if( _chain->GetBranchStatus("genpart_energy"       ) ) { _missing__genpart_energy      = false; _chain->SetBranchAddress("genpart_energy"      , &_genpart_energy      ); } else { _missing__genpart_energy     = true; cout << " HGC >> leaf genpart 'energy     ' not found." << endl; }
        if( _chain->GetBranchStatus("genpart_dvx"          ) ) { _missing__genpart_dvx         = false; _chain->SetBranchAddress("genpart_dvx"         , &_genpart_dvx         ); } else { _missing__genpart_dvx        = true; cout << " HGC >> leaf genpart 'dvx        ' not found." << endl; }
        if( _chain->GetBranchStatus("genpart_dvy"          ) ) { _missing__genpart_dvy         = false; _chain->SetBranchAddress("genpart_dvy"         , &_genpart_dvy         ); } else { _missing__genpart_dvy        = true; cout << " HGC >> leaf genpart 'dvy        ' not found." << endl; }
        if( _chain->GetBranchStatus("genpart_dvz"          ) ) { _missing__genpart_dvz         = false; _chain->SetBranchAddress("genpart_dvz"         , &_genpart_dvz         ); } else { _missing__genpart_dvz        = true; cout << " HGC >> leaf genpart 'dvz        ' not found." << endl; }
        if( _chain->GetBranchStatus("genpart_ovx"          ) ) { _missing__genpart_ovx         = false; _chain->SetBranchAddress("genpart_ovx"         , &_genpart_ovx         ); } else { _missing__genpart_ovx        = true; cout << " HGC >> leaf genpart 'ovx        ' not found." << endl; }
        if( _chain->GetBranchStatus("genpart_ovy"          ) ) { _missing__genpart_ovy         = false; _chain->SetBranchAddress("genpart_ovy"         , &_genpart_ovy         ); } else { _missing__genpart_ovy        = true; cout << " HGC >> leaf genpart 'ovy        ' not found." << endl; }
        if( _chain->GetBranchStatus("genpart_ovz"          ) ) { _missing__genpart_ovz         = false; _chain->SetBranchAddress("genpart_ovz"         , &_genpart_ovz         ); } else { _missing__genpart_ovz        = true; cout << " HGC >> leaf genpart 'ovz        ' not found." << endl; }
        if( _chain->GetBranchStatus("genpart_exx"          ) ) { _missing__genpart_exx         = false; _chain->SetBranchAddress("genpart_exx"         , &_genpart_exx         ); } else { _missing__genpart_exx        = true; cout << " HGC >> leaf genpart 'exx        ' not found." << endl; }
        if( _chain->GetBranchStatus("genpart_exy"          ) ) { _missing__genpart_exy         = false; _chain->SetBranchAddress("genpart_exy"         , &_genpart_exy         ); } else { _missing__genpart_exy        = true; cout << " HGC >> leaf genpart 'exy        ' not found." << endl; }
        if( _chain->GetBranchStatus("genpart_mother"       ) ) { _missing__genpart_mother      = false; _chain->SetBranchAddress("genpart_mother"      , &_genpart_mother      ); } else { _missing__genpart_mother     = true; cout << " HGC >> leaf genpart 'mother     ' not found." << endl; }
        if( _chain->GetBranchStatus("genpart_exphi"        ) ) { _missing__genpart_exphi       = false; _chain->SetBranchAddress("genpart_exphi"       , &_genpart_exphi       ); } else { _missing__genpart_exphi      = true; cout << " HGC >> leaf genpart 'exphi      ' not found." << endl; }
        if( _chain->GetBranchStatus("genpart_exeta"        ) ) { _missing__genpart_exeta       = false; _chain->SetBranchAddress("genpart_exeta"       , &_genpart_exeta       ); } else { _missing__genpart_exeta      = true; cout << " HGC >> leaf genpart 'exeta      ' not found." << endl; }
        if( _chain->GetBranchStatus("genpart_fbrem"        ) ) { _missing__genpart_fbrem       = false; _chain->SetBranchAddress("genpart_fbrem"       , &_genpart_fbrem       ); } else { _missing__genpart_fbrem      = true; cout << " HGC >> leaf genpart 'fbrem      ' not found." << endl; }
        if( _chain->GetBranchStatus("genpart_pid"          ) ) { _missing__genpart_pid         = false; _chain->SetBranchAddress("genpart_pid"         , &_genpart_pid         ); } else { _missing__genpart_pid        = true; cout << " HGC >> leaf genpart 'pid        ' not found." << endl; }
        if( _chain->GetBranchStatus("genpart_gen"          ) ) { _missing__genpart_gen         = false; _chain->SetBranchAddress("genpart_gen"         , &_genpart_gen         ); } else { _missing__genpart_gen        = true; cout << " HGC >> leaf genpart 'gen        ' not found." << endl; }  
        if( _chain->GetBranchStatus("genpart_reachedEE"    ) ) { _missing__genpart_reachedEE   = false; _chain->SetBranchAddress("genpart_reachedEE"   , &_genpart_reachedEE   ); } else { _missing__genpart_reachedEE  = true; cout << " HGC >> leaf genpart 'reachedEE  ' not found." << endl; }  
        if( _chain->GetBranchStatus("genpart_fromBeamPipe" ) ) { _missing__genpart_fromBeamPipe= false; _chain->SetBranchAddress("genpart_fromBeamPipe", &_genpart_fromBeamPipe); } else { _missing__genpart_fromBeamPipe=true; cout << " HGC >> leaf genpart 'fromBeamPipe' not found." << endl;}
        if( _chain->GetBranchStatus("genpart_posx"         ) ) { _missing__genpart_posx        = false; _chain->SetBranchAddress("genpart_posx"        , &_genpart_posx        ); } else { _missing__genpart_posx       = true; cout << " HGC >> leaf genpart 'posx       ' not found." << endl; }
        if( _chain->GetBranchStatus("genpart_posy"         ) ) { _missing__genpart_posy        = false; _chain->SetBranchAddress("genpart_posy"        , &_genpart_posy        ); } else { _missing__genpart_posy       = true; cout << " HGC >> leaf genpart 'posy       ' not found." << endl; }
        if( _chain->GetBranchStatus("genpart_posz"         ) ) { _missing__genpart_posz        = false; _chain->SetBranchAddress("genpart_posz"        , &_genpart_posz        ); } else { _missing__genpart_posz       = true; cout << " HGC >> leaf genpart 'posz       ' not found." << endl; }
    }

    // Trigger cells
    if( _flagTCs ){
        if( _chain->GetBranchStatus( "tc_n"            ) ) { _missing__tc_n          = false; _chain->SetBranchAddress("tc_n"          , &_tc_n           ); } else { _missing__tc_n          = true; cout << " HGC >> leaf TC 'n        ' not found." << endl; }
        if( _chain->GetBranchStatus( "tc_id"           ) ) { _missing__tc_id         = false; _chain->SetBranchAddress("tc_id"         , &_tc_id          ); } else { _missing__tc_id         = true; cout << " HGC >> leaf TC 'id       ' not found." << endl; }
        if( _chain->GetBranchStatus( "tc_subdet"       ) ) { _missing__tc_subdet     = false; _chain->SetBranchAddress("tc_subdet"     , &_tc_subdet      ); } else { _missing__tc_subdet     = true; cout << " HGC >> leaf TC 'subdet   ' not found." << endl; }
        if( _chain->GetBranchStatus( "tc_zside"        ) ) { _missing__tc_zside      = false; _chain->SetBranchAddress("tc_zside"      , &_tc_zside       ); } else { _missing__tc_zside      = true; cout << " HGC >> leaf TC 'zside    ' not found." << endl; }
        if( _chain->GetBranchStatus( "tc_layer"        ) ) { _missing__tc_layer      = false; _chain->SetBranchAddress("tc_layer"      , &_tc_layer       ); } else { _missing__tc_layer      = true; cout << " HGC >> leaf TC 'layer    ' not found." << endl; }
        if( _chain->GetBranchStatus( "tc_wafer"        ) ) { _missing__tc_wafer      = false; _chain->SetBranchAddress("tc_wafer"      , &_tc_wafer       ); } else { _missing__tc_wafer      = true; cout << " HGC >> leaf TC 'wafer    ' not found." << endl; }
        if( _chain->GetBranchStatus( "tc_wafertype"    ) ) { _missing__tc_wafertype  = false; _chain->SetBranchAddress("tc_wafertype"  , &_tc_wafertype   ); } else { _missing__tc_wafertype  = true; cout << " HGC >> leaf TC 'wafertype' not found." << endl; }
        if( _chain->GetBranchStatus( "tc_cell"         ) ) { _missing__tc_cell       = false; _chain->SetBranchAddress("tc_cell"       , &_tc_cell        ); } else { _missing__tc_cell       = true; cout << " HGC >> leaf TC 'cell     ' not found." << endl; }
        if( _chain->GetBranchStatus( "tc_data"         ) ) { _missing__tc_data       = false; _chain->SetBranchAddress("tc_data"       , &_tc_data        ); } else { _missing__tc_data       = true; cout << " HGC >> leaf TC 'data     ' not found." << endl; }
        if( _chain->GetBranchStatus( "tc_energy"       ) ) { _missing__tc_energy     = false; _chain->SetBranchAddress("tc_energy"     , &_tc_energy      ); } else { _missing__tc_energy     = true; cout << " HGC >> leaf TC 'energy   ' not found." << endl; }
        if( _chain->GetBranchStatus( "tc_eta"          ) ) { _missing__tc_eta        = false; _chain->SetBranchAddress("tc_eta"        , &_tc_eta         ); } else { _missing__tc_eta        = true; cout << " HGC >> leaf TC 'eta      ' not found." << endl; }
        if( _chain->GetBranchStatus( "tc_phi"          ) ) { _missing__tc_phi        = false; _chain->SetBranchAddress("tc_phi"        , &_tc_phi         ); } else { _missing__tc_phi        = true; cout << " HGC >> leaf TC 'phi      ' not found." << endl; }
        if( _chain->GetBranchStatus( "tc_x"            ) ) { _missing__tc_x          = false; _chain->SetBranchAddress("tc_x"          , &_tc_x           ); } else { _missing__tc_x          = true; cout << " HGC >> leaf TC 'x        ' not found." << endl; }
        if( _chain->GetBranchStatus( "tc_y"            ) ) { _missing__tc_y          = false; _chain->SetBranchAddress("tc_y"          , &_tc_y           ); } else { _missing__tc_y          = true; cout << " HGC >> leaf TC 'y        ' not found." << endl; }
        if( _chain->GetBranchStatus( "tc_z"            ) ) { _missing__tc_z          = false; _chain->SetBranchAddress("tc_z"          , &_tc_z           ); } else { _missing__tc_z          = true; cout << " HGC >> leaf TC 'z        ' not found." << endl; }
                                                                                                                                                                                         
    }

    // C2D                                                                                                                                                                               
    if( _flagC2D ) {                                                                                                                                                                     
        if( _chain->GetBranchStatus( "cl_n"            ) ) { _missing__cl_n          = false; _chain->SetBranchAddress("cl_n"          , &_cl_n           ); } else { _missing__cl_n          = true; cout << " HGC >> leaf C2D 'n       ' not found." << endl; }
        if( _chain->GetBranchStatus( "cl_id"           ) ) { _missing__cl_id         = false; _chain->SetBranchAddress("cl_id"         , &_cl_id          ); } else { _missing__cl_id         = true; cout << " HGC >> leaf C2D 'id      ' not found." << endl; }
        if( _chain->GetBranchStatus( "cl_pt"           ) ) { _missing__cl_pt         = false; _chain->SetBranchAddress("cl_pt"         , &_cl_pt          ); } else { _missing__cl_pt         = true; cout << " HGC >> leaf C2D 'pt      ' not found." << endl; }                                                       
        if( _chain->GetBranchStatus( "cl_energy"       ) ) { _missing__cl_energy     = false; _chain->SetBranchAddress("cl_energy"     , &_cl_energy      ); } else { _missing__cl_energy     = true; cout << " HGC >> leaf C2D 'energy  ' not found." << endl; }
        if( _chain->GetBranchStatus( "cl_eta"          ) ) { _missing__cl_eta        = false; _chain->SetBranchAddress("cl_eta"        , &_cl_eta         ); } else { _missing__cl_eta        = true; cout << " HGC >> leaf C2D 'eta     ' not found." << endl; }                                                              
        if( _chain->GetBranchStatus( "cl_phi"          ) ) { _missing__cl_phi        = false; _chain->SetBranchAddress("cl_phi"        , &_cl_phi         ); } else { _missing__cl_phi        = true; cout << " HGC >> leaf C2D 'phi     ' not found." << endl; }
        if( _chain->GetBranchStatus( "cl_z"            ) ) { _missing__cl_z          = false; _chain->SetBranchAddress("cl_z"          , &_cl_z           ); } else { _missing__cl_z          = true; cout << " HGC >> leaf C2D 'z       ' not found." << endl; }
        if( _chain->GetBranchStatus( "cl_layer"        ) ) { _missing__cl_layer      = false; _chain->SetBranchAddress("cl_layer"      , &_cl_layer       ); } else { _missing__cl_layer      = true; cout << " HGC >> leaf C2D 'layer   ' not found." << endl; }
        if( _chain->GetBranchStatus( "cl_cells_n"      ) ) { _missing__cl_cells_n    = false; _chain->SetBranchAddress("cl_cells_n"    , &_cl_cells_n     ); } else { _missing__cl_cells_n    = true; cout << " HGC >> leaf C2D 'cells_n ' not found." << endl; }
        if( _chain->GetBranchStatus( "cl_cells_id"     ) ) { _missing__cl_cells_id   = false; _chain->SetBranchAddress("cl_cells_id"   , &_cl_cells_id    ); } else { _missing__cl_cells_id   = true; cout << " HGC >> leaf C2D 'cells_id' not found." << endl; }
        
    } 
                                                                    
    //C3D
    if( _flagC3D ) {       
        if( _chain->GetBranchStatus( "cl3d_id"         ) ) { _missing__cl3d_id       = false; _chain->SetBranchAddress("cl3d_id"        , &_cl3d_id       ); } else { _missing__cl3d_id       = true; cout << " HGC >> leaf C3D 'id      ' not found." << endl; }
        if( _chain->GetBranchStatus( "cl3d_pt"         ) ) { _missing__cl3d_pt       = false; _chain->SetBranchAddress("cl3d_pt"        , &_cl3d_pt       ); } else { _missing__cl3d_pt       = true; cout << " HGC >> leaf C3D 'pt      ' not found." << endl; }
        if( _chain->GetBranchStatus( "cl3d_energy"     ) ) { _missing__cl3d_energy   = false; _chain->SetBranchAddress("cl3d_energy"    , &_cl3d_energy   ); } else { _missing__cl3d_energy   = true; cout << " HGC >> leaf C3D 'energy  ' not found." << endl; }
        if( _chain->GetBranchStatus( "cl3d_eta"        ) ) { _missing__cl3d_eta      = false; _chain->SetBranchAddress("cl3d_eta"       , &_cl3d_eta      ); } else { _missing__cl3d_eta      = true; cout << " HGC >> leaf C3D 'eta     ' not found." << endl; }
        if( _chain->GetBranchStatus( "cl3d_phi"        ) ) { _missing__cl3d_phi      = false; _chain->SetBranchAddress("cl3d_phi"       , &_cl3d_phi      ); } else { _missing__cl3d_phi      = true; cout << " HGC >> leaf C3D 'phi     ' not found." << endl; }
        if( _chain->GetBranchStatus( "cl3d_clusters_id") ) { _missing__cl3d_clusters = false; _chain->SetBranchAddress("cl3d_clusters_id", &_cl3d_clusters); } else { _missing__cl3d_clusters = true; cout << " HGC >> leaf C3D 'clusters' not found." << endl; }
        if( _chain->GetBranchStatus( "cl3d_firstlayer" ) ) { _missing__cl3d_firstlayer= false; _chain->SetBranchAddress("cl3d_firstlayer", &_cl3d_firstlayer); } else { _missing__cl3d_firstlayer = true; cout << " HGC >> leaf C3D 'firstlayer' not found." << endl; }
        if( _chain->GetBranchStatus( "cl3d_maxlayer"   ) ) { _missing__cl3d_maxlayer = false; _chain->SetBranchAddress("cl3d_maxlayer"  , &_cl3d_maxlayer ); } else { _missing__cl3d_maxlayer = true; cout << " HGC >> leaf C3D 'maxlayer' not found." << endl; }
    }

}

HGC::~HGC() { 

    if( _chain        != NULL ) delete _chain       ;
    if( _subdet[0][0] != NULL ) delete _subdet[0][0]; // positive z EE
    if( _subdet[0][1] != NULL ) delete _subdet[0][1]; // positive z FH
    if( _subdet[0][2] != NULL ) delete _subdet[0][2]; // positive z BH
    if( _subdet[0][3] != NULL ) delete _subdet[0][3]; // positive z all
    if( _subdet[1][0] != NULL ) delete _subdet[1][0]; // negative z EE
    if( _subdet[1][1] != NULL ) delete _subdet[1][1]; // negative z FH
    if( _subdet[1][2] != NULL ) delete _subdet[1][2]; // negative z BH
    if( _subdet[1][3] != NULL ) delete _subdet[1][3]; // negative z all

};
    

HGCsubdet* HGC::getSubdet(unsigned endcap, unsigned section) { 

    return _subdet[endcap][section]; 

} 


/* boolen vars */
bool HGC::areTCpresent()       { return _flagTCs; }
bool HGC::areC2Dpresent()      { return _flagC2D; }
bool HGC::areC3Dpresent()      { return _flagC3D; } 
bool HGC::areGenpartPresent()  { return _flagGenpart; }
bool HGC::areGenPresent()      { return _flagGen; }

/* get number of events */
unsigned HGC::getEvents(){

    return _chain->GetEntries();

}


/* read the event evt in memeory */
void HGC::getEvent( int evt ){
        
    /* clear detector before getting the event */
    this->clear();

    /* Get Entry */
    _chain->GetEntry( evt );        

    /*************************/
    /* looping over Gen */
    if(_flagGen ){
      
        HGCgen gen;
        for( int igen = 0; igen<_gen_n; igen++ ){

            gen.setId(igen);
            if( !_missing__gen_id         ) gen.setPDGid (_gen_id     ->at(igen) );  
            if( !_missing__gen_status     ) gen.setStatus(_gen_status ->at(igen) );  
            if( !_missing__gen_energy     ) gen.setEnergy(_gen_energy ->at(igen) );  
            if( !_missing__gen_pt         ) gen.setPt    (_gen_pt     ->at(igen) );  
            if( !_missing__gen_eta        ) gen.setEta   (_gen_eta    ->at(igen) );  
            if( !_missing__gen_phi        ) gen.setPhi   (_gen_phi    ->at(igen) );  
            
            this->addGen( gen );
            this->getSubdet( gen.getEndcapId(), 0 )->addGen( gen );
            this->getSubdet( gen.getEndcapId(), 1 )->addGen( gen );
            this->getSubdet( gen.getEndcapId(), 2 )->addGen( gen );
            this->getSubdet( gen.getEndcapId(), 3 )->addGen( gen );
        }
    }

    /*************************/
    /* looping over Gen Part */
    if(_flagGenpart ){

        
        unsigned ngenpart=0;
        if( !_missing__genpart_eta ) { 
            ngenpart=_genpart_eta->size() ; 
        }
        else {
            cout << " HGC >>> !!! Warning !!! genpart branch eta missing " << endl;
        }

        HGCgenpart genpart;
        int uniqueId = 0;
        for( unsigned igenpart = 0; igenpart<ngenpart; igenpart++ ){
            
            genpart.setUniqueId ( uniqueId++ );   
            
            if( !_missing__genpart_eta         ) genpart.setEta          (_genpart_eta         ->at(igenpart) ); 
            if( !_missing__genpart_phi         ) genpart.setPhi          (_genpart_phi         ->at(igenpart) ); 
            if( !_missing__genpart_pt          ) genpart.setPt           (_genpart_pt          ->at(igenpart) ); 
            if( !_missing__genpart_energy      ) genpart.setEnergy       (_genpart_energy      ->at(igenpart) ); 
            if( !_missing__genpart_dvx         ) genpart.setDvx          (_genpart_dvx         ->at(igenpart) ); 
            if( !_missing__genpart_dvy         ) genpart.setDvy          (_genpart_dvy         ->at(igenpart) ); 
            if( !_missing__genpart_dvz         ) genpart.setDvz          (_genpart_dvz         ->at(igenpart) ); 
            if( !_missing__genpart_ovx         ) genpart.setOvx          (_genpart_ovx         ->at(igenpart) ); 
            if( !_missing__genpart_ovy         ) genpart.setOvy          (_genpart_ovy         ->at(igenpart) ); 
            if( !_missing__genpart_ovz         ) genpart.setOvz          (_genpart_ovz         ->at(igenpart) ); 
            if( !_missing__genpart_exx         ) genpart.setExx          (_genpart_exx         ->at(igenpart) ); 
            if( !_missing__genpart_exy         ) genpart.setExy          (_genpart_exy         ->at(igenpart) ); 
            if( !_missing__genpart_mother      ) genpart.setMother       (_genpart_mother      ->at(igenpart) ); 
            if( !_missing__genpart_exphi       ) genpart.setExphi        (_genpart_exphi       ->at(igenpart) ); 
            if( !_missing__genpart_exeta       ) genpart.setExeta        (_genpart_exeta       ->at(igenpart) ); 
            if( !_missing__genpart_fbrem       ) genpart.setFbrem        (_genpart_fbrem       ->at(igenpart) ); 
            if( !_missing__genpart_pid         ) genpart.setPDGid        (_genpart_pid         ->at(igenpart) ); 
            if( !_missing__genpart_gen         ) genpart.setGen          (_genpart_gen         ->at(igenpart) );    
            if( !_missing__genpart_reachedEE   ) genpart.setReachedEE    (_genpart_reachedEE   ->at(igenpart) ); 
            if( !_missing__genpart_fromBeamPipe) genpart.setFromBeamPipe (_genpart_fromBeamPipe->at(igenpart) ); 
            if( !_missing__genpart_posx        ) genpart.setPosx         (_genpart_posx        ->at(igenpart) ); 
            if( !_missing__genpart_posy        ) genpart.setPosy         (_genpart_posy        ->at(igenpart) ); 
            if( !_missing__genpart_posz        ) genpart.setPosz         (_genpart_posz        ->at(igenpart) ); 
            
            this->addGenpart( genpart );
        }
    }


    /********************/
    /* looping over TCs */
    if( _flagTCs ){
     
        for( int itc=0; itc<_tc_n; itc++ ){
            
	  HGCTC tc;
	  if( !_missing__tc_id       ) tc.setId        ( _tc_id       ->at( itc ) );
	  if( !_missing__tc_subdet   ) tc.setSubdet    ( _tc_subdet   ->at( itc ) );            
	  if( !_missing__tc_zside    ) tc.setZside     ( _tc_zside    ->at( itc ) );
	  if( !_missing__tc_layer    ) tc.setLayer     ( _tc_layer    ->at( itc ) );
	  if( !_missing__tc_wafer    ) tc.setWafer     ( _tc_wafer    ->at( itc ) );
	  if( !_missing__tc_wafertype) tc.setWaferType ( _tc_wafertype->at( itc ) );
	  if( !_missing__tc_cell     ) tc.setCell      ( _tc_cell     ->at( itc ) );
	  if( !_missing__tc_data     ) tc.setData      ( _tc_data     ->at( itc ) );
	  if( !_missing__tc_energy   ) tc.setEnergy    ( _tc_energy   ->at( itc ) );
	  if( !_missing__tc_eta      ) tc.setEta       ( _tc_eta      ->at( itc ) );
	  if( !_missing__tc_phi      ) tc.setPhi       ( _tc_phi      ->at( itc ) );
	  if( !_missing__tc_x        ) tc.setX         ( _tc_x        ->at( itc ) );
          if( !_missing__tc_y        ) tc.setY         ( _tc_y        ->at( itc ) );
          if( !_missing__tc_z        ) tc.setZ         ( _tc_z        ->at( itc ) );

          /* fill detector data */
          int isection = tc.subdet()-3; 
          int iendcap = tc.zside()==-1 ? 1 : 0;

          /* if NOT trigger layer continue */
          //if( !( !_triggerLayersOnly || HGCgeom::instance()->layerZisTriggerLayer(iendcap, tc.layer()) ) )
          //    continue;

          _subdet[iendcap][isection]->add<HGCTC>( tc );
          _subdet[iendcap][3]->add<HGCTC>( tc ); // always to No 3 -> full depth info
          
        }// end TCs LOOP
        
    }// end TCs

    /*****************/
    /* Loop over C2D */
    if( _flagC2D ) {

        if( !_missing__cl_n ){

            if( _verboselevel>1 )
                cout << " HGC >> There are " << _cl_n << " C2D." << endl;
            
            for(int iclu=0; iclu<_cl_n; iclu++){
                
                HGCC2D c2d;
                if( !_missing__cl_id       ) c2d.setId     ( _cl_id      ->at(iclu) ); 
                if( !_missing__cl_pt       ) c2d.setPt     ( _cl_pt      ->at(iclu) ); 
                if( !_missing__cl_energy   ) c2d.setEnergy ( _cl_energy  ->at(iclu) ); 
                if( !_missing__cl_eta      ) c2d.setEta    ( _cl_eta     ->at(iclu) ); 
                if( !_missing__cl_phi      ) c2d.setPhi    ( _cl_phi     ->at(iclu) ); 
                if( !_missing__cl_layer    ) c2d.setLayer  ( _cl_layer   ->at(iclu) ); 
                if( !_missing__cl_cells_id ) c2d.setCells  ( _cl_cells_id->at(iclu) ); 	    

                c2d.setZ( HGCgeom::instance()->layerZ(c2d.getEndcapId(), c2d.layer()) );

                ROOT::Math::RhoEtaPhiPoint c2dpos( c2d.z()*TMath::Tan(2*TMath::ATan(TMath::Exp( c2d.Eta() ))), c2d.Eta(), c2d.Phi() );
                c2d.setX( c2dpos.X() );
                c2d.setY( c2dpos.Y() );
                                
//                if( !( !_triggerLayersOnly || HGCgeom::instance()->layerZisTriggerLayer(c2d.getEndcapId(), c2d.layer()) ) )
//                    continue;

                int subdet = -1;
                
                if( _flagTCs ){
                    
                    HGCTC tc0;
                    _subdet[c2d.getEndcapId()][3]->get<HGCTC>( c2d.cells()[0], tc0 );
                    subdet = tc0.subdet();
//                    c2d.setId( tc0.id() ) ;
                    
                }
                c2d.setSubdet( subdet );
                
                /* fill the detector */
                int iendcap  = c2d.getEndcapId();
                
                int isection = c2d.getSectionId();

                _subdet[iendcap][isection]->add<HGCC2D>( c2d );
                
                _subdet[iendcap][3]->add<HGCC2D>( c2d ); // always to the number 3 -> all depth
                //HGCC2D c2d_check;

                //_subdet[iendcap][3]->get<HGCC2D>( c2d.id(), c2d_check );

            }

        }

    }// end C2D


    /*****************/
    /* Loop over C3D */
    if( _flagC3D ) {

        unsigned nc3d = 0;
        if( !_missing__cl3d_id ) 
            nc3d = _cl3d_id->size();
        else if( !_missing__cl3d_pt )
            nc3d = _cl3d_pt->size();
        else
            std::cout << " HGC >>> !!!WARNING!!! No C3D loaded. "<< std::endl;
        for(unsigned ic3d=0; ic3d<nc3d; ic3d++){
            
            HGCC3D c3d;
	    if( !_missing__cl3d_id        ) c3d.setId      ( _cl3d_id      ->at(ic3d) );
            if( !_missing__cl3d_pt        ) c3d.setPt      ( _cl3d_pt      ->at(ic3d) );
	    if( !_missing__cl3d_energy    ) c3d.setEnergy  ( _cl3d_energy  ->at(ic3d) );
	    if( !_missing__cl3d_eta       ) c3d.setEta     ( _cl3d_eta     ->at(ic3d) );
	    if( !_missing__cl3d_phi       ) c3d.setPhi     ( _cl3d_phi     ->at(ic3d) );
	    if( !_missing__cl3d_clusters  ) c3d.setClusters( _cl3d_clusters->at(ic3d) ); 
            if( !_missing__cl3d_firstlayer) c3d.setFirstLayer( _cl3d_firstlayer->at(ic3d) ); 
            if( !_missing__cl3d_maxlayer  ) c3d.setMaxLayer( _cl3d_maxlayer->at(ic3d) ); 

            double layerZ = HGCgeom::instance()->layerZ( c3d.getEndcapId(), 1 );
            double projX =  c3d.getZprojection( layerZ ).X() / c3d.getZprojection( layerZ ).Z();
            double projY =  c3d.getZprojection( layerZ ).Y() / c3d.getZprojection( layerZ ).Z();
            
            c3d.setXnorm( projX );
            c3d.setYnorm( projY );
            
	    if(_flagC2D){
                
            //temporary_comment     vector<unsigned> cl3d_cells;
            //temporary_comment     for(auto iclu : c3d.clusters()){
            //temporary_comment         HGCC2D c2d = this->getC2D(iclu);
            //temporary_comment         vector<unsigned> TCs = c2d.cells();
            //temporary_comment         cl3d_cells.insert( cl3d_cells.end(), TCs.begin(), TCs.end() );
            //temporary_comment     }
            //temporary_comment     c3d.setCells(cl3d_cells);
                
	    }
	    
            /* fill the detector */
            if( c3d.Eta() > 0 )
                for(unsigned isection=0; isection<4; isection++)      
                    _subdet[0][isection]->add<HGCC3D>( c3d );
            else 
                for(unsigned isection=0; isection<4; isection++)      
                    _subdet[1][isection]->add<HGCC3D>( c3d );
             
        }

    }// end C3D
 
}

void HGC::addGen(HGCgen gen){

    _gen[gen.id()] = gen;
    _genVec.push_back( &_gen[gen.id()] );

}

void HGC::addGenpart(HGCgenpart genpart){

    _genpart[genpart.Gen()] = genpart;
    _genpartVec.push_back( &_genpart[genpart.Gen()] );

}

/* get objects by id */
vector<HGCgen*>       HGC::getGenAll()                   { return _genVec           ; }
vector<HGCgenpart*>   HGC::getGenpartAll()               { return _genpartVec           ; }

vector<HGCgenpart*> HGC::getGenpartAllInPhiRegion(double minPhi, double maxPhi){

    vector<HGCgenpart*> selectedGenpart;

    for(unsigned igenpart=0; igenpart<_genpartVec.size(); igenpart++){
        if( _genpartVec.at(igenpart)->ReachedEE() ){
            double phi = TMath::ASin( _genpartVec.at(igenpart)->Posx().at(0)  / _genpartVec.at(igenpart)->Posy().at(0) );
            if( phi>minPhi && phi<maxPhi ){ 
                selectedGenpart.push_back( _genpartVec.at(igenpart) );
            }
        }
    }

    return selectedGenpart;

}


void HGC::clear() {

    _gen.clear();
    _genpart.clear();

    _genVec.clear();
    _genpartVec.clear();

    _subdet[0][0]->clear();
    _subdet[0][1]->clear();
    _subdet[0][2]->clear();
    _subdet[0][3]->clear();

    _subdet[1][0]->clear();
    _subdet[1][1]->clear();
    _subdet[1][2]->clear();
    _subdet[1][3]->clear();

}


