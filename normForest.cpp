
/* C++ */
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <fstream>
#include <getopt.h>

/* ROOT */
#include <TString.h>
#include <TFile.h>
#include <TChain.h>
#include <TTree.h>
#include <TApplication.h>
#include <TH1.h>
#include <TH2.h>
#include <TGraph.h>
#include <TMultiGraph.h>
#include <TCanvas.h>
#include <TList.h>
#include <TF1.h>
#include <TLine.h>

/* myIncludes*/
#include "HGCgeom.h"
#include "HistoContainer.h"
#include "HGC.h"
#include "HGCTC.h"
#include "HGCC2D.h"
#include "HGCROC.h"
//#include "Ntuplizer.h"
#include "HGCht.h"
#include "HGCpolarHisto_T.h"



using namespace std;

const unsigned VERBOSE_OPTIONS = 0x01;
const unsigned VERBOSE_MAIN    = 0x02;
const unsigned VERBOSE_MAIN2   = 0x04;
const unsigned VERBOSE_MAIN3   = 0x08;
const unsigned VERBOSE_TC      = 0x20;
const unsigned VERBOSE_C2D     = 0x40;
const unsigned VERBOSE_C3D     = 0x80;

const int verboselevel = VERBOSE_OPTIONS | VERBOSE_MAIN ;
const bool flagTCs = true;
const bool flagC2D = true;
const bool flagC3D = true;
const bool flagGen = true;
const bool flagGenPart = false;
//const bool flagHistos = false;
const bool flagNtuple = false;



bool isBit(unsigned word, unsigned bit) { 
    
    if( (word & bit) == bit )
        return true;
    return false;

}


