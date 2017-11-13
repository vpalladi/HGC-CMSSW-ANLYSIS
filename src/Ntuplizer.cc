#include "Ntuplizer.h"
#include <TLorentzVector.h>

Ntuplizer::Ntuplizer( HGC* detector, TString treename ){

    _fileOut = new TFile("test.root", "RECREATE");
    _tree = new TTree(treename, treename);
    _detector = detector;

  if(_detector->areTCpresent()){
        _tree->Branch("TC", detector->getTCmap() );
    }
  if(_detector->areC2Dpresent()){
        _tree->Branch("C2D", detector->getC2Dmap() );
    }
  
}


Ntuplizer::~Ntuplizer(){

}


void Ntuplizer::fillTree(){

  _tree->Fill();

}


void Ntuplizer::write(){

    _tree->Print();
    _tree->Write();
    _fileOut->Close();

}

/*      vector<HGCC2D> cls;
      _detector->getC2Dall( cls );

      _tc_cl.resize(_tc_pt.size(),-1);
      _tc_dR_cl.resize(_tc_pt.size(),-1);
      
      for( auto cl : cls ){	  

	TLorentzVector C2D_tlv = cl.P4();
	vector<unsigned int> TCs = cl.cells();	

	HGCTC tc0 = _detector->getTC(TCs[0]);
	_cl_HGClayer.emplace_back(tc0.correctedLayer());

	for(auto tc : TCs){

	  HGCTC TC = _detector->getTC(tc);
	  TLorentzVector TC_tlv = TC.P4();

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

	TLorentzVector C3D_tlv = c3d.P4();

	vector<unsigned int> TCs = c3d.cells();

	for(auto tc : TCs){

	  HGCTC TC = _detector->getTC(tc);
	  TLorentzVector TC_tlv = TC.P4();

	  int i_tc = _detector->getTC_index(tc);
	  _tc_c3d[i_tc] = _detector->getC3D_index(c3d.id());
	  _tc_dR_c3d[i_tc] = TC_tlv.DeltaR(C3D_tlv);

	}

      }

    }
    
*/


