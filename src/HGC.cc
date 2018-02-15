
/* mylibs */
#include "HGC.h"


HGC::HGC( TList *fileList, bool flagTCs, bool flagC2D, bool flagC3D, bool flagGen, bool flagGenpart, int verboselevel ) {
    
    /* init */
    _evt = 0;
    _verboselevel = verboselevel;
    _flagTCs = flagTCs;
    _flagC2D = flagC2D;
    _flagC3D = flagC3D;
    _flagGen = flagGen;
    _flagGenpart = flagGenpart;
    
    for(unsigned i=0; i<Nlayers; i++) {
        _TD[i]  = new vector<HGCROC> ;
        _TD[i] ->reserve(5000);
    }
    
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

    // Generated Particles (reco)
    if( _flagGen ){
        if( _chain->GetBranchStatus("gen_n"                ) ) { _missing__gen_n               = false; _chain->SetBranchAddress("gen_n"                , &_gen_n                ); } else { _missing__gen_n               = true; }
        if( _chain->GetBranchStatus("gen_id"               ) ) { _missing__gen_id              = false; _chain->SetBranchAddress("gen_id"               , &_gen_id               ); } else { _missing__gen_id              = true; }
        if( _chain->GetBranchStatus("gen_status"           ) ) { _missing__gen_status          = false; _chain->SetBranchAddress("gen_status"           , &_gen_status           ); } else { _missing__gen_status          = true; }
        if( _chain->GetBranchStatus("gen_energy"           ) ) { _missing__gen_energy          = false; _chain->SetBranchAddress("gen_energy"           , &_gen_energy           ); } else { _missing__gen_energy          = true; }
        if( _chain->GetBranchStatus("gen_pt"               ) ) { _missing__gen_pt              = false; _chain->SetBranchAddress("gen_pt"               , &_gen_pt               ); } else { _missing__gen_pt              = true; }
        if( _chain->GetBranchStatus("gen_eta"              ) ) { _missing__gen_eta             = false; _chain->SetBranchAddress("gen_eta"              , &_gen_eta              ); } else { _missing__gen_eta             = true; }
        if( _chain->GetBranchStatus("gen_phi"              ) ) { _missing__gen_phi             = false; _chain->SetBranchAddress("gen_phi"              , &_gen_phi              ); } else { _missing__gen_phi             = true; }
        if( _chain->GetBranchStatus("gen_PUNumInt"         ) ) { _missing__gen_PUNumInt        = false; _chain->SetBranchAddress("gen_PUNumInt"         , &_gen_PUNumInt         ); } else { _missing__gen_PUNumInt        = true; }
        if( _chain->GetBranchStatus("gen_TrueNumInt"       ) ) { _missing__gen_TrueNumInt      = false; _chain->SetBranchAddress("gen_TrueNumInt"       , &_gen_TrueNumInt       ); } else { _missing__gen_TrueNumInt      = true; }   
    }

    // Generated Particles (track)
    if( flagGenpart ){
        if( _chain->GetBranchStatus("genpart_eta"         ) ) { missing__genpart_eta        = false; _chain->SetBranchAddress("genpart_eta"         , &_genpart_eta        ); } else { missing__genpart_eta        = true; }
        if( _chain->GetBranchStatus("genpart_phi"         ) ) { missing__genpart_phi        = false; _chain->SetBranchAddress("genpart_phi"         , &_genpart_phi        ); } else { missing__genpart_phi        = true; }
        if( _chain->GetBranchStatus("genpart_pt"          ) ) { missing__genpart_pt         = false; _chain->SetBranchAddress("genpart_pt"          , &_genpart_pt         ); } else { missing__genpart_pt         = true; }
        if( _chain->GetBranchStatus("genpart_energy"      ) ) { missing__genpart_energy     = false; _chain->SetBranchAddress("genpart_energy"      , &_genpart_energy     ); } else { missing__genpart_energy     = true; }
        if( _chain->GetBranchStatus("genpart_dvx"         ) ) { missing__genpart_dvx        = false; _chain->SetBranchAddress("genpart_dvx"         , &_genpart_dvx        ); } else { missing__genpart_dvx        = true; }
        if( _chain->GetBranchStatus("genpart_dvy"         ) ) { missing__genpart_dvy        = false; _chain->SetBranchAddress("genpart_dvy"         , &_genpart_dvy        ); } else { missing__genpart_dvy        = true; }
        if( _chain->GetBranchStatus("genpart_dvz"         ) ) { missing__genpart_dvz        = false; _chain->SetBranchAddress("genpart_dvz"         , &_genpart_dvz        ); } else { missing__genpart_dvz        = true; }
        if( _chain->GetBranchStatus("genpart_ovx"         ) ) { missing__genpart_ovx        = false; _chain->SetBranchAddress("genpart_ovx"         , &_genpart_ovx        ); } else { missing__genpart_ovx        = true; }
        if( _chain->GetBranchStatus("genpart_ovy"         ) ) { missing__genpart_ovy        = false; _chain->SetBranchAddress("genpart_ovy"         , &_genpart_ovy        ); } else { missing__genpart_ovy        = true; }
        if( _chain->GetBranchStatus("genpart_ovz"         ) ) { missing__genpart_ovz        = false; _chain->SetBranchAddress("genpart_ovz"         , &_genpart_ovz        ); } else { missing__genpart_ovz        = true; }
        if( _chain->GetBranchStatus("genpart_exx"         ) ) { missing__genpart_exx        = false; _chain->SetBranchAddress("genpart_exx"         , &_genpart_exx        ); } else { missing__genpart_exx        = true; }
        if( _chain->GetBranchStatus("genpart_exy"         ) ) { missing__genpart_exy        = false; _chain->SetBranchAddress("genpart_exy"         , &_genpart_exy        ); } else { missing__genpart_exy        = true; }
        if( _chain->GetBranchStatus("genpart_mother"      ) ) { missing__genpart_mother     = false; _chain->SetBranchAddress("genpart_mother"      , &_genpart_mother     ); } else { missing__genpart_mother     = true; }
        if( _chain->GetBranchStatus("genpart_exphi"       ) ) { missing__genpart_exphi      = false; _chain->SetBranchAddress("genpart_exphi"       , &_genpart_exphi      ); } else { missing__genpart_exphi      = true; }
        if( _chain->GetBranchStatus("genpart_exeta"       ) ) { missing__genpart_exeta      = false; _chain->SetBranchAddress("genpart_exeta"       , &_genpart_exeta      ); } else { missing__genpart_exeta      = true; }
        if( _chain->GetBranchStatus("genpart_fbrem"       ) ) { missing__genpart_fbrem      = false; _chain->SetBranchAddress("genpart_fbrem"       , &_genpart_fbrem      ); } else { missing__genpart_fbrem      = true; }
        if( _chain->GetBranchStatus("genpart_pid"         ) ) { missing__genpart_pid        = false; _chain->SetBranchAddress("genpart_pid"         , &_genpart_pid        ); } else { missing__genpart_pid        = true; }
        if( _chain->GetBranchStatus("genpart_gen"         ) ) { missing__genpart_gen        = false; _chain->SetBranchAddress("genpart_gen"         , &_genpart_gen        ); } else { missing__genpart_gen        = true; }  
        if( _chain->GetBranchStatus("genpart_reachedEE"   ) ) { missing__genpart_reachedEE  = false; _chain->SetBranchAddress("genpart_reachedEE"   , &_genpart_reachedEE  ); } else { missing__genpart_reachedEE  = true; }  
        if( _chain->GetBranchStatus("genpart_fromBeamPipe") ) { missing__genpart_fromBeamPipe= false; _chain->SetBranchAddress("genpart_fromBeamPipe", &_genpart_fromBeamPipe); } else { missing__genpart_fromBeamPipe= true; }
        if( _chain->GetBranchStatus("genpart_posx"        ) ) { missing__genpart_posx       = false; _chain->SetBranchAddress("genpart_posx"        , &_genpart_posx       ); } else { missing__genpart_posx       = true; }
        if( _chain->GetBranchStatus("genpart_posy"        ) ) { missing__genpart_posy       = false; _chain->SetBranchAddress("genpart_posy"        , &_genpart_posy       ); } else { missing__genpart_posy       = true; }
        if( _chain->GetBranchStatus("genpart_posz"        ) ) { missing__genpart_posz       = false; _chain->SetBranchAddress("genpart_posz"        , &_genpart_posz       ); } else { missing__genpart_posz       = true; }
    }


    // Trigger cells
    if( _flagTCs ){
        if( _chain->GetBranchStatus( "tc_n"         ) ) { _missing__tc_n          = false; _chain->SetBranchAddress( "tc_n"         , &_tc_n         ); } else { _missing__tc_n          = true; }
        if( _chain->GetBranchStatus( "tc_id"        ) ) { _missing__tc_id         = false; _chain->SetBranchAddress( "tc_id"        , &_tc_id        ); } else { _missing__tc_id         = true; }
        if( _chain->GetBranchStatus( "tc_subdet"    ) ) { _missing__tc_subdet     = false; _chain->SetBranchAddress( "tc_subdet"    , &_tc_subdet    ); } else { _missing__tc_subdet     = true; }
        if( _chain->GetBranchStatus( "tc_zside"     ) ) { _missing__tc_zside      = false; _chain->SetBranchAddress( "tc_zside"     , &_tc_zside     ); } else { _missing__tc_zside      = true; }
        if( _chain->GetBranchStatus( "tc_layer"     ) ) { _missing__tc_layer      = false; _chain->SetBranchAddress( "tc_layer"     , &_tc_layer     ); } else { _missing__tc_layer      = true; }
        if( _chain->GetBranchStatus( "tc_wafer"     ) ) { _missing__tc_wafer      = false; _chain->SetBranchAddress( "tc_wafer"     , &_tc_wafer     ); } else { _missing__tc_wafer      = true; }
        if( _chain->GetBranchStatus( "tc_wafertype" ) ) { _missing__tc_wafertype  = false; _chain->SetBranchAddress( "tc_wafertype" , &_tc_wafertype ); } else { _missing__tc_wafertype  = true; }
        if( _chain->GetBranchStatus( "tc_cell"      ) ) { _missing__tc_cell       = false; _chain->SetBranchAddress( "tc_cell"      , &_tc_cell      ); } else { _missing__tc_cell       = true; }
        if( _chain->GetBranchStatus( "tc_data"      ) ) { _missing__tc_data       = false; _chain->SetBranchAddress( "tc_data"      , &_tc_data      ); } else { _missing__tc_data       = true; }
        if( _chain->GetBranchStatus( "tc_energy"    ) ) { _missing__tc_energy     = false; _chain->SetBranchAddress( "tc_energy"    , &_tc_energy    ); } else { _missing__tc_energy     = true; }
        if( _chain->GetBranchStatus( "tc_eta"       ) ) { _missing__tc_eta        = false; _chain->SetBranchAddress( "tc_eta"       , &_tc_eta       ); } else { _missing__tc_eta        = true; }
        if( _chain->GetBranchStatus( "tc_phi"       ) ) { _missing__tc_phi        = false; _chain->SetBranchAddress( "tc_phi"       , &_tc_phi       ); } else { _missing__tc_phi        = true; }
        if( _chain->GetBranchStatus( "tc_x"         ) ) { _missing__tc_x          = false; _chain->SetBranchAddress( "tc_x"         , &_tc_x         ); } else { _missing__tc_x          = true; }
        if( _chain->GetBranchStatus( "tc_y"         ) ) { _missing__tc_y          = false; _chain->SetBranchAddress( "tc_y"         , &_tc_y         ); } else { _missing__tc_y          = true; }
        if( _chain->GetBranchStatus( "tc_z"         ) ) { _missing__tc_z          = false; _chain->SetBranchAddress( "tc_z"         , &_tc_z         ); } else { _missing__tc_z          = true; }
                                                                                                                                                                                         
    }

    // C2D                                                                                                                                                                               
    if( _flagC2D ) {                                                                                                                                                                     
        if( _chain->GetBranchStatus( "cl_n"         ) ) { _missing__cl_n          = false; _chain->SetBranchAddress("cl_n"          , &_cl_n         ); } else { _missing__cl_n          = true; }
        if( _chain->GetBranchStatus( "cl_pt"        ) ) { _missing__cl_pt         = false; _chain->SetBranchAddress("cl_pt"         , &_cl_pt        ); } else { _missing__cl_pt         = true; }                                                       
        if( _chain->GetBranchStatus( "cl_energy"    ) ) { _missing__cl_energy     = false; _chain->SetBranchAddress("cl_energy"     , &_cl_energy    ); } else { _missing__cl_energy     = true; }
        if( _chain->GetBranchStatus( "cl_eta"       ) ) { _missing__cl_eta        = false; _chain->SetBranchAddress("cl_eta"        , &_cl_eta       ); } else { _missing__cl_eta        = true; }                                                              
        if( _chain->GetBranchStatus( "cl_phi"       ) ) { _missing__cl_phi        = false; _chain->SetBranchAddress("cl_phi"        , &_cl_phi       ); } else { _missing__cl_phi        = true; }
        if( _chain->GetBranchStatus( "cl_z"         ) ) { _missing__cl_z          = false; _chain->SetBranchAddress("cl_z"          , &_cl_z         ); } else { _missing__cl_z          = true; }
        if( _chain->GetBranchStatus( "cl_layer"     ) ) { _missing__cl_layer      = false; _chain->SetBranchAddress("cl_layer"      , &_cl_layer     ); } else { _missing__cl_layer      = true; }
        if( _chain->GetBranchStatus( "cl_cells_n"   ) ) { _missing__cl_cells_n    = false; _chain->SetBranchAddress("cl_cells_n"    , &_cl_cells_n   ); } else { _missing__cl_cells_n    = true; }
        if( _chain->GetBranchStatus( "cl_cells_id"  ) ) { _missing__cl_cells_id   = false; _chain->SetBranchAddress("cl_cells_id"   , &_cl_cells_id  ); } else { _missing__cl_cells_id   = true; }
        
    } 
                                                                                               
    //C3D
    if( _flagC3D ) {       
        if( _chain->GetBranchStatus( "cl3d_id"      ) ) { _missing__cl3d_id       = false ; _chain->SetBranchAddress("cl3d_id"      , &_cl3d_id      ); } else { _missing__cl3d_id       = true; }
        if( _chain->GetBranchStatus( "cl3d_pt"      ) ) { _missing__cl3d_pt       = false ; _chain->SetBranchAddress("cl3d_pt"      , &_cl3d_pt      ); } else { _missing__cl3d_pt       = true; }
        if( _chain->GetBranchStatus( "cl3d_energy"  ) ) { _missing__cl3d_energy   = false ; _chain->SetBranchAddress("cl3d_energy"  , &_cl3d_energy  ); } else { _missing__cl3d_energy   = true; }
        if( _chain->GetBranchStatus( "cl3d_eta"     ) ) { _missing__cl3d_eta      = false ; _chain->SetBranchAddress("cl3d_eta"     , &_cl3d_eta     ); } else { _missing__cl3d_eta      = true; }
        if( _chain->GetBranchStatus( "cl3d_phi"     ) ) { _missing__cl3d_phi      = false ; _chain->SetBranchAddress("cl3d_phi"     , &_cl3d_phi     ); } else { _missing__cl3d_phi      = true; }
        if( _chain->GetBranchStatus( "cl3d_clusters") ) { _missing__cl3d_clusters = false ; _chain->SetBranchAddress("cl3d_clusters", &_cl3d_clusters); } else { _missing__cl3d_clusters = true; }
    }

}

