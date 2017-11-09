#include "Ntuplizer.h"
#include <TLorentzVector.h>

Ntuplizer::Ntuplizer( HGC* detector, TString treename){

  _tree = new TTree(treename,treename);
  _detector = detector;

  if(_detector->areTCpresent()){
    _tree->Branch("tc_pt",     &_tc_pt);
    _tree->Branch("tc_HGClayer",     &_tc_HGClayer);
    _tree->Branch("tc_cl",      &_tc_cl);
    _tree->Branch("tc_dR_cl",   &_tc_dR_cl);    
    _tree->Branch("tc_c3d",     &_tc_c3d);
    _tree->Branch("tc_dR_c3d",  &_tc_dR_c3d);
  }

  if(_detector->areC2Dpresent()){
    _tree->Branch("cl_HGClayer",     &_cl_HGClayer);
    _tree->Branch("cl_cl3d",     &_cl_cl3d);
  }

}


void Ntuplizer::clear(){

  _tc_pt.clear();
  _tc_HGClayer.clear();
  _tc_cl.clear();
  _tc_dR_cl.clear();
  _tc_c3d.clear();
  _tc_dR_c3d.clear();

  _cl_HGClayer.clear();
  _cl_cl3d.clear();

}


void Ntuplizer::fillTree(unsigned nEvt){

  for(int i=0; i<nEvt; i++){

    this->clear();
    _detector->getEvent(i);
    
    if(_detector->areTCpresent()){

      vector<HGCTC> tcs;
      _detector->getTCall( tcs );
      
      for( auto tc : tcs ){
	  
	_tc_pt.emplace_back(tc.Pt());
	_tc_HGClayer.emplace_back(tc.correctedLayer());
	
      }      

    }

    
    if(_detector->areC2Dpresent()){

      vector<HGCC2D> cls;
      _detector->getC2Dall( cls );

      _tc_cl.resize(_tc_pt.size(),-1);
      _tc_dR_cl.resize(_tc_pt.size(),-1);
      
      for( auto cl : cls ){	  

	TLorentzVector C2D_tlv;
	C2D_tlv.SetPtEtaPhiE(cl.Pt(),cl.Eta(),cl.Phi(),cl.Energy());

	vector<unsigned int> TCs = cl.cells();	

	HGCTC tc0 = _detector->getTC(TCs[0]);
	_cl_HGClayer.emplace_back(tc0.correctedLayer());

	for(auto tc : TCs){

	  HGCTC TC = _detector->getTC(tc);
	  TLorentzVector TC_tlv;
	  TC_tlv.SetPtEtaPhiE(TC.Pt(),TC.Eta(),TC.Phi(),TC.Energy());

	  int i_tc = _detector->getTC_index(tc);
	  _tc_cl[i_tc] = _detector->getC2D_index(cl.id());
	  _tc_dR_cl[i_tc] = TC_tlv.DeltaR(C2D_tlv);

	}
	  	
      }   

    }

    if(_detector->areC3Dpresent()){

      vector<HGCC3D> c3ds;
      _detector->getC3Dall( c3ds );
      
      _tc_c3d.resize(_tc_pt.size(),-1);
      _tc_dR_c3d.resize(_tc_pt.size(),-1);

      for( auto c3d : c3ds ){	 

	TLorentzVector C3D_tlv;
	C3D_tlv.SetPtEtaPhiE(c3d.Pt(),c3d.Eta(),c3d.Phi(),c3d.Energy());

	vector<unsigned int> TCs = c3d.cells();

	for(auto tc : TCs){

	  HGCTC TC = _detector->getTC(tc);
	  TLorentzVector TC_tlv;
	  TC_tlv.SetPtEtaPhiE(TC.Pt(),TC.Eta(),TC.Phi(),TC.Energy());

	  int i_tc = _detector->getTC_index(tc);
	  _tc_c3d[i_tc] = _detector->getC3D_index(c3d.id());
	  _tc_dR_c3d[i_tc] = TC_tlv.DeltaR(C3D_tlv);

	}

      }

    }
    
    _tree->Fill();

  }


}
