
/* mylibs */
#include "HGC.h"


HGC::HGC( TList *fileList, bool flagTCs, bool flagC2D, int verboselevel ) {
    
    /* init */
    _evt = 0;
    _verboselevel = verboselevel;
    _flagTCs = flagTCs;
    _flagC2D = flagC2D;
    
    for(unsigned i=0; i<Nlayers; i++) {
        _TC[i]  = new vector<HGCTC>  ;
        _C2D[i] = new vector<HGCC2D> ;
        _TD[i]  = new vector<HGCROC> ;
        _TC[i] ->reserve(10000); 
        _C2D[i]->reserve(600); 
        _TD[i] ->reserve(5000);
    }
    
    /* Get the Chain */
    _chain = new TChain("hgcalTriggerNtuplizer/HGCalTriggerNtuple");
    TIter next(fileList);
    while ( TObject *file = next() ){
        _chain->Add( ((TObjString*)file)->GetString() );
    }
    
    if( _flagTCs ){
        _chain->SetBranchAddress( "tc_n"        , &_tc_n         );
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
        delete _TC[i];
        delete _C2D[i];
        delete _TD[i];
    }

};
    
unsigned HGC::getEvents(){

    return _chain->GetEntries();

}

void HGC::getEvent(int evt){
        
    /* clear detector */
    this->clear();

    /* Get Entry */
    _chain->GetEntry( evt );        
    
    /* looping over TCs */
    if( _flagTCs ){
        
        ///* check internal consistency for TCs*/
        //if( _verboselevel >= 1 )
        //    cout << " >> there are " << _tc_n << " TCs in the event" << endl;
        //if( _tc_n != (int)_tc_energy->size() ){
        //    cout << " !!! Error !!! " << endl
        //         << " The number of TC expected doesn't match the number stored in the tree" << endl;
        //    return 0;
        //}
        
        /* LOOP */
        unsigned nTC = _tc_id->size();
        for( unsigned itc=0; itc<nTC; itc++ ){
            
            HGCTC tc;
            tc._id        = _tc_id->at( itc )       ;
            tc._subdet    = _tc_subdet->at( itc )   ;            
            tc._zside     = _tc_zside->at( itc )    ;
            tc._layer     = _tc_layer->at( itc )    ;
            tc._wafer     = _tc_wafer->at( itc )    ;
            tc._wafertype = _tc_wafertype->at( itc );
            tc._cell      = _tc_cell->at( itc )     ;
            tc._data      = _tc_data->at( itc )     ;
            tc._energy    = _tc_energy->at( itc )   ;
            tc._eta       = _tc_eta->at( itc )      ;
            tc._phi       = _tc_phi->at( itc )      ;
            tc._z         = _tc_z->at( itc )        ;
                
            /* fill detector data */
            this->addTC( tc );
                    
        }// end TCs LOOP
        
    }// end TCs
            
    /* Loop over C2D */
    if( _flagC2D ) {
                
        for(int iclu=0; iclu<_cl_n; iclu++){
            
            HGCC2D c2d;
            c2d._pt     =  _cl_pt->at(iclu);
            c2d._energy =  _cl_energy->at(iclu);
            c2d._eta    =  _cl_eta->at(iclu);
            c2d._phi    =  _cl_phi->at(iclu);
            c2d._x      =  _cl_x->at(iclu);
            c2d._y      =  _cl_y->at(iclu);
            c2d._z      =  _cl_z->at(iclu);
            c2d._layer  =  _cl_layer->at(iclu);
            c2d._ncells =  _cl_ncells->at(iclu);
            c2d._cells  =  _cl_cells->at(iclu);
            
            /* fill the detector */
            this->addC2D( c2d );
            
        }
    }// end C2D

}

void HGC::addTC( HGCTC tc ) { 

    const unsigned tcLayer = tc.getCorrectedLayer();
    _TC[tcLayer]->push_back( tc ); 
    HGCROC TD( tc );

    /* add to the correct HGCROC */
    bool found = false;
    for(unsigned int ihgcroc=0; ihgcroc<_TD[tcLayer]->size(); ihgcroc++ ){ 
        if( _TD[tcLayer]->at(ihgcroc).tcIsContained( tc ) ){
            found = true;
            _TD[tcLayer]->at(ihgcroc).addTC( tc );
            break;
        }
    }

    if( !found ) _TD[tcLayer]->push_back( TD );
        
}

void HGC::addC2D(HGCC2D c2d) { _C2D[c2d._layer]->push_back( c2d ); }

vector<HGCTC>  *HGC::getTC(unsigned layer)  { return _TC[layer];  }
vector<HGCC2D> *HGC::getC2D(unsigned layer) { return _C2D[layer]; }
vector<HGCROC> *HGC::getTD(unsigned layer)  { return _TD[layer];  }

void HGC::getTCall( vector<HGCTC> &data ) {  
    for(unsigned i=0; i<Nlayers; i++)
        data.insert(data.end(), _TC[i]->begin(), _TC[i]->end() );
}
void HGC::getC2Dall( vector<HGCC2D> &data ) {  
    for(unsigned i=0; i<Nlayers; i++)
        data.insert(data.end(), _C2D[i]->begin(), _C2D[i]->end() );
}
void HGC::getTDall( vector<HGCROC> &data ) {
    for(unsigned i=0; i<Nlayers; i++)
        data.insert(data.end(), _TD[i]->begin(), _TD[i]->end() );
}

void HGC::clear() {
    for(unsigned i=0; i<Nlayers; i++){
        _TC[i]->clear();
        _C2D[i]->clear();
        _TD[i]->clear();
    }    
}