HGC::~HGC() {
    
    for(unsigned i=0; i<Nlayers; i++) {
        delete _TD[i];
    }

};
    
unsigned HGC::getEvents(){

    return _chain->GetEntries();

}

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
            if( !_missing__gen_id         ) { gen.setG4Id  (_gen_id     ->at(igen)   ); }
            if( !_missing__gen_status     ) { gen.setStatus(_gen_status ->at(igen)   ); }
            if( !_missing__gen_energy     ) { gen.setEnergy(_gen_energy ->at(igen)   ); }
            if( !_missing__gen_pt         ) { gen.setPt    (_gen_pt     ->at(igen)   ); }
            if( !_missing__gen_eta        ) { gen.setEta   (_gen_eta    ->at(igen)   ); }
            if( !_missing__gen_phi        ) { gen.setPhi   (_gen_phi    ->at(igen)   ); }
            
            this->addGen( gen );
        }
    }

    /*************************/
    /* looping over Gen Part */
    if(_flagGenpart ){

        unsigned ngenpart=0;
        if( !missing__genpart_eta ) { 
            ngenpart=_genpart_eta->size() ; 
        }
        else {
            cout << " HGC >>> !!! Warning !!! genpart branch eta missing " << endl;
        }
        HGCgenpart genpart;
        
        for( unsigned igenpart = 0; igenpart<ngenpart; igenpart++ ){
            
            if( !missing__genpart_eta        ) { genpart.setEta          (_genpart_eta        ->at(igenpart) ); }
            if( !missing__genpart_phi        ) { genpart.setPhi          (_genpart_phi        ->at(igenpart) ); }
            if( !missing__genpart_pt         ) { genpart.setPt           (_genpart_pt         ->at(igenpart) ); }
            if( !missing__genpart_energy     ) { genpart.setEnergy       (_genpart_energy     ->at(igenpart) ); }
            if( !missing__genpart_dvx        ) { genpart.setDvx          (_genpart_dvx        ->at(igenpart) ); }
            if( !missing__genpart_dvy        ) { genpart.setDvy          (_genpart_dvy        ->at(igenpart) ); }
            if( !missing__genpart_dvz        ) { genpart.setDvz          (_genpart_dvz        ->at(igenpart) ); }
            if( !missing__genpart_ovx        ) { genpart.setOvx          (_genpart_ovx        ->at(igenpart) ); }
            if( !missing__genpart_ovy        ) { genpart.setOvy          (_genpart_ovy        ->at(igenpart) ); }
            if( !missing__genpart_ovz        ) { genpart.setOvz          (_genpart_ovz        ->at(igenpart) ); }
            if( !missing__genpart_exx        ) { genpart.setExx          (_genpart_exx        ->at(igenpart) ); }
            if( !missing__genpart_exy        ) { genpart.setExy          (_genpart_exy        ->at(igenpart) ); }
            if( !missing__genpart_mother     ) { genpart.setMother       (_genpart_mother     ->at(igenpart) ); }
            if( !missing__genpart_exphi      ) { genpart.setExphi        (_genpart_exphi      ->at(igenpart) ); }
            if( !missing__genpart_exeta      ) { genpart.setExeta        (_genpart_exeta      ->at(igenpart) ); }
            if( !missing__genpart_fbrem      ) { genpart.setFbrem        (_genpart_fbrem      ->at(igenpart) ); }
            if( !missing__genpart_pid        ) { genpart.setPDGid        (_genpart_pid        ->at(igenpart) ); }
            if( !missing__genpart_gen        ) { genpart.setGen          (_genpart_gen        ->at(igenpart) ); }     
            if( !missing__genpart_reachedEE  ) { genpart.setReachedEE    (_genpart_reachedEE  ->at(igenpart) ); }
            if( !missing__genpart_fromBeamPipe) { genpart.setFromBeamPipe(_genpart_fromBeamPipe->at(igenpart) ); }
            //if( !missing__genpart_posx       ) { genpart.setPosx         (_genpart_posx       ->at(igenpart) ); }
            //if( !missing__genpart_posy       ) { genpart.setPosy         (_genpart_posy       ->at(igenpart) ); }
            //if( !missing__genpart_posz       ) { genpart.setPosz         (_genpart_posz       ->at(igenpart) ); }

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
	  if( !_missing__tc_zside    ) tc.setZSide     ( _tc_zside    ->at( itc ) );
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
	  this->addTC( tc );
                    
        }// end TCs LOOP
        
    }// end TCs

    /*****************/
    /* Loop over C2D */
    if( _flagC2D ) {

        if( !_missing__cl_n ){
            cout << "_cl_n " << _cl_n << endl;
            for(int iclu=0; iclu<_cl_n; iclu++){
                
                HGCC2D c2d;
                if( !_missing__cl_pt       ) c2d.setPt     ( _cl_pt      ->at(iclu) );
                if( !_missing__cl_energy   ) c2d.setEnergy ( _cl_energy  ->at(iclu) );
                if( !_missing__cl_eta      ) c2d.setEta    ( _cl_eta     ->at(iclu) );
                if( !_missing__cl_phi      ) c2d.setPhi    ( _cl_phi     ->at(iclu) );
                if( !_missing__cl_layer    ) c2d.setLayer  ( _cl_layer   ->at(iclu) );
                if( !_missing__cl_cells_id ) c2d.setCells  ( _cl_cells_id->at(iclu) );	    

                int subdet = -1;
                
                if( _flagTCs ){
                    HGCTC tc0 = this->getTC( c2d.cells()[0] );
                    subdet = tc0.subdet();
                    c2d.setId( tc0.id() ) ;
                    if( !_missing__cl_z        ) { 
                        c2d.setZ( _cl_z       ->at(iclu) );
                    }
                    else{
                        c2d.setZ( tc0.z() ) ;
                    }    
                }
                else{
                    cout << "Warning: TCs z coordinate not set!!! Load the cells to get it (src/HGC.cc)" << endl;
                }
                c2d.setSubdet( subdet );
                
                /* fill the detector */
                this->addC2D( c2d );
                
            }

        }

    }// end C2D


    /*****************/
    /* Loop over C3D */
    if( _flagC3D ) {

        unsigned nc3d = _cl3d_id->size();                
        for(unsigned ic3d=0; ic3d<nc3d; ic3d++){
            
            HGCC3D c3d;
	    if( !_missing__cl3d_id       ) c3d.setId(       _cl3d_id      ->at(ic3d) );
            if( !_missing__cl3d_pt       ) c3d.setPt(       _cl3d_pt      ->at(ic3d) );
	    if( !_missing__cl3d_energy   ) c3d.setEnergy(   _cl3d_energy  ->at(ic3d) );
	    if( !_missing__cl3d_eta      ) c3d.setEta(      _cl3d_eta     ->at(ic3d) );
	    if( !_missing__cl3d_phi      ) c3d.setPhi(      _cl3d_phi     ->at(ic3d) );	   
	    if( !_missing__cl3d_clusters ) c3d.setClusters( _cl3d_clusters->at(ic3d) );	    
            
	    if(_flagC2D){
                
                vector<unsigned> cl3d_cells;
                for(auto iclu : c3d.clusters()){
                    HGCC2D c2d = this->getC2D(iclu);
                    vector<unsigned> TCs = c2d.cells();
                    cl3d_cells.insert( cl3d_cells.end(), TCs.begin(), TCs.end() );
                }
                c3d.setCells(cl3d_cells);
                
	    }
	    
            /* fill the detector */
            this->addC3D( c3d );
            
        }

    }// end C3D
    
    /* Layers Z */
    
    _layerZ[ 0]= -1;
    // EE
    _layerZ[ 1]= 320.755;
    _layerZ[ 2]= 321.505;
    _layerZ[ 3]= 322.735;
    _layerZ[ 4]= 323.485;
    _layerZ[ 5]= 324.715;
    _layerZ[ 6]= 325.465;
    _layerZ[ 7]= 326.695;
    _layerZ[ 8]= 327.445;
    _layerZ[ 9]= 328.675;
    _layerZ[10]= 329.425;
    _layerZ[11]= 330.735;
    _layerZ[12]= 331.605;
    _layerZ[13]= 332.915;
    _layerZ[14]= 333.785;
    _layerZ[15]= 335.095;
    _layerZ[16]= 335.965;
    _layerZ[17]= 337.275;
    _layerZ[18]= 338.145;
    _layerZ[19]= 339.455;
    _layerZ[20]= 340.325;
    _layerZ[21]= 341.775;
    _layerZ[22]= 342.845;
    _layerZ[23]= 344.295;
    _layerZ[24]= 345.365;
    _layerZ[25]= 346.815;
    _layerZ[26]= 347.885;
    _layerZ[27]= 349.335;
    _layerZ[28]= 350.405;
    //FH
    _layerZ[29]= 356.335;
    _layerZ[30]= 361.015;
    _layerZ[31]= 365.695;
    _layerZ[32]= 370.375;
    _layerZ[33]= 375.055;
    _layerZ[34]= 379.735;
    _layerZ[35]= 384.415;
    _layerZ[36]= 389.095;
    _layerZ[37]= 393.775;
    _layerZ[38]= 398.455;
    _layerZ[39]= 403.135;
    _layerZ[40]= 407.815;

    // BH is not exact
    _layerZ[41]= 421.0;
    _layerZ[42]= 421.0+9.0*1;
    _layerZ[43]= 421.0+9.0*2;
    _layerZ[44]= 421.0+9.0*3;
    _layerZ[45]= 421.0+9.0*4;
    _layerZ[46]= 421.0+9.0*5;
    _layerZ[47]= 421.0+9.0*6;
    _layerZ[48]= 421.0+9.0*7;
    _layerZ[49]= 421.0+9.0*8;
    _layerZ[50]= 421.0+9.0*9;
    _layerZ[51]= 421.0+9.0*10;
    _layerZ[52]= 421.0+9.0*11;
    
}


