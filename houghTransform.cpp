
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
const bool flagC3D = false;
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

    /* get the geometry */
    HGCgeom::initialise();

    /* PARSING THE COMMAND LINE ARGUMENTS */
    int opt;

    // files and event control
    TString fFileListName;
    TString inputFileName;
    TString outputFileName("tmp.root");
    int nEvt = -1;
    int firstEvent=0;
    unsigned nPhiSectors=32; 

//    unsigned nEndcaps=2; 

    // hough transofrm
    //float zOffset=323; // cm
    //float zOffset=420.387; // cm; middle of the detector
    float    zOffset=334.440; // cm; middle of EE
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
        {"houghZoffset", required_argument,  0, 'z'},
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
            cout << " -z(--houghZoffset) <zOffset> :\t hough transform Zaxis offset (default: 323cm[~1st layer])" << endl;
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
	case 'z':
            zOffset = atof( optarg );
            break;
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
        cout << "  zOffset: "        << zOffset        << endl;
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
    TGraph gGenProjXY;
    TGraph gNbinsVEvtNumber; 
    TGraph gNtotBinsVEvtNumber; 

    // genpart
    vector<TGraph*> gGenpartProj;
    TGraph gGenpartProjXY[2]; // per endcap
    TGraph gGenpartHoughResults[2][nPhiSectors]; // per endcap per phi sector
    

    // C2D
    TGraph gC2Dxy[2]; // per endcap

    // C3D


    /* HOUGH TRANSFORM declarations */
//    HGCht transform[nEndcaps][nPhiSectors];
   
