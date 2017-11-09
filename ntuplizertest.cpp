
/* C++ */
#include <iostream>
#include <vector>
#include <map>
#include <stdlib.h>
#include <fstream>


/* ROOT */
#include <TString.h>
#include <TFile.h>
#include <TChain.h>
#include <TTree.h>
#include <TApplication.h>
#include <TH1.h>
#include <TH2.h>
#include <TCanvas.h>
#include <TList.h>

/* myIncludes*/
#include "HistoContainer.h"
#include "HGC.h"
#include "HGCTC.h"
#include "HGCC2D.h"
#include "HGCROC.h"
#include "Ntuplizer.h"

using namespace std;

const int verboselevel = 3;
const bool flagTCs = true;
const bool flagC2D = false;
//const bool flagHistos = false;
const bool flagNtuple = false;

int main(int argc, char **argv){

    /* PARSING THE COMMAND LINE ARGUMENTS */
    TString fFileListName;
    int opt;
    int nEvt = -1;
    TString inputFileName;
    TString outputFileName;
    
    while ( (opt = getopt (argc, argv, "hn:f:i:o:")) != -1 ) {
        switch (opt)
        {
        case 'h':
            cout << "Help" << endl;
            break;
        case 'n':
            nEvt = atoi(optarg);
            break;
        case 'f':
            fFileListName = optarg;
            break;
        case 'i':
            inputFileName = optarg;
            break;
	case 'o':
            outputFileName = optarg;
            break;
        case '?':
            cout << "unknown param" << endl;
            return 1;
        default:
            return 0;
        }
    }
    
    cout << "Options " << endl;
    cout << "  File List Name: " << fFileListName << endl;
    cout << "  Nevts: " << nEvt << endl;
    cout << "  OutputFile: " << outputFileName <<endl;
    cout << "  InputFile: " << inputFileName <<endl;
	    
    /* get the files and fill the detector*/
    
    TList *fList = new TList();

    if(fFileListName!=""){
        ifstream fFileList(fFileListName);    
        string line;
        while( getline(fFileList,line) ){
            fList->Add( new TObjString( TString(line) ) );
        }
    }
    else
        fList->Add( new TObjString(inputFileName) );

    /* build the detector */
    HGC detector(fList, flagTCs, flagC2D, verboselevel);

    /* output file */
    //TFile f_new = TFile::Open(outputFileName,"RECREATE");

    /* TEST */
    TTree t("t", "tree");
    HGCTC tc;
    map<int,HGCTC> mapHGCTC;
    t.Branch("TCmap", &mapHGCTC);
    t.Branch("TC", &tc);

    /* ntuplizer */
    //Ntuplizer ntu( &detector, "HGCalTriggerNtuple_ext");
    
    /* Loop Over Events */
    unsigned totalEvt = detector.getEvents();
    nEvt = (nEvt==-1) ? totalEvt : nEvt;
    if( verboselevel >=3 )
        cout << " MAIN >> The loop will be performed on " << nEvt << " events" << endl;

    for( int ievt=0; ievt<nEvt; ievt++ ){
        
        HGCTC tc_tree;
        mapHGCTC[ievt] = tc_tree; 
        t.Fill();

        /* Get Entry */
        detector.getEvent( ievt );        
        if(verboselevel >= 1)
            cout << " MAIN >> Analyzing event No " << ievt << endl;

        //  ntu.fillTree();
        
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