void HGC::addTC( HGCTC tc ) { 

//    const unsigned tcLayer = tc.correctedLayer();
    
    /* add tc to the map*/
    _TCs[tc.id()] = tc;
    
    /* keep the pointer for easy access */
    _TCvec.push_back( &_TCs[tc.id()] );
    //_TC_layer[tcLayer].push_back( &_TCs[tc.id()] );

//    HGCROC TD( tc );
//
//    /* add to the correct HGCROC */
//    bool found = false;
//    for(unsigned ihgcroc=0; ihgcroc<_TD[tcLayer]->size(); ihgcroc++ ){ 
//        if( _TD[tcLayer]->at(ihgcroc).tcIsContained( tc ) ){
//            found = true;
//            _TD[tcLayer]->at(ihgcroc).addTC( tc );
//            break;
//        }
//    }
//
//    if( !found ) _TD[tcLayer]->push_back( TD );
        
}


void HGC::addC2D( HGCC2D c2d ) { 
    
    //const unsigned c2dLayer = c2d.correctedLayer();
    
    /* add c2d to the map */
    _C2Ds[c2d.id()] = c2d;
  
    /* keep the pointer for easy access */
    _C2Dvec.push_back( &_C2Ds[c2d.id()] );
//    _C2D_layer[c2dLayer].push_back( &_C2Ds[c2d.id()] );

}


