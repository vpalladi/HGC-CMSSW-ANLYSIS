
/* C++ */
#include <iostream>
#include <vector>
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

const int verboseLevel = 2;
const bool flagTCs = true;
const bool flagC2D = true;
const bool histos = false;
const bool ntuple = true;

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
    HGC detector(fList, flagTCs, flagC2D, verboseLevel);

    /* TApplication */
    TApplication app("app", &argc, argv);

    if(histos){

      /* 1D Histograms */
      TH2D *hhLayerVsEnergy = new TH2D( "Layer vs Energy", "LayerVsEnergy", 
					53, -0.5, 52.5, //Layer
					100, 0, 100    // energy
					); 
      TH1D* hHGCROCmipT = new TH1D("HGCROCmipT", "HGCROCmipT", 1000, 0, 50);
      TH1D* hHGCROCenergy = new TH1D("HGCROCenergy", "HGCROCenergy", 1000, 0, 10);
      TH1D* hTCmipT = new TH1D("TCmipT", "TCmipT", 1000, 0, 50);
      TH1D* hTCperHGCROC = new TH1D("TCperHGCROC", "TCperHGCROC", 100, -0.5, 99.5);
      TH2D* hhTCperHGCROCvsMipT = new TH2D("TCperHGCROCvsMipT", "TCperHGCROCvsMipT", 
					   100, -0.5, 99.5,
					   1000, 0, 50 );
      TH2D* hhTCperHGCROCvsEnergy = new TH2D("TCperHGCROCvsEnergy", "TCperHGCROCvsEnergy", 
					     100, -0.5, 99.5,
					     1000, 0, 10 );
      TH2D* hhSeedsPercentVsNtcs = new TH2D("seedsPercentVsNtcs", "seedsPercentVsNtcs", 
					    100, 0, 1,
					    30, -0.5, 29.5);
      TH2D* hhSeedsPercentVsHGCROCmipT = new TH2D("seedsPercentVsHGCROCmipT", "seedsPercentVsHGCROCmipT", 
						  100, 0, 1,
						  1000, 0, 50 );
      TH2D* hhNSeedsVsNtcs = new TH2D("nSeedsVsNtcs", "nSeedsVsNtcs", 
				      30, -0.5, 29.5,
				      30, -0.5, 29.5 );
      // c2d
      TH1D* hC2Denergy = new TH1D("C2Denergy", "C2Denergy", 1000, 0, 100);
      TH1D* hC2DnCells = new TH1D("C2DnCells", "C2DnCells", 1000, -0.5, 999.5);
      TH2D* hhC2DptVsNcells = new TH2D("C2DptVsNcells", "C2DptVsNcells", 
				       100, 0, 20,
				       250 , -0.5, 249.5 );

      // tc in c2d
      TH1D* hTCinC2Did = new TH1D("TCinC2Did", "TCinC2Did", 100000, -0.5, 99999.5);
      TH1D* hTCinC2Dwafer = new TH1D("TCinC2Dwafer", "TCinC2Dwafer", 600, -0.5, 599.5);
      TH1D* hHGCROCperC2D= new TH1D("HGCROCperC2D", "HGCROCperC2D", 20, -0.5, 19.5);
      TH2D* hhC2DcentreVsNcells = new TH2D("C2DcentreVsNcells", "C2DcentreVsNcells", 
                                         1000, 0, 2000,
					   250 , -0.5, 249.5 );
      TH2D* hhC2Dcentres = new TH2D("C2Dcentres", "C2Dcentres", 
				    600, -300, 300,
				    600, -300, 300);
      TH2D* hhC2DcentresEtaPhi = new TH2D("C2DcentresEtaPhi", "C2DcentresEtaPhi", 
					  100, 0, 5,
					  100, 4, 4 );
      
      
      /* Store the pointers to object you wanna plot */
      vector<histoContainerGeneric*> histos = { 
        new histoContainer<TH2D> ( hhLayerVsEnergy             , TString("layer"),        TString("energy(GeV)"), TString("colz") ),
        new histoContainer<TH1D> ( hHGCROCmipT                 , TString("energy(MipT)"), TString("entries"), TString("") ),
        new histoContainer<TH1D> ( hHGCROCenergy               , TString("energy(GeV)"),  TString("entries"), TString("") ),
        new histoContainer<TH1D> ( hTCmipT                     , TString("energy(GeV)"),  TString("entries"), TString("") ),
        new histoContainer<TH1D> ( hTCperHGCROC                , TString("#TC"),          TString("entries"), TString("") ),
        new histoContainer<TH2D> ( hhTCperHGCROCvsMipT         , TString("#TC"),          TString("energy(MipT)"), TString("colz") ),
        new histoContainer<TH2D> ( hhTCperHGCROCvsEnergy       , TString("#TC"),          TString("energy(GeV)"), TString("colz") ),
        new histoContainer<TH2D> ( hhSeedsPercentVsNtcs        , TString("Seed Population(%)"), TString("#TC"), TString("colz") ),
        new histoContainer<TH2D> ( hhSeedsPercentVsHGCROCmipT  , TString("Seed Population(%)"), TString("energy(MIpT)"), TString("colz") ),
        new histoContainer<TH2D> ( hhNSeedsVsNtcs              , TString("#seeds"),       TString("#TC"), TString("colz") ),
        new histoContainer<TH1D> ( hC2Denergy                  , TString("energy(GeV)"),  TString(""),    TString("") ),
        new histoContainer<TH1D> ( hC2DnCells                  , TString("#TC"),          TString(""),    TString("") ),
        new histoContainer<TH2D> ( hhC2DptVsNcells             , TString("Pt(GeV)"),      TString("#TC"), TString("colz") ),
//        new histoContainer<TH1D> ( hTCinC2Did                  , TString("TCdetId"),      TString(""),    TString("") ), 
//        new histoContainer<TH1D> ( hTCinC2Dwafer               , TString("TCdetWafer"),   TString(""),    TString("") )
        new histoContainer<TH1D> ( hHGCROCperC2D               , TString("N HGCROC"),   TString(""),    TString("") ),
//        new histoContainer<TH2D> ( hhC2DcentreVsNcells         , TString("centre(????)"),   TString("N cells"),    TString("colz") ),
        new histoContainer<TH2D> ( hhC2Dcentres                , TString("X(cm)"),   TString("Y(cm)"),    TString("colz") ),
        new histoContainer<TH2D> ( hhC2DcentresEtaPhi          , TString("eta"),   TString("phi"),    TString("colz") ),
    };


    /* Loop Over Events */
    unsigned totalEvt = detector.getEvents();
    nEvt = (nEvt==-1) ? totalEvt : nEvt;

    for( int ievt=0; ievt<nEvt; ievt++ ){

        /* Get Entry */
        detector.getEvent( ievt );        
        if(verboseLevel >= 1)
            cout << " > Analyzing event No " << ievt << endl;
       
        
        /* looping over TCs */
        if( detector.areTCpresent() ){
                        
            /* get the vector */
            vector<HGCTC> tcs;
            detector.getTCall( tcs );
            
            /* LOOP */
            unsigned nTC = tcs.size();
            for( unsigned itc=0; itc<nTC; itc++ ){
                
                if( verboseLevel >= 2 ) {
                    float percent = 100.*(float)itc/(float)nTC; 
                    if( itc%1000 == 0 )
                        cout << "  > " << itc << " TC processed (" << percent << "%)" << endl;
                }
                     
                /* get the trigger cell */
                HGCTC tc = tcs.at(itc);

                /* DEBUG */
                if(verboseLevel >= 5){
                    cout << " >>> Energy (MipT) " << tc.MipT() << endl;
                    cout << " >>> HGROC id (third) " << tc.third() << endl;
                }
           
                /* Fill Histos */
                hhLayerVsEnergy->Fill( tc.correctedLayer(), tc.Energy() );
                hTCmipT->Fill( tc.MipT() );
            

            }// end TCs LOOP
            
            /* get HGCROC info */
            vector<HGCROC> towerData;
            detector.getTDall( towerData );
            for(unsigned int ihgcroc=0; ihgcroc<towerData.size(); ihgcroc++ ) {

                HGCROC roc = towerData.at(ihgcroc);

                float HGCROCmipt = roc.getMipT();
                float HGCROCenergy = roc.getEnergy();
                int TCsN = roc.getTCsN();
                
                /* fill histos*/
                hHGCROCmipT->Fill( HGCROCmipt );
                hHGCROCenergy->Fill( HGCROCenergy );
                hTCperHGCROC->Fill( TCsN );
                hhTCperHGCROCvsMipT->Fill( TCsN, HGCROCmipt );
                hhTCperHGCROCvsEnergy->Fill( TCsN, HGCROCenergy );
                hhSeedsPercentVsNtcs->Fill( roc.getTCsNifOverMipT(5)/(float)TCsN, TCsN );
                hhSeedsPercentVsHGCROCmipT->Fill( roc.getTCsNifOverMipT(5)/(float)TCsN, HGCROCmipt );
                hhNSeedsVsNtcs->Fill( roc.getTCsNifOverMipT(5), roc.getTCsInMipTrange(2, 5) );
            }
            
            /* tower data printout*/
            if( verboseLevel >= 3 ){
                cout << "Tower Data into the vector " << towerData.size() << endl;
                for(unsigned int ihgcroc=0; ihgcroc<towerData.size(); ihgcroc++ ) {
                    towerData.at(ihgcroc).print();
                }
            }
            
        }// end TCs
        
        /* Loop over C2D */
        if( detector.areC2Dpresent() ) {
            
            vector<HGCC2D> C2Ds;
            detector.getC2Dall( C2Ds );

            unsigned nC2D = C2Ds.size();

            if( verboseLevel >= 1 )
                cout << " >> there are " << nC2D << " C2D in the event" << endl;


            for(unsigned iclu=0; iclu<nC2D; iclu++){
                
                if( verboseLevel >= 2 ) {
                    float percent = 100.*(float)iclu/(float)nC2D; 
                    if( iclu%100 == 0 )
                        cout << "  > " << iclu << " C2D processed (" << percent << "%)" << endl;
                } 
            
                /* get the C2D */
                HGCC2D c2d = C2Ds.at( iclu );
                          
                /* fill the histos*/
                hC2Denergy->Fill( c2d.Energy() );
                hC2DnCells->Fill( c2d.ncells() );
                hhC2DptVsNcells->Fill( c2d.Pt(), c2d.ncells() );
                
                hHGCROCperC2D->Fill( c2d.HGCROCn() );
                hhC2DcentreVsNcells->Fill( TMath::Sqrt( c2d.Centre().X()*c2d.Centre().X() + c2d.Centre().Y()*c2d.Centre().Y() ), c2d.ncells() );
                //c2d.print();
                hhC2Dcentres->Fill(c2d.x(), c2d.y());
                hhC2DcentresEtaPhi->Fill(c2d.Eta(), c2d.Phi());
                
                /* loop over TC within the cluster */
                for (unsigned itc=0; itc<c2d.cells().size(); itc++){
		  HGCalDetId tcid( c2d.cells().at(itc) );
		  hTCinC2Did->Fill( c2d.cells().at(itc) );
                    hTCinC2Dwafer->Fill( tcid.wafer() );                    
                }
            }
        }// end C2D

        /* clear the detector at the end of the event */
//        detector.clear();

    }

    /* Draw ALL Histos */
    unsigned n_histos = histos.size();
    TCanvas canvases[n_histos];
    for(unsigned ihisto=0; ihisto<n_histos; ihisto++){
        canvases[ihisto].cd();
        histos.at(ihisto)->Draw();
    }
    
    /* Run the App (don't quit the graphycs) */
    app.Run();


    }


    

    if(ntuple){

      TFile* f_new = TFile::Open(outputFileName);
      if(f_new!=0){
	cout<<outputFileName<<" already exists, please delete it before converting again"<<endl;
	return 0;
      }
      
      f_new = TFile::Open(outputFileName,"RECREATE");

      Ntuplizer* ntp = new Ntuplizer(&detector,"HGCalTriggerNtuple_ext");
      unsigned totalEvt = detector.getEvents();   
      nEvt = (nEvt==-1) ? totalEvt : nEvt;
      ntp->fillTree(nEvt);
      TTree* tree = ntp->getTree();      
      
      tree->Write();
      f_new->Close();

    }


    return 0;

}


