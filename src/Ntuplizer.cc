#include "Ntuplizer.h"


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

//    delete _tree;

}


void Ntuplizer::fillTree(){
    
    _tree->Fill();

}


void Ntuplizer::write(){

    _tree->Print();
    _tree->Write();
    _fileOut->Close();

}



