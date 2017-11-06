#include "Ntuplizer.h"


Ntuplizer::Ntuplizer( HGC* detector, TString treename){

  _tree = new TTree(treename,treename);
  _detector = detector;

  if(_detector->areTCpresent()){
    _tree->Branch("tc_pt",     &_tc_pt);
    _tree->Branch("tc_HGClayer",     &_tc_HGClayer);
  }

  if(_detector->areC2Dpresent()){
    _tree->Branch("cl_HGClayer",     &_cl_HGClayer);
  }

}


void Ntuplizer::clear(){

  _tc_pt.clear();
  _tc_HGClayer.clear();
  _cl_HGClayer.clear();

}


void Ntuplizer::fillTree(unsigned nEvt){

  for(unsigned i=0; i<nEvt; i++){

    this->clear();
    _detector->getEvent(i);
    
    if(_detector->areTCpresent()){

      vector<HGCTC> tcs;
      _detector->getTCall( tcs );
      
      for( unsigned itc=0; itc<tcs.size(); itc++ ){	  
	
	HGCTC tc = tcs[itc];	  	  
	_tc_pt.emplace_back(tc.Pt());
	_tc_HGClayer.emplace_back(tc.correctedLayer());
	
      }      

    }

    
    if(_detector->areC2Dpresent()){

      vector<HGCC2D> cls;
      _detector->getC2Dall( cls );
      
      for( unsigned icl=0; icl<cls.size(); icl++ ){	  
	
	HGCC2D cl = cls[icl];
	HGCTC tc0 = _detector->getTC(cl.cells()[0]);
	_cl_HGClayer.emplace_back(tc0.correctedLayer());
 	  	
      }   

    }
    
    _tree->Fill();

  }


}
