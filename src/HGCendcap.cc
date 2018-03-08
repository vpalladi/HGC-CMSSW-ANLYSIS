
/* mylibs */
#include "HGCendcap.h"


HGCendcap::HGCendcap( TChain *chain, int endcapId, bool flagTCs, bool flagC2D, bool flagC3D, int verboselevel ) {
    
    /* init */
    _chain = chain;
    _endcapId = endcapId;
    _evt = 0;
    _verboselevel = verboselevel;
    _flagTCs = flagTCs;
    _flagC2D = flagC2D;
    _flagC3D = flagC3D;
    
    for(unsigned i=0; i<nLayers; i++) {
        _TD[i]  = new vector<HGCROC> ;
        _TD[i] ->reserve(5000);
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
    // middle EE 334.440;
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

    if( _endcapId==1 )
        for(int ilayer=0; ilayer<nLayers; ilayer++)
            _layerZ[ilayer] = -_layerZ[ilayer];


}

HGCendcap::~HGCendcap() {
    
    for(unsigned i=0; i<nLayers; i++) {
        delete _TD[i];
    }

};
    
unsigned HGCendcap::getEvents(){

    return _chain->GetEntries();

}

void HGCendcap::getEvent( int evt ){
        
    /* clear detector before getting the event */
    this->clear();

    /* Get Entry */
    _chain->GetEntry( evt );        
    cout << " >>> HGCendcap: evt No " << evt << endl;

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
          if( isPertinent( tc.z() ) )
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
                    cout << "Warning: TCs z coordinate not set!!! Load the cells to get it (src/HGCendcap.cc)" << endl;
                }
                c2d.setSubdet( subdet );
                
                /* fill the detector */
                if( isPertinent( c2d.z() ) )
                    this->addC2D( c2d );
                
            }

        }

    }// end C2D
//
//
//    /*****************/
//    /* Loop over C3D */
//    if( _flagC3D ) {
//
//        unsigned nc3d = _cl3d_id->size();                
//        for(unsigned ic3d=0; ic3d<nc3d; ic3d++){
//            
//            HGCC3D c3d;
//	    if( !_missing__cl3d_id       ) c3d.setId(       _cl3d_id      ->at(ic3d) );
//            if( !_missing__cl3d_pt       ) c3d.setPt(       _cl3d_pt      ->at(ic3d) );
//	    if( !_missing__cl3d_energy   ) c3d.setEnergy(   _cl3d_energy  ->at(ic3d) );
//	    if( !_missing__cl3d_eta      ) c3d.setEta(      _cl3d_eta     ->at(ic3d) );
//	    if( !_missing__cl3d_phi      ) c3d.setPhi(      _cl3d_phi     ->at(ic3d) );	   
//	    if( !_missing__cl3d_clusters ) c3d.setClusters( _cl3d_clusters->at(ic3d) );	    
//            
//	    if(_flagC2D){
//                
//                vector<unsigned> cl3d_cells;
//                for(auto iclu : c3d.clusters()){
//                    HGCC2D c2d = this->getC2D(iclu);
//                    vector<unsigned> TCs = c2d.cells();
//                    cl3d_cells.insert( cl3d_cells.end(), TCs.begin(), TCs.end() );
//                }
//                c3d.setCells(cl3d_cells);
//                
//	    }
//	    
//            /* fill the detector */
//            if( isPertinent( c3d.Eta() ) )
//                this->addC3D( c3d );
//            
//        }
//
//    }// end C3D
//    
    
}


