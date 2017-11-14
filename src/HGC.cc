
/* mylibs */
#include "HGC.h"


HGC::HGC( TList *fileList, bool flagTCs, bool flagC2D, bool flagC3D, int verboselevel ) {
    
    /* init */
    _evt = 0;
    _verboselevel = verboselevel;
    _flagTCs = flagTCs;
    _flagC2D = flagC2D;
    _flagC3D = flagC3D;
    
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
        if( _chain->GetBranchStatus( "tc_z"         ) ) { _missing__tc_z          = false; _chain->SetBranchAddress( "tc_z"         , &_tc_z         ); } else { _missing__tc_z          = true; }
                                                                                                                                                                                         
    }                                                                                                                                                                                    
                                                                                                                                                                                         
    // C2D                                                                                                                                                                               
    if( _flagC2D ) {                                                                                                                                                                     
        if( _chain->GetBranchStatus( "cl_n"         ) ) { _missing__cl_n          = false; _chain->SetBranchAddress("cl_n"          , &_cl_n         ); } else { _missing__cl_n          = true; }
        if( _chain->GetBranchStatus( "cl_pt"        ) ) { _missing__cl_pt         = false; _chain->SetBranchAddress("cl_pt"         , &_cl_pt        ); } else { _missing__cl_pt         = true; }                                                       
        if( _chain->GetBranchStatus( "cl_energy"    ) ) { _missing__cl_energy     = false; _chain->SetBranchAddress("cl_energy"     , &_cl_energy    ); } else { _missing__cl_energy     = true; }
        if( _chain->GetBranchStatus( "cl_eta"       ) ) { _missing__cl_eta        = false; _chain->SetBranchAddress("cl_eta"        , &_cl_eta       ); } else { _missing__cl_eta        = true; }                                                              
        if( _chain->GetBranchStatus( "cl_phi"       ) ) { _missing__cl_phi        = false; _chain->SetBranchAddress("cl_phi"        , &_cl_phi       ); } else { _missing__cl_phi        = true; }
        if( _chain->GetBranchStatus( "cl_x"         ) ) { _missing__cl_x          = false; _chain->SetBranchAddress("cl_x"          , &_cl_x         ); } else { _missing__cl_x          = true; }
        if( _chain->GetBranchStatus( "cl_y"         ) ) { _missing__cl_y          = false; _chain->SetBranchAddress("cl_y"          , &_cl_y         ); } else { _missing__cl_y          = true; }
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
    
    /********************/
    /* looping over TCs */
    if( _flagTCs ){
     
        /* LOOP */
        for( unsigned itc=0; itc<_tc_n; itc++ ){
            
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
                if( !_missing__cl_x        ) c2d.setX      ( _cl_x       ->at(iclu) );
                if( !_missing__cl_y        ) c2d.setY      ( _cl_y       ->at(iclu) );
                if( !_missing__cl_z        ) c2d.setZ      ( _cl_z       ->at(iclu) );
                if( !_missing__cl_layer    ) c2d.setLayer  ( _cl_layer   ->at(iclu) );
                if( !_missing__cl_cells_id ) c2d.setCells  ( _cl_cells_id->at(iclu) );	    
                
                int subdet = -1;
                
                if( _flagTCs ){
                    HGCTC tc0 = this->getTC( c2d.cells()[0] );
                    subdet = tc0.subdet();
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
  
}


void HGC::addTC( HGCTC tc ) { 

    const unsigned tcLayer = tc.correctedLayer();
    
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


void HGC::getTDall( vector<HGCROC> &data ) {

    for(unsigned i=0; i<Nlayers; i++)
        data.insert(data.end(), _TD[i]->begin(), _TD[i]->end() );

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


