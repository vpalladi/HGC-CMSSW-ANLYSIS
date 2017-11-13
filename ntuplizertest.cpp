
/* C++ */
#include <iostream>
#include <vector>
#include <map>

/* ROOT */
#include <TString.h>
#include <TFile.h>
#include <TTree.h>

/* myIncludes*/
#include "HGCTC.h"

using namespace std;

int main(int argc, char **argv){

    /* TEST */
    TTree t("t", "tree");
    HGCTC tc;
    map<int,HGCTC> mapHGCTC;
    t.Branch("TCmap", &mapHGCTC);

    for( int ievt=0; ievt<100; ievt++ ){
        
        HGCTC tc_tree;
        mapHGCTC[ievt] = tc_tree; 
        t.Fill();

    }// end of evt loop

    TFile f("test.root", "RECREATE");
   
    t.Print();
    t.Write();
    
    f.Close();


//    ntu.write();


//
////      Ntuplizer* ntp = new Ntuplizer(&detector,"HGCalTriggerNtuple_ext");
//      unsigned totalEvt = detector.getEvents();   
//      nEvt = (nEvt==-1) ? totalEvt : nEvt;
//      ntp->fillTree(nEvt);
//      TTree* tree = ntp->getTree();      
//      
//      tree->Write();
//      f_new->Close();
//
//    }



    return 0;

}


