#include "Ntuplizer.h"
#include "TROOT.h"

Ntuplizer::Ntuplizer( HGC* detector, TString treename ){

    gROOT->ProcessLine("#include <map>");

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

//    delete _tree;

}


void Ntuplizer::fillTree(){
    
//    for(unsigned i=0; i<nEvt; i++){
//        
//        _detector->getEvent(i);
//        
//        if(_detector->areTCpresent()){
//
//            vector<HGCTC*> tcs = _detector->getTCall();
//      
//            for( unsigned itc=0; itc<tcs.size(); itc++ ){	  
//	
//                HGCTC tc = tcs[itc];	  	  
//                _tc_pt.emplace_back(tc.Pt());
//                _tc_HGClayer.emplace_back(tc.correctedLayer());
//
//            }
//
//        }

    
//        if(_detector->areC2Dpresent()){
//
//            vector<HGCC2D> cls;
//            _detector->getC2Dall( cls );
//      
//            for( unsigned icl=0; icl<cls.size(); icl++ ){	  
//	
//                HGCC2D cl = cls[icl];
//                HGCTC tc0 = _detector->getTC(cl.cells()[0]);
//                _cl_HGClayer.emplace_back(tc0.correctedLayer());
// 	  	
//            }   
//        }
    
        _tree->Fill();

//    }

}


void Ntuplizer::write(){

    _tree->Print();
    _tree->Write();
    _fileOut->Close();

}



/* Private members */

void Ntuplizer::clear(){

//    _tc_pt.clear();
//    _tc_HGClayer.clear();
//    _cl_HGClayer.clear();

}


