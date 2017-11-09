
/* mylibs */
#include "HGC.h"


HGC::HGC( TList *fileList, bool flagTCs, bool flagC2D, int verboselevel ) {
    
    /* init */
    _evt = 0;
    _verboselevel = verboselevel;
    _flagTCs = flagTCs;
    _flagC2D = flagC2D;
    
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
    
    if( _flagTCs ){
      //_chain->SetBranchAddress( "tc_n"        , &_tc_n         );
        _chain->SetBranchAddress( "tc_id"       , &_tc_id        );
        _chain->SetBranchAddress( "tc_subdet"   , &_tc_subdet    );
        _chain->SetBranchAddress( "tc_zside"    , &_tc_zside     );
        _chain->SetBranchAddress( "tc_layer"    , &_tc_layer     );
        _chain->SetBranchAddress( "tc_wafer"    , &_tc_wafer     );
        _chain->SetBranchAddress( "tc_wafertype", &_tc_wafertype );
        _chain->SetBranchAddress( "tc_cell"     , &_tc_cell      );
        _chain->SetBranchAddress( "tc_data"     , &_tc_data      );
        _chain->SetBranchAddress( "tc_energy"   , &_tc_energy    );
        _chain->SetBranchAddress( "tc_eta"      , &_tc_eta       );
        _chain->SetBranchAddress( "tc_phi"      , &_tc_phi       );
        _chain->SetBranchAddress( "tc_z"        , &_tc_z         );
    }
    // C2D
    if( _flagC2D ) {
        _chain->SetBranchAddress("cl_n"      , &_cl_n      );
        _chain->SetBranchAddress("cl_pt"     , &_cl_pt     );
        _chain->SetBranchAddress("cl_energy" , &_cl_energy );
        _chain->SetBranchAddress("cl_eta"    , &_cl_eta    );
        _chain->SetBranchAddress("cl_phi"    , &_cl_phi    );
        _chain->SetBranchAddress("cl_x"      , &_cl_x      );
        _chain->SetBranchAddress("cl_y"      , &_cl_y      );
        _chain->SetBranchAddress("cl_z"      , &_cl_z      );
        _chain->SetBranchAddress("cl_layer"  , &_cl_layer  );
        _chain->SetBranchAddress("cl_ncells" , &_cl_ncells );
        _chain->SetBranchAddress("cl_cells"  , &_cl_cells  );
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
    
    /* looping over TCs */
    if( _flagTCs ){
     
        /* LOOP */
        unsigned nTC = _tc_id->size();
        for( unsigned itc=0; itc<nTC; itc++ ){
            
	  HGCTC tc;
	  tc.setId(         _tc_id->at( itc )       );
	  tc.setSubdet(     _tc_subdet->at( itc )   );            
	  tc.setZSide(      _tc_zside->at( itc )    );
	  tc.setLayer(      _tc_layer->at( itc )    );
	  tc.setWafer(      _tc_wafer->at( itc )    );
	  tc.setWaferType(  _tc_wafertype->at( itc ));
	  tc.setCell(       _tc_cell->at( itc )     );
	  tc.setData(       _tc_data->at( itc )     );
	  tc.setEnergy(     _tc_energy->at( itc )   );
	  tc.setEta(        _tc_eta->at( itc )      );
	  tc.setPhi(        _tc_phi->at( itc )      );
	  tc.setZ(          _tc_z->at( itc )        );
          
	  /* fill detector data */
	  this->addTC( tc );
                    
        }// end TCs LOOP
        
    }// end TCs

            
    /* Loop over C2D */
    if( _flagC2D ) {
                
        for(int iclu=0; iclu<_cl_n; iclu++){

            HGCC2D c2d;
            c2d.setPt(       _cl_pt->at(iclu)     );
	    c2d.setEnergy(   _cl_energy->at(iclu) );
	    c2d.setEta(      _cl_eta->at(iclu)    );
	    c2d.setPhi(      _cl_phi->at(iclu)    );
	    c2d.setLayer(    _cl_layer->at(iclu)  );
	    c2d.setCells(    _cl_cells->at(iclu)  );	    
	    /*c2d._x      =  _cl_x->at(iclu);
            c2d._y      =  _cl_y->at(iclu);
            c2d._z      =  _cl_z->at(iclu);*/

	    int subdet = -1;
	    if(_flagTCs){
	      HGCTC tc0 = this->getTC(c2d.cells()[0]);
	      subdet = tc0.subdet();
	    }	
	    c2d.setSubdet( subdet );
	    
            /* fill the detector */
            this->addC2D( c2d );
            
        }
    }// end C2D

}


void HGC::addTC( HGCTC tc ) { 

    const unsigned tcLayer = tc.correctedLayer();
    
    /* add tc to the map*/
    _TCs[tc.id()] = tc;
    
    /* keep the pointer for easy access */
    _TCtoStore.push_back( &_TCs[tc.id()] );
    _TC_layer[tcLayer].push_back( &_TCs[tc.id()] );

//    HGCROC TD( tc );
//
//    /* add to the correct HGCROC */
//    bool found = false;
//    for(unsigned int ihgcroc=0; ihgcroc<_TD[tcLayer]->size(); ihgcroc++ ){ 
//        if( _TD[tcLayer]->at(ihgcroc).tcIsContained( tc ) ){
//            found = true;
//            _TD[tcLayer]->at(ihgcroc).addTC( tc );
//            break;
//        }
//    }
//
//    if( !found ) _TD[tcLayer]->push_back( TD );
        
}

void HGC::addC2D(HGCC2D c2d) { 
    
    const unsigned c2dLayer = c2d.correctedLayer();
    
    /* add c2d to the map*/
    _C2Ds[c2d.id()] = c2d;
  
    /* keep the pointer for easy access */
    _C2DtoStore.push_back( &_C2Ds[c2d.id()] );    
    _C2D_layer[c2dLayer].push_back( &_C2Ds[c2d.id()] );

}


HGCTC HGC::getTC(unsigned ID){

  return _TCs[ID];

}

HGCC2D HGC::getC2D(unsigned ID){

  return _C2Ds[ID];

}


map<unsigned,HGCTC>  *HGC::getTCmap(){

    return &_TCs;

}


map<unsigned,HGCC2D> *HGC::getC2Dmap(){

    return &_C2Ds;

}


vector<HGCTC*>  HGC::getTC_layer(unsigned layer)  { 
    
    return _TC_layer[layer];

}


vector<HGCC2D*> HGC::getC2D_layer(unsigned layer) { 

  return _C2D_layer[layer];  

}


vector<HGCROC> *HGC::getTD(unsigned layer) { 
    
    return _TD[layer];  

}


vector<HGCTC*> HGC::getTCall() {  

    return _TCtoStore;

}

vector<HGCC2D*> HGC::getC2Dall() {  

    return _C2DtoStore;

}

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

}

