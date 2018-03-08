
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


using namespace std;

const unsigned VERBOSE_OPTIONS = 0x01;
const unsigned VERBOSE_MAIN    = 0x02;
const unsigned VERBOSE_MAIN2   = 0x04;
const unsigned VERBOSE_MAIN3   = 0x08;
const unsigned VERBOSE_HOUGH   = 0x10;
const unsigned VERBOSE_TC      = 0x20;
const unsigned VERBOSE_C2D     = 0x40;
const unsigned VERBOSE_C3D     = 0x80;

const int verboselevel = VERBOSE_OPTIONS | VERBOSE_MAIN ;
const bool flagTCs = true;
const bool flagC2D = true;
const bool flagC3D = true;
const bool flagGen = true;
const bool flagGenPart = true;
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
//    unsigned nEndcaps=2; 

    // hough transofrm
    //float zOffset=323; // cm
    //float zOffset=420.387; // cm; middle of the detector
    float    zOffset[nLongitudinalSections] = { 334.440, 382.075, 708.000, 334.440 }; // cm; middle of EE
    float    slopeCorrection[nLongitudinalSections] = { 32, 32, 32, 4 }; // cm; middle of EE
    double   houghThr=0;
    unsigned houghCols=32;
    unsigned houghRows=32;
    bool     houghWeight = false;

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
        //{"houghZoffset", required_argument,  0, 'z'},
        {"houghThr",     required_argument,  0, 't'},
        {"houghCols",    required_argument,  0, 'C'},
        {"houghRows",    required_argument,  0, 'R'},
        {"houghWeight",  required_argument,  0, 'w'},
        {"persistentGraphics",    required_argument,  0, 'p'},
        {0,0,0,0}
    };

    int optIndex = 0;
    while ( (opt = getopt_long (argc, argv, "hf:i:o:d:n:s:z:t:C:R:wsp", longOptions, &optIndex) ) != -1 ) {
        
        switch (opt)
        {
        case 'h':
            cout << "Help" << endl;
            cout << " -h(--help        ) :\t shows this help" << endl; 
            cout << " -f(--fileList    ) <files> :\t list of files to be processed" << endl; 
            cout << " -i(--iFile       ) <ifile> :\t single file input" << endl; 
            cout << " -o(--oFile       ) <ofile> :\t output file" << endl; 
            cout << " -d(--firstEvent  ) <firstEvent> :\t first event to be processed (default: 0)" << endl;
            cout << " -n(--nEvt        ) <nEvt> :\t number of events to be processed (default: all)" << endl;
            cout << " -s(--nPhiSectors ) <nPhiSectors> :\t number of phi sectors for hough transform (default: 32)" << endl;
            //cout << " -z(--houghZoffset) <zOffset> :\t hough transform Zaxis offset (default: 323cm[~1st layer])" << endl;
            cout << " -t(--houghThr    ) <houghThr> :\t hough transform Threscold (default: 0)" << endl;
            cout << " -C(--houghCols   ) <houghCols> :\t hough transform number of cols (default: 32)" << endl;
            cout << " -R(--houghRows   ) <houghRows> :\t hough transform number of rows (default: 32)" << endl;
            cout << " -w(--houghWeight ) :\t this turns on the weighting for Pt of the Hhhough transform (default: false)" << endl;
            cout << " -p(--persistentGraphics   ) :\t keeps the graphis persistent crl+c to kill it (default: false)" << endl;
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
            //case 'z':
            //zOffset = atof( optarg );
            //break;
	case 't':
            houghThr = atof( optarg );
            break;
	case 'C':
            houghCols = atoi( optarg );
            break;
	case 'R':
            houghRows = atoi( optarg );
            break;
	case 'w':
            houghWeight = true;
            break;
	case 'p':
            persistentGraphics = true;
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
        //cout << "  zOffset: "        << zOffset        << endl;
        cout << "  houghThr: "       << houghThr       << endl;
        cout << "  houghCols: "      << houghCols      << endl;
        cout << "  houghRows: "      << houghRows      << endl;
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
    
    // triggercells
    
    // gen 
    vector<TGraph*> gGenProj;
    TGraph gGenProjXY[HGCgeom::instance()->nEndcaps()][nLongitudinalSections][nPhiSectors];
    TGraph gNtotBinsVEvtNumber; 

    // genpart
    TGraph gGenpartProjXY[HGCgeom::instance()->nEndcaps()]; // per endcap
    TGraph gGenpartHoughResults[HGCgeom::instance()->nEndcaps()][nPhiSectors]; // per endcap per phi sector

    // C2D

    // C3D
    double nC3D[nEvt][HGCgeom::instance()->nEndcaps()];

    // hough transform
    TGraph gNbinsVsEvtNumber[HGCgeom::instance()->nEndcaps()][nLongitudinalSections][nPhiSectors]; 
    TH1D hNbinsVsEvtNumber[HGCgeom::instance()->nEndcaps()][nLongitudinalSections][nPhiSectors]; 
    for(unsigned iendcap=0; iendcap<HGCgeom::instance()->nEndcaps(); iendcap++) {
        for(unsigned isection=0; isection<nLongitudinalSections; isection++) {
            for(unsigned isector=0; isector<nPhiSectors; isector++) {
                hNbinsVsEvtNumber[iendcap][isection][isector].SetBins(4000, 0, 3999);
            }
        }
    }


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
        unsigned nBinsOverThr = 0;

        if( isBit(verboselevel, VERBOSE_MAIN) )
            cout << " MAIN >> Analyzing event No " << ievt << endl;

        /******BUILD DIR HIERARCHY******/
        /* make a directory for this event and the phi sector in iFile */
        TDirectory* eventDir = fileOut->mkdir( Form("event_%d", ievt) );

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
       
        /* Get Entry */
        detector.getEvent( ievt );


        /************************/
        /* looping over the GEN */
        if( detector.areGenPresent() ){
        
            vector<HGCgen*> gens = detector.getGenAll();

            for(unsigned igen=0; igen<gens.size(); igen++) {

                HGCgen* gen = gens.at(igen);

                if( !( gen->Eta()>1.5 && gen->Eta()<3 ) ) continue;
                //TGraph* g = new TGraph();
                //
                //g->SetPoint( 0, 
                //             gen->getZprojection( zOffset[0] ).Phi(), 
                //             gen->getZprojection( zOffset[0] ).Rho()       
                //    );
                //g->SetMarkerStyle(27);
                //g->SetMarkerColor(8);
                //g->SetMarkerSize( gen->Pt()/10 );
                //gGenProj.push_back(g);

                /* store the gen part into the correct section and phi sector */
                for(unsigned isection=0; isection<nLongitudinalSections; isection++){
                    
//                    gen->getHT( 300, zOffset[isection], 1 ).Write( Form("gen_%d_%d_300", gen->PDGid(), gen->id() ) );
//                    gen->getHT( 320, zOffset[isection], 1 ).Write( Form("gen_%d_%d_320", gen->PDGid(), gen->id() ) );
                    
                    unsigned iendcap = gen->getEndcapId();
                    unsigned isector = gen->getPhiSectorProj( nPhiSectors, minPhis, maxPhis, zOffset[isection] );
                    gGenProjXY[iendcap][isection][isector].SetPoint( gGenProjXY[iendcap][isection][isector].GetN(),
                                                                     gen->getZprojection( zOffset[isection] ).X(), 
                                                                     gen->getZprojection( zOffset[isection] ).Y() 
                        );
                    
                }
                
            } // end gen loop
            
            for(unsigned iendcap=0; iendcap<HGCgeom::instance()->nEndcaps(); iendcap++) {
                for(unsigned isection=0; isection<nLongitudinalSections; isection++) {
                    for(unsigned isector=0; isector<nPhiSectors; isector++){
                        
                        if( gGenProjXY[iendcap][isection][isector].GetN() == 0 ) continue;

                        fileOut->cd( Form("event_%d/endcap_%d/section_%d/sector_%d/",
                                          ievt, 
                                          iendcap,
                                          isection,
                                          isector )
                            );
                        gGenProjXY[iendcap][isection][isector].Write("gen_xy_proj"); // save the graph
                        gGenProjXY[iendcap][isection][isector].Set(0); // clean the graph
                    
                    }
                }
            }
            
        }


        /************************/
        /* looping over the GENPART */
//TBD        if( detector.areGenpartPresent() ){
//TBD        
//TBD            vector<HGCgenpart*> genparts = detector.getGenpartAll();
//TBD
//TBD            cout << "GENPART >>> N " << genparts.size() << endl;
//TBD            
//TBD            for(unsigned igenpart=0; igenpart<genparts.size(); igenpart++) {
//TBD            
//TBD                HGCgenpart* genpart = genparts.at(igenpart);
//TBD
//TBD                // continue if genpart is not reaching the EE
//TBD                //if( !genpart->hasPos() || genpart->ReachedEE()!=2 ) continue; 
//TBD                if( genpart->ReachedEE()!=2 ) continue; 
//TBD                
//TBD                // get phi sector and endcap
//TBD                int isector      = genpart->getPhiSectorProj(1, nPhiSectors, minPhis, maxPhis );
//TBD                unsigned iendcap = genpart->getEndcapId();
//TBD                double zOffset_corrected = genpart->getEndcapId()==0 ? zOffset : -zOffset;
//TBD                
//TBD                // XY projection
//TBD                gGenpartProjXY[iendcap].SetPoint(gGenpartProjXY[iendcap].GetN(), 
//TBD                                                 genpart->Posx().at(0),
//TBD                                                 genpart->Posy().at(0)
//TBD                    );
//TBD                gGenpartHoughResults[iendcap][isector].SetPoint( gGenpartHoughResults[iendcap][isector].GetN(), 
//TBD                                                                 genpart->getHTcoordinate(zOffset_corrected).getX(),
//TBD                                                                 genpart->getHTcoordinate(zOffset_corrected).getY()
//TBD                    );
//TBD                
//TBD                // get to the correct folder
//TBD                for(unsigned isection=0; isection<nLongitudinalSections; isection++){
//TBD
//TBD                    fileOut->cd( Form("event_%d/endcap_%d/section_%d/sector_%d/", 
//TBD                                      ievt, 
//TBD                                      iendcap,
//TBD                                      isection,
//TBD                                      isector )
//TBD                        );
//TBD                    
//TBD                    genpart->getHT(1, zOffset_corrected ).Write( Form("genpart_%d_%d_layer_1", genpart->PDGid(), genpart->UniqueId() ) );
//TBD                    genpart->getHT(3, zOffset_corrected ).Write( Form("genpart_%d_%d_layer_3", genpart->PDGid(), genpart->UniqueId() ) );
//TBD
//TBD                }
//TBD
//TBD            } // end loop genpart
//TBD            
//TBD            fileOut->cd( Form("event_%d", ievt) );
//TBD            gGenpartProjXY[0].Write("genpart_xy_proj_endcap_0"); // save the graph
//TBD            gGenpartProjXY[1].Write("genpart_xy_proj_endcap_1"); // save the graph
//TBD
//TBD            for(unsigned iendcap=0; iendcap<HGCgeom::instance()->nEndcaps(); iendcap++){
//TBD                
//TBD                for(unsigned isector=0; isector<nPhiSectors; isector++){
//TBD                    fileOut->cd( Form("event_%d/endcap_%d/sector_%d/", ievt, iendcap, isector) );
//TBD                    
//TBD                    if( gGenpartHoughResults[iendcap][isector].GetN() > 0 ){
//TBD                        gGenpartHoughResults[iendcap][isector].Write("HTtruthResults");
//TBD                        gGenpartHoughResults[iendcap][isector].Set(0);
//TBD                    }
//TBD                }
//TBD            }
//TBD
//TBD            gGenpartProjXY[0].Set(0); // clean the graph
//TBD            gGenpartProjXY[1].Set(0); // clean the graph
//TBD
//TBD        } // end genpart


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
                    vector<HGCTC*> tcs = detector.getSubdet( iendcap, isection )->getTCall();
                    
                    /* LOOP */
                    unsigned nTC = tcs.size();
                    for( unsigned itc=0; itc<nTC; itc++ ){
                        
                        if( isBit(verboselevel, VERBOSE_TC) ) {
                            float percent = 100.*(float)itc/(float)nTC; 
                            if( itc%1000 == 0 )
                                cout << " MAIN >> " << itc << " TC processed (" << percent << "%)" << endl;
                        }
                    
                        /* get the trigger cell */
                        HGCTC* tc = tcs.at(itc);
                    
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

                    if( isBit(verboselevel, VERBOSE_HOUGH) ) 
                        cout << " >>> MAIN: Section " << isection << endl;

                    /* point the correct location */
                    fileOut->cd( Form("event_%d/endcap_%d/section_%d", ievt, iendcap, isection) );

                    /* get subdetector */
                    HGCsubdet* subdetector = detector.getSubdet(iendcap, isection);
                    
                    /* get C2Ds projection */
                    TGraph gC2Dxy;
                    subdetector->getC2DallInPhiRegion(-4, 4, gC2Dxy );
                    
                    
                    gC2Dxy.Write( Form("C2D_xy_proj_endcap_%d", iendcap) ); // save the graph
                    

                    /* loop over PHI sectors */
                    
                    /* get the C2D in the phi sector */
                    //int totPhiSec =0;
                    //for(unsigned isector=0; isector<nPhiSectors; isector++){
                    //    C2Ds[isector] = subdetector->getC2DallInPhiRegion(minPhis[isector], maxPhis[isector]);
                    //    totPhiSec+=C2Ds[isector].size();
                    //}
                    
                    for(unsigned isector=0; isector<nPhiSectors; isector++){
                        
                        if( isBit(verboselevel, VERBOSE_HOUGH) ) 
                            cout << " >>> MAIN: Phi sector " << isector << endl;

                        /* get the right folder */
                        fileOut->cd( Form("event_%d/endcap_%d/section_%d/sector_%d", ievt, iendcap, isection, isector) );
                        
                        /*** C2Ds ***/
                        
                        /* c2d projection graphs */
                        TGraph gC2DxyPhi;
                        subdetector->getC2DallInPhiRegion( minPhis[isector], maxPhis[isector], gC2DxyPhi );
                        gC2DxyPhi.Write( "C2D_xy_proj" ); // save the graph                        
                       
                        /*** HOUGH TRANSFORM ***/
                        
                        /* params */
                        double zOffset_corrected = iendcap==0 ? zOffset[isection] : -zOffset[isection];
                        double minTanTheta=-0.6, maxTanTheta=0.6;
                        double minRho=0, maxRho=200;

                        /* get the hough transofrm fro the subdetector */
                        HGCht  transform = detector.getSubdet(iendcap, isection)->getRhoZtransform_C2D( houghCols, minTanTheta, maxTanTheta,
                                                                                                        houghRows, minRho, maxRho,
                                                                                                        zOffset_corrected, slopeCorrection[isection],
                                                                                                        minPhis[isector], maxPhis[isector], 
                                                                                                        houghWeight );
                        
                        /* get the bins over thr */
                        vector<HGCbin> houghBins = transform.getBinsAboveThr( houghThr );
                        nBinsOverThr += houghBins.size();
                        
                        /* histos and graphs */
                        TH2D histo, histoThr;
                        TGraph g;

                        transform.getTransformedHisto( histo );
                        transform.getTransformedHisto( histoThr, houghThr );                        
                        transform.getXYgraph( g );
                        
                        histoThr.Write(Form("HTthr_%d",    isector));
                        histo.Write(Form("HT_%d",    isector));
                        g.Write( Form("phiSectorTransverse_%d", isector) );
                        
                        /* BINS */

                        if ( houghBins.size() ) {
                            
                            if( isBit(verboselevel, VERBOSE_HOUGH ) ) {
                                cout << " >>> HOUGH: There are " << houghBins.size() << " bins over thr." << endl;
                            }
                            
                            gNbinsVsEvtNumber[iendcap][isection][isector].SetPoint( gNbinsVsEvtNumber[iendcap][isection][isector].GetN(), 
                                                                                    ievt, 
                                                                                    houghBins.size() 
                                );
                                
                            // loop over the bins from the HT 
                            for( unsigned ibin=0; ibin<houghBins.size(); ibin++ ) {
                                
                                TGraph gEne;
                                houghBins.at(ibin).getLongitudinalEnergyProfile( detector.getSubdet(iendcap,isection), gEne );
                                gEne.Write( Form( "EnergyProfile_bin_x%d_y%d", 
                                                  houghBins.at(ibin).getCentreId().first, 
                                                  houghBins.at(ibin).getCentreId().second )
                                    );
                                
                                
                                TGraph gPt;
                                houghBins.at(ibin).getLongitudinalPtProfile( detector.getSubdet(iendcap, isection), gPt );
                                gPt.Write( Form( "PtProfile_x%d_y%d",
                                                 houghBins.at(ibin).getCentreId().first, 
                                                 houghBins.at(ibin).getCentreId().second )
                                    );
                            
                            } // end loop over bins

                        } // hough bin if

                    } // endl loop over sectors

                } // end loop over sections  
                    
            } // end loop over endcaps
            
        } // end c2d
            

        /**********************/
        /* Loooping over C3Ds */
        if( detector.areC3Dpresent() ){
            
            int isubdet = 3;

            for(unsigned iendcap=0; iendcap< HGCgeom::instance()->nEndcaps(); iendcap++ ){
                vector<HGCC3D*> C3Ds;
                
                C3Ds = detector.getSubdet(iendcap, isubdet)->getC3Dall();
                nC3D[nEvt][iendcap] =  C3Ds.size();
                
            } // endl loop over C3Ds
        
        } // end C3Ds

        /**********save*************/

        //for(unsigned iendcap=0; iendcap<2; iendcap++){
        //    for(unsigned isector=0; isector<nPhiSectors; isector++){
        //        fileOut->cd( Form("event_%d/sector_%d/endcap_%d", ievt, isector, iendcap) );
        //        
        //        if( gGenpartHoughResults[iendcap][isector].GetN() > 0 )
        //            gGenpartHoughResults[iendcap][isector].Write("HTtruthResults");
        //        else 
        //            cout << "gGenpartHoughResults no results" << endl; 
        //    }
        //}

        /**********save*************/


        /**********cleaning*********/

        /* cleaning 'sector related' stuff */
        //for(unsigned iendcap=0; iendcap<nEndcaps; iendcap++) {
        //    for(unsigned isector=0; isector<nPhiSectors; isector++){