//    for(unsigned iendcap=0; iendcap<nEndcaps; iendcap++){
//        for(unsigned isector=0; isector<nPhiSectors; isector++){
//            transform[iendcap][isector].setParams( houghCols, -1, 1,
//                                                   houghRows, 0, 200 );
//        }
//    }

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

        /* make a directory for this event and the phi sector in iFile */
        TDirectory* eventDir = fileOut->mkdir( Form("event_%d", ievt) );
        
        for(unsigned isector=0; isector<nPhiSectors; isector++){   
            // new folder for phi sector
            fileOut->cd();
            fileOut->mkdir( Form("event_%d/sector_%d/endcap_0", ievt, isector) );
            fileOut->mkdir( Form("event_%d/sector_%d/endcap_1", ievt, isector) );
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

                if( !(gen->Eta() >1.5 && gen->Eta() <3) ) continue;
                TGraph* g = new TGraph();
                
                g->SetPoint( 0, 
                             gen->getZprojection( zOffset ).Phi(), 
                             gen->getZprojection( zOffset ).Rho()       
                    );
                g->SetMarkerStyle(27);
                g->SetMarkerColor(8);
                g->SetMarkerSize( gen->Pt()/10 );
                gGenProj.push_back(g);

                /* store the gen part into the correct phi sector */
                fileOut->cd( Form("event_%d/sector_%d/endcap_%d",
                                  ievt, 
                                  gen->getPhiSectorProj(nPhiSectors, minPhis, maxPhis, zOffset) ,
                                  gen->getEndcapId() )
                    );
                gen->getHT( 300, zOffset, 1 ).Write( Form("gen_%d_%d_300", gen->g4id(), gen->id() ) );
                gen->getHT( 320, zOffset, 1 ).Write( Form("gen_%d_%d_320", gen->g4id(), gen->id() ) );
                gGenProjXY.SetPoint( gGenProjXY.GetN(),
                                     gen->getZprojection(zOffset).X(), 
                                     gen->getZprojection(zOffset).Y() 
                    );

            }

            fileOut->cd( Form("event_%d", ievt) ); // 
            gGenProjXY.Write("gen_xy_proj"); // save the graph
            gGenProjXY.Set(0); // clean the graph

        }


        /************************/
        /* looping over the GENPART */
        if( detector.areGenpartPresent() ){
        
            vector<HGCgenpart*> genparts = detector.getGenpartAll();

            cout << "GENPART >>> N " << genparts.size() << endl;
            
            for(unsigned igenpart=0; igenpart<genparts.size(); igenpart++) {
            
                HGCgenpart* genpart = genparts.at(igenpart);

                // continue if genpart is not reaching the EE
                //if( !genpart->hasPos() || genpart->ReachedEE()!=2 ) continue; 
                if( genpart->ReachedEE()!=2 ) continue; 
                
                // get phi sector and endcap
                int isector      = genpart->getPhiSectorProj(1, nPhiSectors, minPhis, maxPhis );
                unsigned iendcap = genpart->getEndcapId();
                double zOffset_corrected = genpart->getEndcapId()==0 ? zOffset : -zOffset;
                
                // XY projection
                gGenpartProjXY[iendcap].SetPoint(gGenpartProjXY[iendcap].GetN(), 
                                                 genpart->Posx().at(0),
                                                 genpart->Posy().at(0)
                    );
                gGenpartHoughResults[iendcap][isector].SetPoint( gGenpartHoughResults[iendcap][isector].GetN(), 
                                                                 genpart->getHTcoordinate(zOffset_corrected).getX(),
                                                                 genpart->getHTcoordinate(zOffset_corrected).getY()
                    );
                
                // get to the correct folder
                fileOut->cd( Form("event_%d/sector_%d/endcap_%d", 
                                  ievt, 
                                  isector,
                                  iendcap )
                    );
                
                genpart->getHT(1, zOffset_corrected ).Write( Form("genpart_%d_%d_layer_1", genpart->PDGid(), genpart->UniqueId() ) );
                genpart->getHT(3, zOffset_corrected ).Write( Form("genpart_%d_%d_layer_3", genpart->PDGid(), genpart->UniqueId() ) );
                
            } // end loop genpart
            
            fileOut->cd( Form("event_%d", ievt) );
            gGenpartProjXY[0].Write("genpart_xy_proj_endcap_0"); // save the graph
            gGenpartProjXY[1].Write("genpart_xy_proj_endcap_1"); // save the graph

            for(unsigned iendcap=0; iendcap<HGCgeom::kNendcaps; iendcap++){
                for(unsigned isector=0; isector<nPhiSectors; isector++){
                    fileOut->cd( Form("event_%d/sector_%d/endcap_%d", ievt, isector, iendcap) );
                    
                    if( gGenpartHoughResults[iendcap][isector].GetN() > 0 ){
                        gGenpartHoughResults[iendcap][isector].Write("HTtruthResults");
                        gGenpartHoughResults[iendcap][isector].Set(0);
                    }
                }
            }


            gGenpartProjXY[0].Set(0); // clean the graph
            gGenpartProjXY[1].Set(0); // clean the graph

        } // end genpart


        /********************/
        /* looping over TCs */
        if( detector.areTCpresent() ){

            if( isBit(verboselevel, VERBOSE_MAIN) )
                cout << " MAIN >> TC analysis " << endl;
     
            for(int iendcap=0; iendcap<2; iendcap++) {
                /* get the vector */
                vector<HGCTC*> tcs = detector.getSubdet(iendcap,3)->getTCall();
                
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

                }// end encap loop

            }// end TCs LOOP

        }// end TCs


        /************************/
        /* looping over the C2D */

        if( detector.areC2Dpresent() ) {

            cout << " MAIN >>> C2D analysis." << endl;

            for(int iendcap=0; iendcap<2; iendcap++) {
                vector<HGCC2D*> C2Ds[nPhiSectors];

                // how many sectors
                if( isBit(verboselevel, VERBOSE_HOUGH) )
                    cout << " HOUGH >> The detector is divide into " << nPhiSectors << " phi sectors." << endl;

                // get the C2D in the phi sector
                //cout << " det all       " << detector.getSubdet(iendcap,3)->getC2Dall().size() << endl;
                //cout << "one phi sector " << detector.getSubdet(iendcap,3)->getC2DallInPhiRegion(-4, 4).size() << endl;
                int totPhiSec =0;
                for(unsigned isector=0; isector<nPhiSectors; isector++){
                    C2Ds[isector] = detector.getSubdet(iendcap,3)->getC2DallInPhiRegion(minPhis[isector], maxPhis[isector]);
                    totPhiSec+=C2Ds[isector].size();
                    //cout << " endcap " << iendcap << " sector " << isector << " " << C2Ds[isector].size() << endl;
                }
                //cout << "sum phi sector " << totPhiSec << endl;

                /* loop over ALL clusters */
                for(unsigned iclu=0; iclu<detector.getSubdet(iendcap,3)->getC2Dall().size(); iclu++){
                    
                    double xClu = detector.getSubdet(iendcap,3)->getC2Dall().at(iclu)->x();
                    double yClu = detector.getSubdet(iendcap,3)->getC2Dall().at(iclu)->y();
                    //int cluEndcap = detector.getSubdet(iendcap,3)->getC2Dall().at(iclu)->getEndcapId();
                    
                    // XY projection (layer1)
                    gC2Dxy[iendcap].SetPoint( gC2Dxy[iendcap].GetN(), xClu, yClu );
                    
                }
                
                fileOut->cd( Form("event_%d", ievt) );
                gC2Dxy[0].Write("C2D_xy_proj_endcap_0"); // save the graph
                gC2Dxy[1].Write("C2D_xy_proj_endcap_1"); // save the graph
                
                gC2Dxy[0].Set(0); // clean the graph
                gC2Dxy[1].Set(0); // clean the graph
                
                /*************************/
                /* loop over PHI sectors */
                for(unsigned isector=0; isector<nPhiSectors; isector++){
                    
                    // get the right folder
                    fileOut->cd( Form("event_%d/sector_%d/endcap_%d", ievt, isector, iendcap) );
                    
                    if( isBit(verboselevel, VERBOSE_HOUGH) ) {
                        cout << " >>> HOUGH: Phi sector " << isector << endl;
                    }
                    
//                    // looping over clusers
//                    for(unsigned iclu=0; iclu<C2Ds[isector].size(); iclu++){
//                        
//                        // get the C2D
//                        HGCC2D* c2d = C2Ds[isector].at( iclu );
//                        
//                        if(c2d->z()<0 || c2d->z()>400) continue;
//                        
//                        /* HOUGH TRANFORM */
//                        
//                        
//                        //float w = houghWeight ? c2d->Pt() : 1;
//                        //if( isBit(verboselevel, VERBOSE_HOUGH) ) {
//                        //    cout << " >>> HOUGH: Adding point "  << isector << endl;
//                        //}
//                        //transform[c2d->getEndcapId()][isector].addPointPhysicalSpace( (c2d->z()-zOffset), 
//                        //                                                             c2d->r(), 
//                        //                                                             c2d->id(), 
//                        //                                                             w
//                        //    );
//                        
//                    }// end clusters loop
                    

                    /*****************/
                    /* HOUGH FOR C2D */
                    double zOffset_corrected = iendcap==0 ? zOffset : -zOffset;
                    double minTanTheta=-0.6, maxTanTheta=0.6;
                    double minRho=0, maxRho=200;
                    HGCht  transform = detector.getSubdet(iendcap, 3)->getRhoZtransform_C2D( houghCols, minTanTheta, maxTanTheta,
                                                                                             houghRows, minRho, maxRho,
                                                                                             zOffset_corrected,
                                                                                             minPhis[isector], maxPhis[isector], 
                                                                                             houghWeight );
                    
                    // get the bins over thr
                    vector<HGChoughBin> houghBins = transform.getBinsAboveThr( houghThr );
                    nBinsOverThr += houghBins.size();
                    
                    // SOME HT histos and graphs
                    
                    transform.getTransformedHisto( Form("HT_%d", isector) )->Write();
                    transform.getTransformedHistoThr( Form("HTthr_%d", isector), houghThr )->Write();
                    transform.getXYgraph( Form("phiSectorTransverse_%d", isector) ).Write();
                    
                    // if there are bins over thr
                    if ( houghBins.size() ) {
                        
                        if( isBit(verboselevel, VERBOSE_HOUGH ) ) {
                            cout << " >>> HOUGH: There are " << houghBins.size() << " bins over thr." << endl;
                        }
                        
                        gNbinsVEvtNumber.SetPoint( gNbinsVEvtNumber.GetN(), 
                                                   ievt, 
                                                   houghBins.size() 
                            );
                        
                        // loop over the bins from the HT 
                        for( unsigned ibin=0; ibin<houghBins.size(); ibin++ ) {
                            
//                            TGraph gEne;
//                            houghBins.at(ibin).getLongitudinalEnergyProfileC2D( detector.getSubdet(iendcap,3), gEne );
//                            gEne.Write( Form( "longitudinalEnergyShape_phiSector_%d_xBin_%d_yBin_%d", 
//                                              isector, 
//                                              houghBins.at(ibin).getCentreId().first, 
//                                              houghBins.at(ibin).getCentreId().second )
//                                );
                            
                            
//                            TGraph gPt;
//                            houghBins.at(ibin).getLongitudinalPtProfileC2D( detector.getSubdet(iendcap,3), gPt );
//                            gPt.Write( Form( "longitudinalPtShape_phiSector_%d_xBin_%d_yBin_%d", 
//                                             isector,
//                                             houghBins.at(ibin).getCentreId().first, 
//                                             houghBins.at(ibin).getCentreId().second )
//                                );
                            
                            
                            if( isBit(verboselevel, VERBOSE_HOUGH ) ) {
                                
                                cout << "BIN " <<  houghBins.at(ibin).getCentreId().first << " " << houghBins.at(ibin).getCentreId().second 
                                     << " contains " << houghBins.at(ibin).getIds().size() << " hits" << endl; 
                                
                                cout << "Is in sector "     << isector                   << endl 
                                     << " centreIdx " << houghBins.at(ibin).getCentreId().first
                                     << " centreIdy " << houghBins.at(ibin).getCentreId().second
                                     << endl
                                     << " content "   << houghBins.at(ibin).getContent()
                                     << endl;
                                
//                                transform[iendcap][isector].setParams( houghCols, -0.5, 0.5,
//                                                                       houghRows, 0,    200 
//                                       );
                                
                                for( unsigned iid=0; iid<houghBins.at(ibin).getIds().size(); iid++) 
                                    cout << iid << " - " << houghBins.at(ibin).getIds().at(iid) << endl;
                                
                            }
                            
                        } // end loop over bins
                        
                    }
                    
                    

                } // end loop over sectors
    
            } // end loop over endcaps

        } // end c2d
            
        gNtotBinsVEvtNumber.SetPoint( gNtotBinsVEvtNumber.GetN(), ievt, nBinsOverThr );
        
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

    }// end of evt loop
    

    if( isBit(verboselevel, VERBOSE_MAIN) )
        cout << " MAIN >>> The loop over events ended. " << endl;

    
    TCanvas cHoughPoint("cHoughPoint", "cHoughPoint", 1);
    gNbinsVEvtNumber.SetMarkerStyle(20);
    gNbinsVEvtNumber.Draw("AP");
    fileOut->cd();
    gNbinsVEvtNumber.Write("NbinsOverThrPerEvent");
    gNtotBinsVEvtNumber.Write("NtotBinsOverThrPerEvent");
    
    /* close the out file */
    fileOut->Close();
    
    /* Run the App (don't quit the graphycs) */
    if( persistentGraphics ) app.Run();

    return 0;

}