void HGC::addC3D(HGCC3D c3d)      {   

    /* add c2d to the map */
    _C3Ds[c3d.id()] = c3d;

    /* keep the pointer for easy access */
    _C3Dvec.push_back( &_C3Ds[c3d.id()] );

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
HGCTC                 HGC::getTC(unsigned ID)      { return _TCs[ID]          ; }
HGCC2D                HGC::getC2D(unsigned ID)     { return _C2Ds[ID]         ; }
HGCC3D                HGC::getC3D(unsigned ID)     { return _C3Ds[ID]         ; }

map<unsigned,HGCTC>  *HGC::getTCmap()              { return &_TCs             ; }
map<unsigned,HGCC2D> *HGC::getC2Dmap()             { return &_C2Ds            ; }
map<unsigned,HGCC3D> *HGC::getC3Dmap()             { return &_C3Ds            ; }

vector<HGCTC*>        HGC::getTC_layer(unsigned layer)   { return _TC_layer[layer]  ; }
vector<HGCC2D*>       HGC::getC2D_layer(unsigned layer)  { return _C2D_layer[layer] ; }
vector<HGCROC>       *HGC::getTD(unsigned layer)         { return _TD[layer]        ; }


vector<HGCTC*>        HGC::getTCall()                    { return _TCvec            ; }
vector<HGCC2D*>       HGC::getC2Dall()                   { return _C2Dvec           ; }
vector<HGCC3D*>       HGC::getC3Dall()                   { return _C3Dvec           ; }
vector<HGCgen*>       HGC::getGenAll()                   { return _genVec           ; }


void HGC::getTDall( vector<HGCROC> &data ) {

    for(unsigned i=0; i<Nlayers; i++)
        data.insert(data.end(), _TD[i]->begin(), _TD[i]->end() );

}


vector<HGCTC*> HGC::getTCallInPhiRegion(double minPhi, double maxPhi){

    vector<HGCTC*> selectedTc;

    for(unsigned itc=0; itc<_TCvec.size(); itc++){
        if( _TCvec.at(itc)->Phi()>minPhi && _TCvec.at(itc)->Phi()<maxPhi ){
            selectedTc.push_back( _TCvec.at(itc) );
        }
    }

    return selectedTc;

}


vector<HGCC2D*> HGC::getC2DallInPhiRegion(double minPhi, double maxPhi){

    vector<HGCC2D*> selectedC2D;

    for(unsigned iC2D=0; iC2D<_C2Dvec.size(); iC2D++){
        if( _C2Dvec.at(iC2D)->Phi()>minPhi && _C2Dvec.at(iC2D)->Phi()<maxPhi ){ 
            selectedC2D.push_back( _C2Dvec.at(iC2D) );
        }
    }

    return selectedC2D;

}


vector<HGCC2D*> HGC::getC2DallInRphiRegion(double minR, double maxR, double minPhi, double maxPhi){

    vector<HGCC2D*> selectedC2D;

    for(unsigned iC2D=0; iC2D<_C2Dvec.size(); iC2D++){
        if( _C2Dvec.at(iC2D)->Phi()>minPhi && _C2Dvec.at(iC2D)->Phi()<maxPhi && 
            _C2Dvec.at(iC2D)->r()>minR && _C2Dvec.at(iC2D)->r()<maxR            ){
            selectedC2D.push_back( _C2Dvec.at(iC2D) );
        }
    }

    return selectedC2D;

}


vector<HGCTC*> HGC::getTCallInRphiRegion(double minR, double maxR, double minPhi, double maxPhi){

    vector<HGCTC*> selectedTc;

    for(unsigned itc=0; itc<_TCvec.size(); itc++){
        if( _TCvec.at(itc)->Phi()>minPhi && _TCvec.at(itc)->Phi()<maxPhi &&
            _TCvec.at(itc)->r()>minR     && _TCvec.at(itc)->r()<maxR        ){
            selectedTc.push_back( _TCvec.at(itc) );
        }
    }

    return selectedTc;

}


void HGC::clear() {

    for(unsigned ilayer=0; ilayer<Nlayers; ilayer++){
        _TC_layer[ilayer].clear();
        _C2D_layer[ilayer].clear();
    }    
    
    _TCs.clear();
    _C2Ds.clear();
    _C3Ds.clear();
    
    _TCvec.clear();
    _C2Dvec.clear();
    _C3Dvec.clear();

    

}