//      //          transform[iendcap][isector].clear();
        //        gGenpartHoughResults[iendcap][isector].Set(0);
        //    }
        //}
        
        /**********cleaning*********/

        if( isBit(verboselevel, VERBOSE_MAIN) )            
            cout << " MAIN >>> Event processed. " << endl; 

    } // end of evt loop


    for(unsigned iendcap=0; iendcap<HGCgeom::instance()->nEndcaps(); iendcap++) {
        for(unsigned isection=0; isection<nLongitudinalSections; isection++) {
            for(unsigned isector=0; isector<nPhiSectors; isector++) {
                fileOut->cd();
                gNbinsVsEvtNumber[iendcap][isection][isector].Write( Form("HTbinsVsEvent_endcap%d_section%d_sector%d", iendcap, isection, isector) );
            }
        }
    }

    if( isBit(verboselevel, VERBOSE_MAIN) )
        cout << " MAIN >>> The loop over events ended. " << endl;

    
    //TCanvas cHoughPoint("cHoughPoint", "cHoughPoint", 1);
    //gNbinsVEvtNumber.SetMarkerStyle(20);
    //gNbinsVEvtNumber.Draw("AP");
    //fileOut->cd();
    //gNbinsVEvtNumber.Write("NbinsOverThrPerEvent");
    //gNtotBinsVEvtNumber.Write("NtotBinsOverThrPerEvent");
    
    /* close the out file */
    fileOut->Close();
    
    /* Run the App (don't quit the graphycs) */
    if( persistentGraphics ) app.Run();

    return 0;

}


