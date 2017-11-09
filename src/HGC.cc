
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
      _chain->Add( ((TObjString*)file)->GetString() );
    }
    
    if( _flagTCs ){     
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
        _chain->SetBranchAddress("cl_id"     , &_cl_id     );
        _chain->SetBranchAddress("cl_pt"     , &_cl_pt     );
        _chain->SetBranchAddress("cl_energy" , &_cl_energy );
        _chain->SetBranchAddress("cl_eta"    , &_cl_eta    );
        _chain->SetBranchAddress("cl_phi"    , &_cl_phi    );       
        _chain->SetBranchAddress("cl_layer"  , &_cl_layer  );       
        _chain->SetBranchAddress("cl_cells"  , &_cl_cells  );
    }
    //C3D
    if( _flagC3D ) {       
        _chain->SetBranchAddress("cl3d_id"     , &_cl3d_id     );
        _chain->SetBranchAddress("cl3d_pt"     , &_cl3d_pt     );
        _chain->SetBranchAddress("cl3d_energy" , &_cl3d_energy );
        _chain->SetBranchAddress("cl3d_eta"    , &_cl3d_eta    );
        _chain->SetBranchAddress("cl3d_phi"    , &_cl3d_phi    );
        _chain->SetBranchAddress("cl3d_clusters"  , &_cl3d_clusters  );
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

void HGC::getEvent(int evt){
        
    /* clear detector */
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
        unsigned nclu = _cl_id->size();                
        for(unsigned int iclu=0; iclu<nclu; iclu++){

            HGCC2D c2d;
	    c2d.setId(       _cl_id->at(iclu)     );
            c2d.setPt(       _cl_pt->at(iclu)     );
	    c2d.setEnergy(   _cl_energy->at(iclu) );
	    c2d.setEta(      _cl_eta->at(iclu)    );
	    c2d.setPhi(      _cl_phi->at(iclu)    );
	    c2d.setLayer(    _cl_layer->at(iclu)  );
	    c2d.setCells(    _cl_cells->at(iclu)  );	    
	    
	    int subdet = -1;
	    float z = 0.;
	    if(_flagTCs){
	      HGCTC tc0 = this->getTC(c2d.cells()[0]);
	      subdet = tc0.subdet();
	      z = tc0.z();
	    }	
	    c2d.setSubdet( subdet );
	    c2d.setZ( z );
	    
            /* fill the detector */
            this->addC2D( c2d );
            
        }
    }// end C2D


    /* Loop over C3D */
    if( _flagC3D ) {
        unsigned nc3d = _cl3d_id->size();                
        for(unsigned int ic3d=0; ic3d<nc3d; ic3d++){

            HGCC3D c3d;
	    c3d.setId(       _cl3d_id->at(ic3d)     );
            c3d.setPt(       _cl3d_pt->at(ic3d)     );
	    c3d.setEnergy(   _cl3d_energy->at(ic3d) );
	    c3d.setEta(      _cl3d_eta->at(ic3d)    );
	    c3d.setPhi(      _cl3d_phi->at(ic3d)    );	   
	    c3d.setClusters(    _cl3d_clusters->at(ic3d)  );	    

	    if(_flagC2D){

	      vector<unsigned int> cl3d_cells;
	      for(auto iclu : c3d.clusters()){
		HGCC2D c2d = this->getC2D(iclu);
		vector<unsigned int> TCs = c2d.cells();
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
    _TC.emplace_back(tc);    
    _TC_map[tc.id()] = _TC.size()-1;
    _TC_layer[tcLayer].emplace_back( _TC.size()-1 );

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

void HGC::addC2D(HGCC2D c2d) { 

  _C2D.emplace_back(c2d);    
  _C2D_map[c2d.id()] = _C2D.size()-1;
  const unsigned c2dLayer = c2d.correctedLayer();
  _C2D_layer[c2dLayer].emplace_back( _C2D.size()-1 );

}

void HGC::addC3D(HGCC3D c3d) { 

  _C3D.emplace_back(c3d);    
  _C3D_map[c3d.id()] = _C3D.size()-1;

}

HGCTC HGC::getTC(unsigned int ID){

  return _TC[_TC_map[ID]];

}

HGCC2D HGC::getC2D(unsigned int ID){

  return _C2D[_C2D_map[ID]];

}

HGCC3D HGC::getC3D(unsigned int ID){

  return _C3D[_C3D_map[ID]];

}


unsigned int HGC::getTC_index(unsigned int ID){

  return _TC_map[ID];

}

unsigned int HGC::getC2D_index(unsigned int ID){

  return _C2D_map[ID];

}

unsigned int HGC::getC3D_index(unsigned int ID){

  return _C3D_map[ID];

}



vector<HGCTC>  HGC::getTC_layer(unsigned layer)  { 

  vector<HGCTC> TCs;
  for(auto i : _TC_layer[layer])
    TCs.emplace_back(_TC[i]);
  return TCs;  

}

vector<HGCC2D> HGC::getC2D_layer(unsigned layer) { 

  vector<HGCC2D> C2Ds;
  for(auto i : _C2D_layer[layer])
    C2Ds.emplace_back(_C2D[i]);
  return C2Ds;  

}

vector<HGCROC> *HGC::getTD(unsigned layer) { 
    
    return _TD[layer];  

}

void HGC::getTCall( vector<HGCTC> &data ) {  

  data = _TC;

}

void HGC::getC2Dall( vector<HGCC2D> &data ) {  

  data = _C2D;

}


void HGC::getC3Dall( vector<HGCC3D> &data ) {  

  data = _C3D;

}


void HGC::getTDall( vector<HGCROC> &data ) {

    for(unsigned i=0; i<Nlayers; i++)
        data.insert(data.end(), _TD[i]->begin(), _TD[i]->end() );

}

void HGC::clear() {

    for(unsigned i=0; i<Nlayers; i++){
        _TD[i]->clear();
    }    
    
    _TC.clear();
    _C2D.clear();
    _C3D.clear();
    _TC_map.clear();
    _C2D_map.clear();
    _C3D_map.clear();
    _TC_layer.clear();
    _C2D_layer.clear();


}