int main(int argc, char **argv){

    /* PARSING THE COMMAND LINE ARGUMENTS */
    int opt;

    // files and event control
    TString fFileListName;
    TString inputFileName;
    TString outputFileName("tmp.root");
    int nEvt = -1;
    int firstEvent=0;
    unsigned nPhiSectors=32; 
    unsigned nLongitudinalSections=4;
    unsigned nNNsearch=9, nNNsum=0;
    float c3dRadius=0.1;
    bool saveEventByEvent = false;
    TString newC3DsStrategy = "none";

    // graphics
    bool persistentGraphics=false;

    const struct option longOptions[] = {
        {"help",         no_argument,        0, 'h'},
        {"fileList",     required_argument,  0, 'f'},
        {"iFile",        required_argument,  0, 'i'},
        {"oFile",        required_argument,  0, 'o'},
        {"firstEvent",   required_argument,  0, 'd'},
        {"nEvt",         required_argument,  0, 'n'},
        {"nPhiSectors",  required_argument,  0, 's'},
        {"nNNsearch",    required_argument,  0, 'v'},
        {"nNNsum",       required_argument,  0, 'a'},
        {"saveEvtByEvt", required_argument,  0, 'e'},
        {"strategy"    , required_argument,  0, 'g'},
        {0,0,0,0}
    };

    int optIndex = 0;
    while ( (opt = getopt_long (argc, argv, "hf:i:o:d:n:s:v:r:eg:", longOptions, &optIndex) ) != -1 ) {
        
        switch (opt)
        {
        case 'h':
            cout << "Help" << endl;
            cout << " -h(--help        ) :\t shows this help." << endl; 
            cout << " -f(--fileList    ) <files> :\t list of files to be processed." << endl; 
            cout << " -i(--iFile       ) <ifile> :\t single file input." << endl; 
            cout << " -o(--oFile       ) <ofile> :\t output file." << endl; 
            cout << " -d(--firstEvent  ) <firstEvent> :\t first event to be processed (default: 0)." << endl;
            cout << " -n(--nEvt        ) <nEvt> :\t number of events to be processed (default: all)." << endl;
            cout << " -s(--nPhiSectors ) <nPhiSectors> :\t number of phi sectors (default: 32)." << endl;
            cout << " -v(--nNNsearch   ) <nNNsearch> :\t norm coo strategy, number of bins around the maxima to search." << endl;
            //cout << " -a(--nNNsum      ) <nNNsum> :\t norm coo strategy, number of bins around the maxima to sum," << endl;
            cout << " -r(--c3dRadius   ) <c3dradius> :\t radius (in norm coo) to clusterise the norm tranform," << endl;
            cout << " -e(--saveEvtByEvt) <saveEvtByEvt> :\t saves info event-by-event base, WARNING large output files." << endl;
            cout << " -g(--strategy    ) <strategy> :\t defines which strategy to adopt for the new C3Ds ('forest', 'norm', 'single', 'gen', 'none', 'polarFW', 'polarFWtc')." << endl;
            return 0;
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
	case 'n':
            nEvt = atoi( optarg );
            break;
        case 'd':
            firstEvent = atoi( optarg );
            break;            
        case 's':
            nPhiSectors = atoi( optarg );
            break;
        case 'v':
            nNNsearch = atoi( optarg );
            break;
        case 'r':
            c3dRadius = atof( optarg );
            break;
        case 'e':
            saveEventByEvent = true;
            break;
        case 'g':
            newC3DsStrategy = optarg;
            break;
        case '?':
            cout << "unknown param" << endl;
            return 1;
        default:
            return 0;
        }
    }
    
    if( isBit(verboselevel, VERBOSE_OPTIONS) ){
        cout << "Options "           << endl;
        cout << "  File List Name: " << fFileListName  << endl;
        cout << "  firstEvent: "     << firstEvent     << endl;
        cout << "  nEvts: "          << nEvt           << endl;
        cout << "  OutputFile: "     << outputFileName << endl;
        cout << "  InputFile: "      << inputFileName  << endl;
        cout << "  nPhiSectors: "    << nPhiSectors    << endl;
        cout << "  nNNsearch: "      << nNNsearch      << endl;
        cout << "  nNNsum: "         << nNNsum         << endl;
        cout << "  newC3strategy: "  << newC3DsStrategy<< endl;
    }

    /****************/
    /* OPEN fileOUT */

    TFile* fileOut = new TFile( outputFileName, "recreate" ); 
    

    /***************************************/
    /* get the files and fill the detector */

    TList *fList = new TList();

    if( fFileListName != "" ) {
        ifstream fFileList( fFileListName );    
        string line;
        while( getline( fFileList, line ) ){
            fList->Add( new TObjString( TString(line) ) );
        }
    }
    else {
        fList->Add( new TObjString( inputFileName ) );
    }

    /* build the detector */
    HGC detector( fList, 
                  flagTCs, flagC2D, flagC3D, flagGen, flagGenPart, 
                  true,
                  verboselevel 
        );   

   

    /* implement the Ntuplizer */
    //    Ntuplizer ntupl( &detector, "myTree" );

    /****************/
    /* TApplication */

    TApplication app( "app", &argc, argv );
   

    /*************************/
    /* Histograms and graphs */
    
    // gen 
    TGraph gGenProjXY[HGCgeom::instance()->nEndcaps()][nLongitudinalSections][nPhiSectors];
    TGraph gGenProjXYnorm[HGCgeom::instance()->nEndcaps()][nLongitudinalSections][nPhiSectors];
    
    // genpart

    // triggercells

    // C2D

    // C3D
    double nC3D[nEvt][HGCgeom::instance()->nEndcaps()];
    TGraph gC3DProjXYnorm[HGCgeom::instance()->nEndcaps()][nLongitudinalSections][nPhiSectors];

    /*********/
    /* trees */
    TTree *tNewC3Ds = new TTree( "newC3Ds", "newC3Ds" );
//    TTree *tForestC3Ds = new TTree( "forestC3Ds", "forestC3Ds" );
//    TTree *tC3DsSingle = new TTree( "C3DsSingle", "C3DsSingle" );

    vector<HGCC3D> newC3Ds[2];
//    vector<HGCC3D> forestC3Ds[2];
//    vector<HGCC3D> C3DsSingle[2];

    tNewC3Ds->Branch( "endcap0", &newC3Ds[0], 64000, 1);
    tNewC3Ds->Branch( "endcap1", &newC3Ds[1], 64000, 1);
//    tForestC3Ds->Branch( "endcap0", &forestC3Ds[0], 64000, 1);
//    tForestC3Ds->Branch( "endcap1", &forestC3Ds[1], 64000, 1);
//    tC3DsSingle->Branch( "endcap0", &C3DsSingle[0], 64000, 1);
//    tC3DsSingle->Branch( "endcap1", &C3DsSingle[1], 64000, 1);

    /***************/
    /* phi sectors */
    double minPhis[nPhiSectors];
    double maxPhis[nPhiSectors];
    
    for(unsigned isector=0; isector<nPhiSectors; isector++){
        double pi = TMath::Pi();
//        double dPhi = ( 2*pi )/( 3*nPhiSectors );// overlap
        double dPhi = ( 2*pi )/( nPhiSectors ); // NO overlap
        
//        minPhis[isector] = dPhi * (1+3*isector) - pi - 2*dPhi; // overlap
//        maxPhis[isector] = dPhi * (1+3*isector) - pi + 2*dPhi;
        minPhis[isector] = - pi + dPhi * (isector) ; // NO overlap
        maxPhis[isector] = - pi + dPhi * (isector+1) ;

    }       

    /********************/
    /* Loop Over Events */    
    unsigned totalEvt = detector.getEvents();
    nEvt = (nEvt==-1) ? totalEvt : nEvt;
    if( isBit(verboselevel, VERBOSE_MAIN) )
        cout << " MAIN >> Looping over " << nEvt << " events" << endl;

    for( int ievt=firstEvent; ievt<(firstEvent+nEvt); ievt++ ){

        /* loop based variables */

        if( isBit(verboselevel, VERBOSE_MAIN) )
            cout << " MAIN >> Analyzing event No " << ievt << endl;

        /******BUILD DIR HIERARCHY******/
        /* make a directory for this event and the phi sector in iFile */
        TDirectory* eventDir;
        if( saveEventByEvent ){
            eventDir = fileOut->mkdir( Form("event_%d", ievt) );
            
            fileOut->mkdir( Form("event_%d/endcap_0/", ievt) );
            fileOut->mkdir( Form("event_%d/endcap_1/", ievt) );
            
            for(unsigned isection=0; isection<nLongitudinalSections; isection++){
                // new folder for phi sector
                fileOut->mkdir( Form("event_%d/endcap_0/section_%d/", ievt, isection) );
                fileOut->mkdir( Form("event_%d/endcap_1/section_%d/", ievt, isection) );
                
                for(unsigned isector=0; isector<nPhiSectors; isector++){   
                    fileOut->mkdir( Form("event_%d/endcap_0/section_%d/sector_%d/", ievt, isection, isector) );
                    fileOut->mkdir( Form("event_%d/endcap_1/section_%d/sector_%d/", ievt, isection, isector) );
                    
                }
                
            }
            
            eventDir->cd();
            
        }
        
        /* Get Entry */
        detector.getEvent( ievt );

        /************************/
        /* looping over the GEN */
        if( detector.areGenPresent() ){
        
            vector<HGCgen*> gens = detector.getGenAll();
            
            for(unsigned igen=0; igen<gens.size(); igen++) {

                HGCgen* gen = gens.at(igen);
                //cout << gen->Eta() << " " << gen->getEndcapId() << endl;
                //if( !( gen->Eta()>1.5 && gen->Eta()<3 ) ) continue;

                /* store the gen part into the correct section and phi sector */
                for(unsigned isection=0; isection<nLongitudinalSections; isection++){
                                        
                    unsigned iendcap = gen->getEndcapId();
                    unsigned isector = gen->getPhiSectorProj( nPhiSectors, minPhis, maxPhis, HGCgeom::instance()->layerZ(iendcap, 1) );
                    gGenProjXY[iendcap][isection][isector].SetPoint( gGenProjXY[iendcap][isection][isector].GetN(),
                                                                     gen->getZprojection( HGCgeom::instance()->layerZ(iendcap, 1) ).X(), 
                                                                     gen->getZprojection( HGCgeom::instance()->layerZ(iendcap, 1) ).Y()
                        );
                    
                    gGenProjXYnorm[iendcap][isection][isector].SetPoint( gGenProjXYnorm[iendcap][isection][isector].GetN(),
                                                                         gen->xNorm(), 
                                                                         gen->yNorm()
                        );        
                }
                
            } // end gen loop
            
            for(unsigned iendcap=0; iendcap<HGCgeom::instance()->nEndcaps(); iendcap++) {
                for(unsigned isection=0; isection<nLongitudinalSections; isection++) {
                    for(unsigned isector=0; isector<nPhiSectors; isector++){
                        
                        if( gGenProjXY[iendcap][isection][isector].GetN() == 0 ) continue;

                        if( saveEventByEvent ){
                            fileOut->cd( Form("event_%d/endcap_%d/section_%d/sector_%d/",
                                              ievt, 
                                              iendcap,
                                              isection,
                                              isector )
                                );
                            gGenProjXY[iendcap][isection][isector].Write("gen_xy_proj"); // save the graph
                            gGenProjXYnorm[iendcap][isection][isector].Write("gen_xy_proj_norm"); // save the graph
                        }
                            gGenProjXY[iendcap][isection][isector].Set(0); // clean the graph
                            gGenProjXYnorm[iendcap][isection][isector].Set(0); // clean the graph
                    
                    }
                }
            }
            
        }


        /************************/
        /* looping over the GENPART */


        /********************/
        /* looping over TCs */
        if( detector.areTCpresent() ){
            
            if( isBit(verboselevel, VERBOSE_MAIN) )
                cout << " MAIN >> TC analysis " << endl;
     
            /* endcap loop */
            for(unsigned iendcap=0; iendcap<2; iendcap++) {
                
                /* section loop */
                for(unsigned isection=0; isection<nLongitudinalSections; isection++) {

                    /* get tcs vector */
                    vector<const HGCTC*> tcs = detector.getSubdet( iendcap, isection )->getAll<HGCTC>();
                    
                    /* LOOP */
                    unsigned nTC = tcs.size();
                    for( unsigned itc=0; itc<nTC; itc++ ){
                        
                        if( isBit(verboselevel, VERBOSE_TC) ) {
                            float percent = 100.*(float)itc/(float)nTC; 
                            if( itc%1000 == 0 )
                                cout << " MAIN >> " << itc << " TC processed (" << percent << "%)" << endl;
                        }
                    
                        /* get the trigger cell */
                        const HGCTC* tc = tcs.at(itc);
                    
                        /* DEBUG */
                        if( isBit(verboselevel, VERBOSE_TC) ) {
                            cout << " MAIN >> Energy (MipT) " << tc->MipT() << endl;
                            cout << " MAIN >> HGROC id (third) " << tc->third() << endl;
                        }
                    
                        /* Fill Histos and Graphs*/
                        //hhTCetaPhi->Fill( tc->Eta(), tc->Phi() );
                        //if( tc->MipT() > 5 ) { 
                        //    gTCrVsPhi->SetPoint( gTCrVsPhi->GetN(), tc->Phi(), tc->r() );
                        //    gTCrVsPhi_SL[tc->layer()].SetPoint( gTCrVsPhi_SL[tc->layer()].GetN(), tc->Phi(), tc->r() );
                        //}
                        //                hhLayerVsEnergy->Fill( tc->correctedLayer(), tc->Energy() );
                        //hTCmipT->Fill( tc->MipT() );

                    }// end sections loop
                    
                }// end encap loop
                    

            }// end TCs LOOP

        }// end TCs


        /************************/
        /* looping over the C2D */
        if( detector.areC2Dpresent() ) {

            cout << " MAIN >>> C2D analysis." << endl;

            /* endcap loop */
            for(unsigned iendcap=0; iendcap<HGCgeom::instance()->nEndcaps(); iendcap++) {

                /* section loop */
                for(unsigned isection=0; isection<nLongitudinalSections; isection++) {

                    /* point the correct location */
                    if( saveEventByEvent )  fileOut->cd( Form("event_%d/endcap_%d/section_%d", ievt, iendcap, isection) );

                    /* get subdetector */
                    HGCsubdet* subdetector = detector.getSubdet(iendcap, isection);
                    
                    /* get C2Ds projection */
                    TGraph gC2Dxy;
                    subdetector->getC2DallInPhiRegion(-4, 4, gC2Dxy );
     
                    if( saveEventByEvent )  gC2Dxy.Write( Form("C2D_xy_proj_endcap_%d", iendcap) ); // save the graph
                    

                    /* loop over PHI sectors */
                    
                    /* get the C2D in the phi sector */
                    //int totPhiSec =0;
                    //for(unsigned isector=0; isector<nPhiSectors; isector++){
                    //    C2Ds[isector] = subdetector->getC2DallInPhiRegion(minPhis[isector], maxPhis[isector]);
                    //    totPhiSec+=C2Ds[isector].size();
                    //}
                    
                    for(unsigned isector=0; isector<nPhiSectors; isector++){
                        
                        /* get the right folder */
                        if( saveEventByEvent )  fileOut->cd( Form("event_%d/endcap_%d/section_%d/sector_%d", ievt, iendcap, isection, isector) );
                        
                        /*** C2Ds ***/
                        
                        /* c2d projection graphs */
                        TGraph gC2DxyPhi;
                        subdetector->getC2DallInPhiRegion( minPhis[isector], maxPhis[isector], gC2DxyPhi );
                        if( saveEventByEvent )  gC2DxyPhi.Write( "C2D_xy_proj" ); // save the graph                        
                       
                        /*** NORM COORDINATES ***/
                        HGCnorm normTransform = detector.getSubdet(iendcap, isection)->getNormTransform_C2D( 200, -0.6, 0.6,
                                                                                                             200, -0.6, 0.6,
                                                                                                             minPhis[isector], maxPhis[isector], 
                                                                                                             1
                                                                                                             
                            );
                        //TH2D hNorm;
                        //normTransform.getTransformedHisto( hNorm );
                        //if( saveEventByEvent )  hNorm.Write(Form("norm_%d",    isector));

                        if( isection ==3 ) {
                            /* norm */
                            if( newC3DsStrategy == "norm" ){
                                HGCnorm normTransformC3D = detector.getSubdet(iendcap, isection)->getNormTransform_C3D( 200, -0.6, 0.6,
                                                                                                                        200, -0.6, 0.6,
                                                                                                                        minPhis[isector], maxPhis[isector], 
                                                                                                                        1//, nNNsearch, c3dRadius,
                                                                                                                        //detector.getGenAll()
                                    );
                                
                                //TH2D hNormC3D, hNormC3Dmaxima;
                                //normTransformC3D.getTransformedHisto( hNormC3D );
                                //hNormC3D.Write(Form("normC3D_%d",    isector));
                                
                                //normTransformC3D.getHistoLocalMaxima( hNormC3Dmaxima );
                                //hNormC3Dmaxima.Write(Form("normC3Dmaxima_%d",    isector));
                                
                                newC3Ds[iendcap] = normTransformC3D.getNewC3Ds(nNNsearch, c3dRadius,
                                                                               detector.getGenAll()
                                    );
                                for(unsigned ic3d=0; ic3d<newC3Ds[iendcap].size(); ic3d++) {
                                    gC3DProjXYnorm[iendcap][isection][isector].SetPoint( gC3DProjXYnorm[iendcap][isection][isector].GetN(),
                                                                                         newC3Ds[iendcap].at(ic3d).xNorm(), 
                                                                                         newC3Ds[iendcap].at(ic3d).yNorm()
                                        );
                                    newC3Ds[iendcap].at(ic3d).setNearestGen( detector.getGenAll() );
                                    
                                }
                                gC3DProjXYnorm[iendcap][isection][isector].Write("c3d_xy_proj_norm");
                                
                            }
                            
                            /* forest */
                            if( newC3DsStrategy == "forest" ){
                            
                                HGCforest forestC3D = detector.getSubdet(iendcap, isection)->getNormTransform_C3Dforest( 200, -0.6, 0.6,
                                                                                                                         200, -0.6, 0.6,
                                                                                                                         minPhis[isector], maxPhis[isector], 
                                                                                                                         1 
                                    );
                                
                                //TH2D hNormC3Dforest;
                                //normTransformC3D.getTransformedHisto( hNormC3Dforest, 1 );
                                //hNormC3Dforest.Write(Form("normC3Dforest_%d",    isector));
                                
                                newC3Ds[iendcap] = forestC3D.getNewC3Ds();
                                //detector.getSubdet(iendcap, isection)->fillTree();
                                for(unsigned ic3d=0; ic3d<newC3Ds[iendcap].size(); ic3d++) {
                                    newC3Ds[iendcap].at(ic3d).setNearestGen( detector.getGenAll() );
                                }
                            }

                            /* C3Ds All (1c3d per event) */
                            if( newC3DsStrategy == "single" ){

                                HGCsingleC3D singleC3D = detector.getSubdet(iendcap, isection)->getSingleC3D();
                                
                                newC3Ds[iendcap] = singleC3D.getNewC3Ds();

                            }

                            /* gen */
                            if( newC3DsStrategy == "gen" ){

                                HGCC3Dgen C3Dgen = detector.getSubdet(iendcap, isection)->getGenC3D( c3dRadius );
                                
                                newC3Ds[iendcap] = C3Dgen.getNewC3Ds();

                            }

                            /* polarFW */
                            unsigned binSums[36] = { 
                                13,                                           // 0
                                11, 11, 11,                                   // 1 - 3
                                9, 9, 9,                                      // 4 - 6
                                7, 7, 7, 7, 7, 7,                             // 7 - 12
                                5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,  //13 - 27
                                3, 3, 3, 3, 3, 3, 3, 3                        //28 - 35
                            };
                            if( newC3DsStrategy == "polarFW" ){

                                HGCpolarHisto<HGCC2D> grid = detector.getSubdet(iendcap, isection)->getPolarFwC3D<HGCC2D>( c3dRadius );
                                
                                if( saveEventByEvent ) { 
                                    grid.getHisto()               ->Write( "polarFW_gridH" );
                                    grid.getHistoSums( binSums )  ->Write( "polarFW_gridHS" );
                                    grid.getHistoMaxima( binSums )->Write( "polarFW_gridM" );
                                    grid.getGraph()               ->Write( "polarFW_gridG" );
                                }

                                newC3Ds[iendcap] = grid.getNewC3Ds( c3dRadius, binSums );

                                for(unsigned ic3d=0; ic3d<newC3Ds[iendcap].size(); ic3d++) {
                                    newC3Ds[iendcap].at(ic3d).setNearestGen( detector.getGenAll() );
                                }

                            }

                            /* polarFWtc */
                            if( newC3DsStrategy == "polarFWtc" ){

                                HGCpolarHisto<HGCTC> grid = detector.getSubdet(iendcap, isection)->getPolarFwC3D<HGCTC>( c3dRadius );
                                                                
                                if( saveEventByEvent ) { 
                                    grid.getHisto()               ->Write( "polarFWtc_gridTcH"  );
                                    grid.getHistoSums( binSums )  ->Write( "polarFWtc_gridTcHS" );
                                    grid.getHistoMaxima( binSums )->Write( "polarFWtc_gridTcM"  );
                                    grid.getGraph()               ->Write( "polarFWtc_gridTcG"  );
                                }

                                newC3Ds[iendcap] = grid.getNewC3Ds( c3dRadius, binSums );
                                
                                for(unsigned ic3d=0; ic3d<newC3Ds[iendcap].size(); ic3d++) {
                                    newC3Ds[iendcap].at(ic3d).setNearestGen( detector.getGenAll() );
                                }

                            }


                        } // end only section 3  

                    } // endl loop over sectors

                } // end loop over sections  
                    
            } // end loop over endcaps

            tNewC3Ds->Fill();
//            tForestC3Ds->Fill();
//            tC3DsSingle->Fill();

            newC3Ds[0].clear();
            newC3Ds[1].clear();
//            forestC3Ds[0].clear();
//            forestC3Ds[1].clear();
//            C3DsSingle[0].clear();
//            C3DsSingle[1].clear();
            
        } // end c2d
            

        /**********************/
        /* Loooping over C3Ds */
        if( detector.areC3Dpresent() ){
            
            int isubdet = 3;

            for(unsigned iendcap=0; iendcap< HGCgeom::instance()->nEndcaps(); iendcap++ ){
                vector<const HGCC3D*> C3Ds;
                
                C3Ds = detector.getSubdet(iendcap, isubdet)->getAll<HGCC3D>();
                nC3D[nEvt][iendcap] =  C3Ds.size();
                
            } // endl loop over C3Ds
        
        } // end C3Ds

        /**********save*************/

        /**********save*************/


        /**********cleaning*********/
        
        /**********cleaning*********/

        if( isBit(verboselevel, VERBOSE_MAIN) )            
            cout << " MAIN >>> Event processed. " << endl; 

    } // end of evt loop


    fileOut->cd("/");
    tNewC3Ds->Write();
    //tForestC3Ds->Write();
    //tC3DsSingle->Write();

    for(unsigned iendcap=0; iendcap<HGCgeom::instance()->nEndcaps(); iendcap++) {
        for(unsigned isection=0; isection<nLongitudinalSections; isection++) {
            if(isection==3)

            for(unsigned isector=0; isector<nPhiSectors; isector++) {
                fileOut->cd();
            }
        }
    }

    if( isBit(verboselevel, VERBOSE_MAIN) )
        cout << " MAIN >>> The loop over events ended. " << endl;

    /* close the out file */
    fileOut->Close();
    
    /* Run the App (don't quit the graphycs) */
    if( persistentGraphics ) app.Run();

    return 0;

}


