#include <iostream>
#include <memory>

#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <getopt.h>

#include <TApplication.h>
#include <TROOT.h>
#include <TStyle.h>
#include <TList.h>
#include <TTree.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TVirtualPad.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TProfile.h>
#include <TGraphErrors.h>
#include <TLegend.h>

#include <HGC.h>
#include <HGCC3D.h>
#include <HGCgen.h>
#include <HGCgeom.h>
#include <HGCgenClu.h>
#include <HGCpolarHisto_T.h>


using namespace std;

class hRange {

public:
    hRange(float vmin, float vmax){
        min=vmin;
        max=vmax;
    }
    
    float min, max;

};

class hName {

public:

    hName( TObject* featureHisto, std::string featureName, hRange *featureYrange ) {
        histo = featureHisto;
        name  = featureName;
        range = featureYrange;
    }

    TObject* histo;
    std::string name;
    hRange *range;

};

int main(int argc, char** argv){

    /* parameteres */
    int nentries = -50;
    double ptCutGen = 0.5;
    double ptCutGenjet = 20;
    double genCluRadius = 0.00625;

//    string ene("50");
//    string PU("noPU");

    unsigned nBinsToSum[36] = { 
        13,                                           // 0
        11, 11, 11,                                   // 1 - 3
        9, 9, 9,                                      // 4 - 6
        7, 7, 7, 7, 7, 7,                             // 7 - 12
        5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,  //13 - 27
        3, 3, 3, 3, 3, 3, 3, 3                        //28 - 35
    };
    
//    bool c2dFlag = false;
//    bool c3dFlag = false;

    TString fOutName( "jetOut.root" );


    /* inputs */
    const struct option longOptions[] = {
        {"help",         no_argument,        0, 'h'},
        //{"energy",       required_argument,  0, 'e'},
        {"nEvt",         required_argument,  0, 'n'},
        //{"pu",           required_argument,  0, 'p'},
        {"o",            required_argument,  0, 'o'},
        {"g",            required_argument,  0, 'o'},
        {"j",            required_argument,  0, 'c'},
//        {"c2dFlag",      required_argument,  0, '2'},
//        {"c3dFlag",      required_argument,  0, '3'},
        
        {0,0,0,0}
    };
    
    int opt;
    int optIndex = 0;
    while ( (opt = getopt_long (argc, argv, "hn:o:g:j:r:", longOptions, &optIndex) ) != -1 ) {
        
        switch (opt)
        {
        case 'h':
            cout << "Help" << endl;
            cout << " -h(--help        ) :\t shows this help." << endl; 
            //cout << " -e(--energy      ) <energy> :\t energy of the sample to process." << endl; 
            cout << " -n(--nEvt        ) <nEvt> :\t number of events to be processed (default: all)." << endl;
            //cout << " -p(--pu          ) <pu> :\t PU to process ('noPU' or PU200)." << endl;
            cout << " -o(--outFile     ) <o> :\t output file name." << endl;
            cout << " -g(--cutGen      ) <cutGen> :\t Pt cut gen." << endl;
            cout << " -j(--cutGenjet   ) <cutGenJet> :\t Pt cut genjet." << endl;
            cout << " -r(--genRadius   ) <genRadius> :\t clusterig radius for Gen (norm coordinate)." << endl;
            
//            cout << " -2(--c2dFlag     ) <c2dFlag> :\t process the c2ds." << endl;
//            cout << " -3(--c3dFlag     ) <c3dFlag> :\t process the c3ds." << endl;
            return 0;
            break;
	//case 'e':
        //    ene = optarg;
        //    break;
	case 'n':
            nentries = atoi( optarg );
            break;
        //case 'p':
        //    PU = optarg;
        //    break;
        case 'o':
            fOutName = optarg;
            break;
        case 'g':
            ptCutGen = atof(optarg);
            break;
        case 'j':
            ptCutGenjet = atof(optarg);
            break;
        case 'r':
            genCluRadius = atof(optarg);
            break;
//        case '2':
//            c2dFlag = true;
//            break;    
//        case '3':
//            c3dFlag = true;
//            break;    
        case '?':
            cout << "unknown param" << endl;
            return 1;
        default:
            return 0;
        }
    }

    cout << " >>> PARAMETERS <<< "     << endl 
         << "     nentries     "  << nentries     << endl
         << "     ptCutGen     "  << ptCutGen     << "GeV" << endl
         << "     ptCutGenjet  "  << ptCutGenjet  << "GeV" << endl
         << "     genCluRadius "  << genCluRadius << endl
//         << "     ene        "  << ene       << endl
//         << "     PU         "  << PU        << endl
         << "     OUT FILE   "  << fOutName  << endl
         << std::boolalpha
//         << "     C2Dloop    "  << c2dFlag   << endl
//         << "     C3Dloop    "  << c3dFlag   << endl
         << std::noboolalpha;

    TApplication app("theApp", &argc, argv);
  
    /* style */
    gStyle->SetOptStat(0);
    gStyle->SetOptTitle(0);

    /* samples */
    vector< pair< pair<string,int> ,string > > files = { 
        pair< pair<string,int>,string >( pair<string,int>("polarGrid"   , kBlue    ), "/afs/cern.ch/user/v/vpalladi/CMS/CMSSW_rel/CMSSW_10_1_7/src/L1Trigger/L1THGCal/test/PhaseIITDRFall17DR__VBF_HToInvisible_M125_14TeV_powheg_pythia8__GEN-SIM-RECO__noPU_93X_upgrade2023_realistic_v2-v1__0046ABA3-42BC-E711-8DEB-0242AC1C0501.root" )
//        pair< pair<string,int>,string >( pair<string,int>("polarGrid"   , kBlue    ), "/afs/cern.ch/user/v/vpalladi/CMS/CMSSW_rel/CMSSW_10_1_7/src/L1Trigger/L1THGCal/test/test.root" )
    };

    const unsigned Nsamples = files.size();    

    /* cmssw */
    vector<HGC*> detectors;

    for( unsigned ifile=0; ifile<Nsamples; ifile++ ){
       
        cout << " >>> Opening: " << files[ifile].second << endl;
 
        TList* fList = new TList(); 
        fList->Add( new TObjString( TString( files[ifile].second ) ) );
        
        HGC *detector = new HGC( fList, 
                                 true, true, true, true, true, 
                                 false,
                                 0 
            );
        
        detectors.push_back( detector );
        
    }

    cout << " >>> All files opened." << endl;

    /* number of events */
    nentries = nentries>0 ? nentries : detectors.at(0)->getEvents();

    /* features */
        
    /* histos */

    TH1D *hNgenClu          = new TH1D("nGenClu",      "nGenClu",      1000, 0, 1000); 
    TH1D *hNgenInGenClu     = new TH1D("nGenInGenClu", "nGenInGenClu", 100, 0, 100); 
    TH1D *hNgenCluInGridBin = new TH1D("hNgenCluInGridBin", "hNgenCluInGridBin", 100, 0, 100); 
    
    // 1d
    TH1D *hGenjetEta       = new TH1D("GenjetEta", "GenjetEta", 1000, 1, 3); 
    TH1D *hNgenjet_E0      = new TH1D("Ngenjet_E0", "Ngenjet_E0", 10, 0, 10); 
    TH1D *hNgenjet_E1      = new TH1D("Ngenjet_E1", "Ngenjet_E1", 10, 0, 10); 

    TH1D *hDistance       = new TH1D("distance", "distance", 1000, 0, 1.2); 
    TH1D *hDistanceZoom   = new TH1D("distanceZoom", "distanceZoom", 1000, 0, .12); 
    TH1D *hDistanceEtaPhi = new TH1D("distanceEtaPhi", "distanceEtaPhi", 1000, 0, 12); 
    TH1D *hDistanceEtaPhiZoom = new TH1D("distanceEtaPhiZoom", "distanceEtaPhiZoom", 1000, 0, 1); 
    
    TH1D *hDistanceGenjet = new TH1D("distanceGenjet", "distanceGenjet", 1000, 0, 1.2); 
    
    TH1D *hDistanceFromGenjet = new TH1D("distanceFromGenjet", "distanceFromGenjet", 1000, 0, 1.2); 
    TH1D *hDistanceFromGenjetHighestNorm = new TH1D("distanceFromGenjetHighestNorm", "distanceFromGenjetHighestNorm", 1000, 0, 1.2); 
    TH1D *hDistanceFromGenjetHighestEtaPhi = new TH1D("distanceFromGenjetHighestEtaPhi", "distanceFromGenjetHighestEtaPhi", 1000, 0, 12); 
    TH1D *hDistanceFromGenjetEtaPhi = new TH1D("distanceFromGenjetEtaPhi", "distanceFromGenjetEtaPhi", 1000, 0, 12);    
    
    TH1D *hMinDistanceMaxFromGenNorm = new TH1D("minDistanceMaxFromGenNorm", "minDistanceMaxFromGenNorm", 1000, 0, 12);
    
    TH1D *hNumberOfGen01 = new TH1D("numberOfGen01", "numberOfGen01", 10, 0, 10);
    TH1D *hNumberOfGen03 = new TH1D("numberOfGen03", "numberOfGen03", 10, 0, 10);
    TH1D *hNumberOfGen01ratio03 = new TH1D("numberOfGen01ratio03", "numberOfGen01ratio03", 10, 0, 10);

//    TH1D *hDistancePtCut           = new TH1D("distancePtCut", "distancePtCut", 1000, 0, 1.2); 
//    TH1D *hDistanceGenjetPtCut     = new TH1D("distanceGenjetPtCut", "distanceGenjetPtCut", 1000, 0, 1.2); 
//    TH1D *hDistanceZoomPtCut       = new TH1D("distanceZoomPtCut", "distanceZoomPtCut", 1000, 0, .12); 
//    TH1D *hDistanceFromGenjetPtCut = new TH1D("distanceFromGenjetPtCut", "distanceFromGenjetPtCut", 1000, 0, 1.2); 
    
    // 2d
    TH2D *hhDistanceVsPtratio = new TH2D("distanceVsPtratio", "distanceVsPtratio", 
                                        1000, 0, 1.2,
                                        1000, 0, 50  );
    TH2D *hhDistanceVsGenjetEta = new TH2D("distanceVsGenjetEta", "distanceVsGenjetEta", 
                                          1000, 0, 1.2,
                                          100, 1, 3  );
    TH2D *hhDistanceFromGenjetVsPt = new TH2D("distanceFromGenjetVsPt", "distanceFromGenjetVsPt", 
                                             1000, 0, 1.2,
                                             100, 0, 200
        ); 
    TH2D *hhDistanceFromGenjetHighestNormVsEta   = new TH2D("distanceFromGenjetHighestNormVsEta", "distanceFromGenjetHighestNormVsEta", 
                                                            1000, 0, 1.2,
                                                            100 , 1, 3 
        ); 
    TH2D *hhDistanceFromGenjetHighestEtaPhiVsEta = new TH2D("distanceFromGenjetHighestEtaPhiVsEta", "distanceFromGenjetHighestEtaPhiVsEta", 
                                                            1000, 0, 12,
                                                            100 , 1, 3 
        ); 
    

//    TH2D *hDistanceVsPtratioPtCut = new TH2D("distanceVsPtratioPtcut", "distancePtratioPtcut", 
//                                        1000, 0, 1.2,
//                                        1000, 0, 50  );
//    TH2D *hDistanceFromGenjetVsPtPtCut = new TH2D("distanceFromGenjetVsPtPtcut", "distanceFromGenjetVsPtPtcut", 
//                                             1000, 0, 1.2,
//                                             1000, 0, 200
//        ); 


    for(unsigned isample=0; isample<Nsamples; isample++) { 
   
    }

    
    /* c2d ghost */
    vector< hName > hAll = { hName(  (TObject*)hNgenClu                 , "hNgenClu"                   , new hRange(0.00001, 1 ) ), 
                             hName(  (TObject*)hNgenInGenClu            , "hNgenInGenClu"              , new hRange(0.00001, 1 ) ), 
                             hName(  (TObject*)hNgenCluInGridBin        , "hNgenCluInGridBin"          , new hRange(0.00001, 1 ) ), 

                             hName(  (TObject*)hGenjetEta               , "hGenjetEta"                 , new hRange(0.00001, 1 ) ), 
                             hName(  (TObject*)hNgenjet_E0              , "hNgenjet_E0"                , new hRange(0.00001, 1 ) ), 
                             hName(  (TObject*)hNgenjet_E1              , "hNgenjet_E1"                , new hRange(0.00001, 1 ) ), 
                             hName(  (TObject*)hDistance                , "hDistance"                  , new hRange(0.00001, 1 ) ), 
                             hName(  (TObject*)hDistanceZoom            , "hDistanceZoom"              , new hRange(0.00001, 1 ) ), 
                             hName(  (TObject*)hDistanceEtaPhi          , "hDistanceEtaPhi"            , new hRange(0.00001, 1 ) ), 
                             hName(  (TObject*)hDistanceEtaPhiZoom      , "hDistanceEtaPhiZoom"        , new hRange(0.00001, 1 ) ), 
                             hName(  (TObject*)hDistanceGenjet          , "hDistanceGenjet"            , new hRange(0.00001, 1 ) ), 
                             hName(  (TObject*)hDistanceFromGenjet      , "hDistanceFromGenjet"        , new hRange(0.00001, 1 ) ), 
                             hName(  (TObject*)hDistanceFromGenjetEtaPhi, "hDistanceFromGenjetEtaPhi"  , new hRange(0.00001, 1 ) ), 
                             hName(  (TObject*)hDistanceFromGenjetHighestNorm     , "hDistanceFromGenjetHighestNorm"    , new hRange(0.00001, 1 ) ), 
                             hName(  (TObject*)hDistanceFromGenjetHighestEtaPhi   , "hDistanceFromGenjetHighestEtaPhi"  , new hRange(0.00001, 1 ) ), 
                             hName(  (TObject*)hMinDistanceMaxFromGenNorm,"hMinDistanceMaxFromGenNorm" , new hRange(0.00001, 1 ) ),
                             hName(  (TObject*)hNumberOfGen01ratio03     ,"hNumberOfGen01ratio03"      , new hRange(0.00001, 1 ) ),
                             hName(  (TObject*)hNumberOfGen01            ,"hNumberOfGen01"             , new hRange(0.00001, 1 ) ),
                             hName(  (TObject*)hNumberOfGen03            ,"hNumberOfGen03"             , new hRange(0.00001, 1 ) ),
                             
                             hName(  (TObject*)hhDistanceVsPtratio       ,"hDistancePtratio"           , new hRange(0.00001, 1 ) ),
                             hName(  (TObject*)hhDistanceFromGenjetVsPt  ,"hDistanceFromGenjetVsPt"    , new hRange(0.00001, 1 ) ),
                             hName(  (TObject*)hhDistanceVsGenjetEta     , "hDistanceVsGenjetEta"      , new hRange(0.00001, 1 ) ),
                             hName(  (TObject*)hhDistanceFromGenjetHighestNormVsEta         , "hhDistanceFromGenjetHighestNormVsEta"        , new hRange(0.00001, 1 ) ),
                             hName(  (TObject*)hhDistanceFromGenjetHighestEtaPhiVsEta         , "hhDistanceFromGenjetHighestEtaPhiVsEta"        , new hRange(0.00001, 1 ) )
//hName(  (TObject*)hDistanceFromGenjetVsPtPtCut , "hDistanceFromGenjetVsPtPtCut" , new hRange(0.00001, 1 ) )
                             //     hName(  hNc3dZoom             , "Nc3dZoom"              , new hRange(0.00001, 1 ) )
                             
    };
    const unsigned Nfeatures = hAll.size();

    /* opening file out */
    TFile fOut(fOutName, "recreate");

    /* event Loop */
    cout << " >>> looping on " << nentries << " entries" << endl;
    int nentriesTenPC = nentries>10 ? nentries/10 : nentries;

    for(int ievt=0; ievt<nentries; ievt++) {

        if( ievt%nentriesTenPC == 0 )
            cout << 10*ievt/nentriesTenPC << "% of events processed." << endl;

        // get events from cmssw samples
        for( unsigned idetector=0; idetector<detectors.size(); idetector++ )
            detectors.at(idetector)->getEvent(ievt);
        
        // get the gen 
        vector<HGCgen*>     allGen     = detectors.at(0)->getGenAll();
        vector<HGCgen*>     allGenjet  = detectors.at(0)->getGenjetAll();
        //vector<HGCgenpart*> allGenpart = detectors.at(0)->getGenpartAll();
                
        for( int iendcap=0; iendcap<2; iendcap++ ) {

            /* get the clusterized gen part */
            vector<HGCgenClu> genClusters = detectors.at(0)->getSubdet(iendcap, 3)->getGenClusters( 0., genCluRadius ); // dr is a 2cm dr
            HGCpolarHisto<HGCgenClu> genCluGrid = detectors.at(0)->getSubdet(iendcap, 3)->getPolarGenClu( 0., genCluRadius );

            /* get tcs and c2ds */
            HGCpolarHisto<HGCC2D> polarGridC2D = detectors.at(0)->getSubdet(iendcap, 3)->getPolarFwC3D<HGCC2D>( 0.01 );
            //HGCpolarHisto<HGCTC>  polarGridTC  = detectors.at(0)->getSubdet(iendcap, 3)->getPolarFwC3D<HGCTC>( 0.01 );

            /* gen clusters */
            hNgenClu->Fill( genClusters.size() );

            for (auto genClu : genClusters) {
                hNgenInGenClu->Fill( genClu.getConstituents().size() );
            }

            TH2D* histoGenClu = genCluGrid.getHisto( false );
            for(int ibinx=1; ibinx<=histoGenClu->GetNbinsX(); ibinx++ ) {
                for(int ibiny=1; ibiny<=histoGenClu->GetNbinsY(); ibiny++ ) {
                    int content = histoGenClu->GetBinContent( ibinx, ibiny );
                    if( content > 0 ) hNgenCluInGridBin->Fill( content );
                }
            }

            /* polar grid */
            vector<maximaT> maxima = polarGridC2D.getMaxima( nBinsToSum );

            /* genjet loop */
            TGraph gGenjet;
            int igenjet=-1;
            int nGenjets=0;
            for( auto genjet : allGenjet ) {
                igenjet++;

                if( fabs( genjet->Eta() )<1.6 || fabs( genjet->Eta()>2.9 ) ) continue;
                if( genjet->getEndcapId() != iendcap ) continue;
                if( genjet->Pt() < ptCutGenjet )       continue;

                pair<int, double> Radius01[maxima.size()]; // nparticles, Pt
                pair<int, double> Radius03[maxima.size()]; // nparticles, Pt

                nGenjets++;
                gGenjet.SetPoint( gGenjet.GetN(), genjet->xNorm(), genjet->yNorm() );
                hGenjetEta->Fill( genjet->Eta() );

                for( auto genjetR : allGenjet ) {
                    if( fabs( genjetR->Eta() )<1.6 || fabs( genjetR->Eta()>2.9 ) ) continue;
                    if( genjetR->getEndcapId() != iendcap ) continue;
                    if( genjetR->Pt() < ptCutGenjet )       continue;
                    
                    hDistanceGenjet->Fill( genjet->distanceNorm( genjetR ) );
                }
               
                double genTotPt=0;
                float distNormHighestGenFromGenjet = 0;
                float distEtaPhiHighestGenFromGenjet = 0;
                float highestGenPt = 0;
                for( auto gen : allGen ) {
                    //cout << gen->genjetId() << " " << igenjet << endl;
                    //if( gen->getEndcapId() != iendcap ) continue; // no need identified by genjetid   
                    if( gen->Status() != 1 ) continue; // only stable particles 
                    if( gen->genjetId() != igenjet ) continue; // only particles from current genjet 
                    if( gen->Pt() < ptCutGen ) continue;
                    
                    /* loop on maxima */
                    unsigned imaximum=0;
                    double minDistance=100;
                    for( auto maximum : maxima ) {

                        double d = sqrt( pow(maximum.first-gen->xNorm(), 2) + pow(maximum.second-gen->yNorm(), 2) );
                        if(d<0.01){
                            (Radius01[imaximum].first)++;
                            Radius01[imaximum].second += gen->Pt();
                        }
                        if(d>0.01 && d<0.03){
                            (Radius03[imaximum].first)++;
                            Radius03[imaximum].second += gen->Pt();
                        }
                        if(d<minDistance)
                            minDistance=d;
                        
                        imaximum++;
 
                    }

                    hMinDistanceMaxFromGenNorm->Fill(minDistance);
                    
                    /* highest Pt gen */
                    if( highestGenPt < gen->Pt() ) {
                        highestGenPt = gen->Pt();
                        distNormHighestGenFromGenjet = genjet->distanceNorm( gen );
                        distEtaPhiHighestGenFromGenjet = genjet->distanceEtaPhi( gen );
                    }

                    hDistanceFromGenjet    ->Fill( genjet->distanceNorm( gen ) );
                    hDistanceFromGenjetEtaPhi->Fill( genjet->distanceEtaPhi( gen ) );
                    hhDistanceFromGenjetVsPt->Fill( genjet->distanceNorm( gen ), gen->Pt() );
                    //cout << genjet->Pt() << " " << gen->Pt() << " " << genTotPt << " " << genjet->distanceNorm( gen ) << endl;

                    if( genjet->distanceNorm( gen )<0.1 )
                        genTotPt += gen->Pt();
                    //if( gen->Pt() > ptCutGen && genjet->Pt() > ptCutGenjet ) {
                    //    hDistanceFromGenjetPtCut->Fill( genjet->distanceNorm( gen ) );
                    //    hDistanceFromGenjetVsPtPtCut->Fill( genjet->distanceNorm( gen ), gen->Pt() );
                    //}
                    

                    for( auto genR : allGen ) {
                        //if( genR->getEndcapId() != iendcap ) continue; // no need identified by genjetid   
                        if( genR->Status() != 1 ) continue; 
                        if( genR->genjetId() != igenjet ) continue;
                        if( genR == gen ) continue;
                        if( genR->Pt() < ptCutGen ) continue;

                        hDistance->Fill( gen->distanceNorm(genR) );
                        hDistanceZoom->Fill( gen->distanceNorm(genR) );
                        
                        hDistanceEtaPhi->Fill( gen->distanceEtaPhi(genR) );
                        hDistanceEtaPhiZoom->Fill( gen->distanceEtaPhi(genR) );

                        double Ptratio = gen->Pt()/genR->Pt();
                        hhDistanceVsPtratio  ->Fill( gen->distanceNorm(genR), Ptratio );
                        hhDistanceVsGenjetEta->Fill( gen->distanceNorm(genR), genjet->Eta() );
                    } // end genR loop
                    
                } // end gen loop
                
                for( unsigned imaxima=0; imaxima<maxima.size(); imaxima++ ) {
                    hNumberOfGen01->Fill( Radius01[imaxima].first );
                    if( Radius01[imaxima].first ) 
                        hNumberOfGen03->Fill(Radius01[imaxima].first);
                    if( Radius03[imaxima].first!=0 )
                        hNumberOfGen01ratio03->Fill( Radius01[imaxima].first/Radius03[imaxima].first );
                }

                hDistanceFromGenjetHighestNorm  ->Fill(distNormHighestGenFromGenjet);
                hDistanceFromGenjetHighestEtaPhi->Fill(distEtaPhiHighestGenFromGenjet);
                
                hhDistanceFromGenjetHighestNormVsEta  ->Fill( distNormHighestGenFromGenjet,   genjet->Eta() );
                hhDistanceFromGenjetHighestEtaPhiVsEta->Fill( distEtaPhiHighestGenFromGenjet, genjet->Eta() );

            }// end genjet loop

            if( iendcap == 0 ) 
                hNgenjet_E0->Fill(nGenjets);
            else 
                hNgenjet_E1->Fill(nGenjets);
//            gGenjet.Write( Form("gGenjet_%d_%d", ievt, iendcap) );
//            
//            /* gen loop */
//            TGraph gGen;
//            for( auto gen : allGen ) {
//
//                if( gen->id() != 1 )                continue; 
//                if( iendcap != gen->getEndcapId() ) continue; 
//
//                gGen.SetPoint( gGen.GetN(), gen->xNorm(), gen->yNorm() );
//                if( gen->Eta()>1.6 && gen->Eta()<2.8 ) {                    
//
//                    for( auto genR : allGen ) {
//
//                        if( genR->id() != 1 ) continue; // only stable particles
//                        if( iendcap != genR->getEndcapId() ) continue; 
//
//                        if( genR->Eta()>1.6 && genR->Eta()<2.8 ) {
//
//                            if( gen != genR ) {
//                                hDistance->Fill( gen->distanceNorm( genR ) );
//                                hDistanceZoom->Fill( gen->distanceNorm( genR ) );
//                                hDistanceVsPtratio->Fill( gen->distanceNorm( genR ), gen->Pt()/genR->Pt() );
//                                if( gen->Pt() > ptCutGen && genR->Pt() > ptCutGen ) {
//                                    hDistancePtCut->Fill( gen->distanceNorm( genR ) );
//                                    hDistanceZoomPtCut->Fill( gen->distanceNorm( genR ) );
//                                    hDistanceVsPtratioPtCut->Fill( gen->distanceNorm( genR ), gen->Pt()/genR->Pt() );
//                                }
//                            }
//                        }
//                        
//                    }
//                }
//
//            }// end gen loop
//            gGen.SetName( Form("gGen_%d_%d", ievt, iendcap) );
//            gGen.Write();
//
        }// end endcap loop

    } // end event loop

    cout << " >>> all events processed " << endl;
    
    /* canvases */

    for( unsigned ifeature=0; ifeature<Nfeatures; ifeature++ ) {
        hAll.at(ifeature).histo->Write( hAll.at(ifeature).name.c_str() );
    }

    /* c2d */
    
    /* plotting into canvases */
    
    fOut.Close();

    //app.Run();

    return 0;

} // macro's end