void HGCendcap::addTC( HGCTC tc ) { 

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


void HGCendcap::addC2D( HGCC2D c2d ) { 
    
    //const unsigned c2dLayer = c2d.correctedLayer();
    
    /* add c2d to the map */
    _C2Ds[c2d.id()] = c2d;
  
    /* keep the pointer for easy access */
    _C2Dvec.push_back( &_C2Ds[c2d.id()] );
//    _C2D_layer[c2dLayer].push_back( &_C2Ds[c2d.id()] );

}


void HGCendcap::addC3D(HGCC3D c3d)      {   

    /* add c2d to the map */
    _C3Ds[c3d.id()] = c3d;

    /* keep the pointer for easy access */
    _C3Dvec.push_back( &_C3Ds[c3d.id()] );

}



/* get objects by id */
HGCTC                 HGCendcap::getTC(unsigned ID)      { return _TCs[ID]          ; }
HGCC2D                HGCendcap::getC2D(unsigned ID)     { return _C2Ds[ID]         ; }
HGCC3D                HGCendcap::getC3D(unsigned ID)     { return _C3Ds[ID]         ; }

map<unsigned,HGCTC>  *HGCendcap::getTCmap()              { return &_TCs             ; }
map<unsigned,HGCC2D> *HGCendcap::getC2Dmap()             { return &_C2Ds            ; }
map<unsigned,HGCC3D> *HGCendcap::getC3Dmap()             { return &_C3Ds            ; }

vector<HGCTC*>        HGCendcap::getTC_layer(unsigned layer)   { return _TC_layer[layer]  ; }
vector<HGCC2D*>       HGCendcap::getC2D_layer(unsigned layer)  { return _C2D_layer[layer] ; }
vector<HGCROC>       *HGCendcap::getTD(unsigned layer)         { return _TD[layer]        ; }


vector<HGCTC*>        HGCendcap::getTCall()                    { return _TCvec            ; }
vector<HGCC2D*>       HGCendcap::getC2Dall()                   { return _C2Dvec           ; }
vector<HGCC3D*>       HGCendcap::getC3Dall()                   { return _C3Dvec           ; }

void HGCendcap::getTDall( vector<HGCROC> &data ) {

    for(unsigned i=0; i<nLayers; i++)
        data.insert(data.end(), _TD[i]->begin(), _TD[i]->end() );

}


vector<HGCTC*> HGCendcap::getTCallInPhiRegion(double minPhi, double maxPhi){

    vector<HGCTC*> selectedTc;

    for(unsigned itc=0; itc<_TCvec.size(); itc++){
        if( _TCvec.at(itc)->Phi()>minPhi && _TCvec.at(itc)->Phi()<maxPhi ){
            selectedTc.push_back( _TCvec.at(itc) );
        }
    }

    return selectedTc;

}


vector<HGCC2D*> HGCendcap::getC2DallInPhiRegion(double minPhi, double maxPhi){

    vector<HGCC2D*> selectedC2D;

    for(unsigned iC2D=0; iC2D<_C2Dvec.size(); iC2D++){
        if( _C2Dvec.at(iC2D)->Phi()>minPhi && _C2Dvec.at(iC2D)->Phi()<maxPhi ){ 
            selectedC2D.push_back( _C2Dvec.at(iC2D) );
        }
    }

    return selectedC2D;

}


vector<HGCC2D*> HGCendcap::getC2DallInRphiRegion(double minR, double maxR, double minPhi, double maxPhi){

    vector<HGCC2D*> selectedC2D;

    for(unsigned iC2D=0; iC2D<_C2Dvec.size(); iC2D++){
        if( _C2Dvec.at(iC2D)->Phi()>minPhi && _C2Dvec.at(iC2D)->Phi()<maxPhi && 
            _C2Dvec.at(iC2D)->r()>minR && _C2Dvec.at(iC2D)->r()<maxR            ){
            selectedC2D.push_back( _C2Dvec.at(iC2D) );
        }
    }

    return selectedC2D;

}


vector<HGCTC*> HGCendcap::getTCallInRphiRegion(double minR, double maxR, double minPhi, double maxPhi){

    vector<HGCTC*> selectedTc;

    for(unsigned itc=0; itc<_TCvec.size(); itc++){
        if( _TCvec.at(itc)->Phi()>minPhi && _TCvec.at(itc)->Phi()<maxPhi &&
            _TCvec.at(itc)->r()>minR     && _TCvec.at(itc)->r()<maxR        ){
            selectedTc.push_back( _TCvec.at(itc) );
        }
    }

    return selectedTc;

}




void HGCendcap::clear() {

    for(unsigned ilayer=0; ilayer<nLayers; ilayer++){
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


bool HGCendcap::isPertinent(float z){

    if( _endcapId==0 && z>0 ) 
        return true;    
    else if( _endcapId==1 && z<0 ) 
        return true;
    else
        return false;
    
    return false;

}


