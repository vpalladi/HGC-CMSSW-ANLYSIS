
/* C++ */
#include <iostream>
#include <vector>
#include <stdlib.h>

/* ROOT */
#include <TString.h>
#include <TFile.h>
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

using namespace std;

const int verboseLevel = 2;
const bool flagTCs = true;
const bool flagC2D = true;

int main(int argc, char **argv){

    /* PARSING THE COMMAND LINE ARGUMENTS */
    TString fName;
    int opt;
    int nEvt = -1;
    
    while ( (opt = getopt (argc, argv, "hn:f:")) != -1 ) {
        switch (opt)
        {
        case 'h':
            cout << "Help" << endl;
            break;
        case 'n':
            nEvt = atoi(optarg);
            break;
        case 'f':
            fName = optarg;
            break;
        case '?':
            cout << "unknown param" << endl;
            return 1;
        default:
            return 0;
        }
    }
    
    cout << "Options " << endl;
    cout << "  File Name: " << fName << endl;
    cout << "  Nevts: " << nEvt << endl;
    
    /* TApplication */
    TApplication app("app", &argc, argv);

    /* Open file */
    TFile f(fName, "READ");
    
    /* Get The Tree*/
    TTree* t = (TTree*)f.Get("hgcalTriggerNtuplizer/HGCalTriggerNtuple");
    
    /* Branches */
    // Trigger Cells
    int tc_n;
    vector<unsigned int> *tc_id        = 0;
    vector<int         > *tc_subdet    = 0;
    vector<int         > *tc_zside     = 0;
    vector<int         > *tc_layer     = 0;
    vector<int         > *tc_wafer     = 0;
    vector<int         > *tc_wafertype = 0;
    vector<int         > *tc_cell      = 0;
    vector<unsigned int> *tc_data      = 0;
    vector<float       > *tc_energy    = 0;
    vector<float       > *tc_eta       = 0;
    vector<float       > *tc_phi       = 0;
    vector<float       > *tc_z         = 0;
    // C2D
    int                           cl_n          ;
    vector<float>                 *cl_pt     = 0;
    vector<float>                 *cl_energy = 0;
    vector<float>                 *cl_eta    = 0;
    vector<float>                 *cl_phi    = 0;
    vector<int>                   *cl_layer  = 0;
    vector<int>                   *cl_ncells = 0;
    vector<vector<unsigned int> > *cl_cells  = 0;

    /* Set Branch Addresses*/
    // trigger cells
    if( flagTCs ){
        t->SetBranchAddress( "tc_n"        , &tc_n         );
        t->SetBranchAddress( "tc_id"       , &tc_id        );
        t->SetBranchAddress( "tc_subdet"   , &tc_subdet    );
        t->SetBranchAddress( "tc_zside"    , &tc_zside     );
        t->SetBranchAddress( "tc_layer"    , &tc_layer     );
        t->SetBranchAddress( "tc_wafer"    , &tc_wafer     );
        t->SetBranchAddress( "tc_wafertype", &tc_wafertype );
        t->SetBranchAddress( "tc_cell"     , &tc_cell      );
        t->SetBranchAddress( "tc_data"     , &tc_data      );
        t->SetBranchAddress( "tc_energy"   , &tc_energy    );
        t->SetBranchAddress( "tc_eta"      , &tc_eta       );
        t->SetBranchAddress( "tc_phi"      , &tc_phi       );
        t->SetBranchAddress( "tc_z"        , &tc_z         );
    }
    // C2D
    if( flagC2D ) {
        t->SetBranchAddress("cl_n"      , &cl_n      );
        t->SetBranchAddress("cl_pt"     , &cl_pt     );
        t->SetBranchAddress("cl_energy" , &cl_energy );
        t->SetBranchAddress("cl_eta"    , &cl_eta    );
        t->SetBranchAddress("cl_phi"    , &cl_phi    );
        t->SetBranchAddress("cl_layer"  , &cl_layer  );
        t->SetBranchAddress("cl_ncells" , &cl_ncells );
        t->SetBranchAddress("cl_cells"  , &cl_cells  );
    }

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
        new histoContainer<TH1D> ( hHGCROCperC2D               , TString("N HGCROC"),   TString(""),    TString("") )
    };

    /* THE DETECTOR */
    HGC detector;

    /* Loop Over Events */
    unsigned tEntries = t->GetEntries();
    nEvt = (nEvt==-1) ? tEntries : nEvt;

    for( int ievt=0; ievt<nEvt; ievt++ ){

        /* Get Entry */
        t->GetEntry( ievt );        
        if(verboseLevel >= 1)
            cout << " > Analyzing event No " << ievt << endl;
       
        
        /* looping over TCs */
        if( flagTCs ){
            
            /* check internal consistency for TCs*/
            if( verboseLevel >= 1 )
                cout << " >> there are " << tc_n << " TCs in the event" << endl;
            if( tc_n != (int)tc_energy->size() ){
                cout << " !!! Error !!! " << endl
                     << " The number of TC expected doesn't match the number stored " << endl;
                return 0;
            }
            
            /* LOOP */
            unsigned nTC = tc_id->size();
            for( unsigned itc=0; itc<tc_id->size(); itc++ ){
                if( verboseLevel >= 2 ) {
                    float percent = 100.*(float)itc/(float)nTC; 
                    if( itc%1000 == 0 )
                        cout << "  > " << itc << " TC processed (" << percent << "%)" << endl;
                }
                
                HGCTC tc;
                tc._id        = tc_id->at( itc )       ;
                tc._subdet    = tc_subdet->at( itc )   ;            
                tc._zside     = tc_zside->at( itc )    ;
                tc._layer     = tc_layer->at( itc )    ;
                tc._wafer     = tc_wafer->at( itc )    ;
                tc._wafertype = tc_wafertype->at( itc );
                tc._cell      = tc_cell->at( itc )     ;
                tc._data      = tc_data->at( itc )     ;
                tc._energy    = tc_energy->at( itc )   ;
                tc._eta       = tc_eta->at( itc )      ;
                tc._phi       = tc_phi->at( itc )      ;
                tc._z         = tc_z->at( itc )        ;
            
                /* fill detector data */
                detector.addTC( tc );
                
                /* DEBUG */
                if(verboseLevel >= 5){
                    cout << " >>> Energy (MipT) " << tc.getMipT() << endl;
                    cout << " >>> HGROC id (third) " << tc.getThird() << endl;
                }
           
                /* Fill Histos */
                hhLayerVsEnergy->Fill( tc.getCorrectedLayer(), tc._energy );
                hTCmipT->Fill( tc.getMipT() );

            }// end TCs LOOP
            
            /* get HGCROC info */
            vector<HGCROC> *towerData = detector.getTDall();
            for(unsigned int ihgcroc=0; ihgcroc<towerData->size(); ihgcroc++ ) {
                float HGCROCmipt = towerData->at(ihgcroc).getMipT();
                float HGCROCenergy = towerData->at(ihgcroc).getEnergy();
                int TCsN = towerData->at(ihgcroc).getTCsN();
                
                /* fill histos*/
                hHGCROCmipT->Fill( HGCROCmipt );
                hHGCROCenergy->Fill( HGCROCenergy );
                hTCperHGCROC->Fill( TCsN );
                hhTCperHGCROCvsMipT->Fill( TCsN, HGCROCmipt );
                hhTCperHGCROCvsEnergy->Fill( TCsN, HGCROCenergy );
                hhSeedsPercentVsNtcs->Fill( towerData->at(ihgcroc).getTCsNifOverMipT(5)/(float)TCsN, TCsN );
                hhSeedsPercentVsHGCROCmipT->Fill( towerData->at(ihgcroc).getTCsNifOverMipT(5)/(float)TCsN, HGCROCmipt );
                hhNSeedsVsNtcs->Fill( towerData->at(ihgcroc).getTCsNifOverMipT(5), towerData->at(ihgcroc).getTCsInMipTrange(2, 5) );
            }
            
            /* tower data printout*/
            if( verboseLevel >= 3 ){
                cout << "Tower Data into the vector " << towerData->size() << endl;
                for(unsigned int ihgcroc=0; ihgcroc<towerData->size(); ihgcroc++ ) {
                    towerData->at(ihgcroc).print();
                }
            }
            
        }// end TCs
        
        /* Loop over C2D */
        if( flagC2D ) {

            if( verboseLevel >= 1 )
                cout << " >> there are " << cl_n << " C2D in the event" << endl;

            for(int iclu=0; iclu<cl_n; iclu++){
                
                if( verboseLevel >= 2 ) {
                    float percent = 100.*(float)iclu/(float)cl_n; 
                    if( iclu%100 == 0 )
                        cout << "  > " << iclu << " C2D processed (" << percent << "%)" << endl;
                } 
                
                HGCC2D c2d;
                c2d._pt     =  cl_pt->at(iclu);
                c2d._energy =  cl_energy->at(iclu);
                c2d._eta    =  cl_eta->at(iclu);
                c2d._phi    =  cl_phi->at(iclu);
                c2d._layer  =  cl_layer->at(iclu);
                c2d._ncells =  cl_ncells->at(iclu);
                c2d._cells  =  cl_cells->at(iclu);
                
                /* fill the detector */
                detector.addC2D( c2d );
          
                /* fill the histos*/
                hC2Denergy->Fill( c2d._energy );
                hC2DnCells->Fill( c2d._ncells );
                hhC2DptVsNcells->Fill( c2d._pt, c2d._ncells );
                
                hHGCROCperC2D->Fill( c2d.getHGCROCn() );
                /* loop over TC within the cluster */
                for (unsigned itc=0; itc<c2d._cells.size(); itc++){
                    HGCalDetId tcid( c2d._cells.at(itc) );
                    hTCinC2Did->Fill( c2d._cells.at(itc) );
                    hTCinC2Dwafer->Fill( tcid.wafer() );                    
                }
            }
        }// end C2D

        /* clear the detector at the end of the event */
        detector.clear();

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

    return 0;

}


