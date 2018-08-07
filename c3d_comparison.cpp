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

    hName( TH1D** featureHisto, std::string featureName, hRange *featureYrange ) {
        histo = featureHisto;
        name = featureName;
        range = featureYrange;
    }

    TH1D** histo;
    std::string name;
    hRange *range;

};


int main(int argc, char** argv){

    /* parameteres */
    int nentries = -50;
    double ptCut = 0.5; // Pt cut for C3D (as in CMSSW L1Trigger/L1THGCal/python/hgcalTriggerPrimitiveDigiProducer_cfi.py )

    string ene("50");
    string PU("noPU");

    bool c2dFlag = false;
    bool c3dFlag = false;

    /* inputs */
    const struct option longOptions[] = {
        {"help",         no_argument,        0, 'h'},
        {"energy",       required_argument,  0, 'e'},
        {"nEvt",         required_argument,  0, 'n'},
        {"pu",           required_argument,  0, 'p'},
        {"c2dFlag",      required_argument,  0, '2'},
        {"c3dFlag",      required_argument,  0, '3'},
        
        {0,0,0,0}
    };
    
    int opt;
    int optIndex = 0;
    while ( (opt = getopt_long (argc, argv, "he:n:p:23", longOptions, &optIndex) ) != -1 ) {
        
        switch (opt)
        {
        case 'h':
            cout << "Help" << endl;
            cout << " -h(--help        ) :\t shows this help." << endl; 
            cout << " -e(--energy      ) <energy> :\t energy of the sample to process." << endl; 
            cout << " -n(--nEvt        ) <nEvt> :\t number of events to be processed (default: all)." << endl;
            cout << " -p(--pu          ) <pu> :\t PU to process ('noPU' or PU200)." << endl;
            cout << " -2(--c2dFlag     ) <c2dFlag> :\t process the c2ds." << endl;
            cout << " -3(--c3dFlag     ) <c3dFlag> :\t process the c3ds." << endl;
            return 0;
            break;
	case 'e':
            ene = optarg;
            break;
	case 'n':
            nentries = atoi( optarg );
            break;
        case 'p':
            PU = optarg;
            break;
        case '2':
            c2dFlag = true;
            break;    
        case '3':
            c3dFlag = true;
            break;    
        case '?':
            cout << "unknown param" << endl;
            return 1;
        default:
            return 0;
        }
    }

    cout << " >>> PARAMETERS <<< "     << endl 
         << "     nentries   "  << nentries  << endl
         << "     ptCut      "  << ptCut     << endl
         << "     ene        "  << ene       << endl
         << "     PU         "  << PU        << endl
         << std::boolalpha
         << "     C2Dloop    "  << c2dFlag   << endl
         << "     C3Dloop    "  << c3dFlag   << endl
         << std::noboolalpha;

    
    TApplication app("theApp", &argc, argv);
  
    /* style */
    gStyle->SetOptStat(0);
    gStyle->SetOptTitle(0);

    /* samples */
    string path = "";
//    string pathMulticone = "";
//    string pathMulticone02 = "";
//    string pathMulticoneMR = "";

    if( opendir( ("pi_"+PU+"_E"+ene).c_str() ) == NULL ){
        mkdir(("pi_"+PU+"_E"+ene).c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    }

    TString fOutName( ("pi_"+PU+"_E"+ene+"/canvases.root").c_str() );

    if( ene == "50"){
        if( PU == "noPU" ){
            path            = "root://cms-xrd-global.cern.ch//store/user/vpalladi/TPG/cone_R01_CMSSW_9_3_7_tc/SinglePiPt50Eta1p6_2p8/SinglePiPt50Eta1p6_2p8_noPUFEVT/180619_155107/0000";
//            pathMulticone   = "root://cms-xrd-global.cern.ch//store/user/vpalladi/TPG/multicone/SinglePiPt50Eta1p6_2p8/SinglePiPt50Eta1p6_2p8_noPUFEVT/180524_181021/0000";
//            pathMulticone02 = "root://cms-xrd-global.cern.ch//store/user/vpalladi/TPG/multicone_R02/SinglePiPt50Eta1p6_2p8/SinglePiPt50Eta1p6_2p8_noPUFEVT/180529_105957/0000";
            //pathMulticoneMR = "root://cms-xrd-global.cern.ch//store/user/vpalladi/TPG/multicone_multiradii/SinglePiPt50Eta1p6_2p8/SinglePiPt50Eta1p6_2p8_noPUFEVT/180605_192728/0000";
        }
        else if( PU == "PU200" ){
            path            = "root://cms-xrd-global.cern.ch//store/user/vpalladi/TPG/cone_R01_CMSSW_9_3_7_tc/SinglePiPt50Eta1p6_2p8/SinglePiPt50Eta1p6_2p8_PU200FEVT/180619_155030/0000";
//            pathMulticone   = "root://cms-xrd-global.cern.ch//store/user/vpalladi/TPG/multicone/SinglePiPt50Eta1p6_2p8/SinglePiPt50Eta1p6_2p8_PU200FEVT/180524_181002/0000";
//            pathMulticone02 = "root://cms-xrd-global.cern.ch//store/user/vpalladi/TPG/multicone_R02/SinglePiPt50Eta1p6_2p8/SinglePiPt50Eta1p6_2p8_PU200FEVT/180529_105940/0000";
        }
    }
    else if( ene == "100" ){
        if( PU == "noPU" ){
            path          = "root://cms-xrd-global.cern.ch//store/user/vpalladi/TPG/CMSSW_9_3_7/SinglePiPt100Eta1p6_2p8/SinglePiPt100Eta1p6_2p8_noPUFEVT/180514_175604/0000";
//            pathMulticone = "root://cms-xrd-global.cern.ch//store/user/vpalladi/TPG/multicone/SinglePiPt100Eta1p6_2p8/SinglePiPt100Eta1p6_2p8_noPUFEVT/180524_160101/0000";
        }
        else if( PU == "PU200" ){
            path          = "root://cms-xrd-global.cern.ch//store/user/vpalladi/TPG/CMSSW_9_3_7/SinglePiPt100Eta1p6_2p8/SinglePiPt100Eta1p6_2p8_PU200FEVT/180514_175628/0000";
//            pathMulticone = "root://cms-xrd-global.cern.ch//store/user/vpalladi/TPG/multicone/SinglePiPt100Eta1p6_2p8/SinglePiPt100Eta1p6_2p8_PU200FEVT/180524_155706/0000";
        }
    }


    vector< pair< pair<string,int> ,string > > files = { 
        //pair< pair<string,int>,string >( pair<string,int>("forest"        , kGreen+1 ), (path+"/C3Dstudies/SinglePiPt100Eta1p6_2p8_1_E"+ene+"_forest.root"     ).c_str()) ,
//        pair< pair<string,int>,string >( pair<string,int>("single"        , kGreen-1 ), (path+"/C3Dstudies/SinglePiPt100Eta1p6_2p8_1_E"+ene+"_single_cone_R01_CMSSW_9_3_7_tc.root"     ).c_str()) ,
        
//        pair< pair<string,int>,string >( pair<string,int>("norm9x9_R01"   , kBlue    ), (path+"/C3Dstudies/SinglePiPt100Eta1p6_2p8_1_E"+ene+"_cone_CMSSW_9_3_7_tc_norm_R01_SR9.root"    ).c_str()) ,
//        pair< pair<string,int>,string >( pair<string,int>("norm9x9_R02"   , kPink    ), (path+"/C3Dstudies/SinglePiPt100Eta1p6_2p8_1_E"+ene+"_cone_CMSSW_9_3_7_tc_norm_R02_SR9.root"    ).c_str()) , 
//        pair< pair<string,int>,string >( pair<string,int>("norm9x9_R1"    , kViolet  ), (path+"/C3Dstudies/SinglePiPt100Eta1p6_2p8_1_E"+ene+"_cone_CMSSW_9_3_7_tc_norm_R1_SR9.root"     ).c_str()) , 
        
//        pair< pair<string,int>,string >( pair<string,int>("norm5x5_R01"   , kBlue+1  ), (path+"/C3Dstudies/SinglePiPt100Eta1p6_2p8_1_E"+ene+"_cone_CMSSW_9_3_7_tc_norm_R01_SR5.root"    ).c_str()) ,
//        pair< pair<string,int>,string >( pair<string,int>("norm5x5_R02"   , kPink+1  ), (path+"/C3Dstudies/SinglePiPt100Eta1p6_2p8_1_E"+ene+"_cone_CMSSW_9_3_7_tc_norm_R02_SR5.root"    ).c_str()) ,
//        pair< pair<string,int>,string >( pair<string,int>("norm5x5_R1"    , kViolet+1), (path+"/C3Dstudies/SinglePiPt100Eta1p6_2p8_1_E"+ene+"_cone_CMSSW_9_3_7_tc_norm_R1_SR5.root"     ).c_str()) ,

        pair< pair<string,int>,string >( pair<string,int>("polarFW_R01"   , kBlue    ), (path+"/C3Dstudies/SinglePiPt100Eta1p6_2p8_1_E"+ene+"_"+PU+"FEVT_polarFW_cone_R01_CMSSW_9_3_7_tc_R01.root"    ).c_str()) ,
        pair< pair<string,int>,string >( pair<string,int>("polarFW_R02"   , kBlue+1  ), (path+"/C3Dstudies/SinglePiPt100Eta1p6_2p8_1_E"+ene+"_"+PU+"FEVT_polarFW_cone_R01_CMSSW_9_3_7_tc_R02.root"    ).c_str()) ,
        pair< pair<string,int>,string >( pair<string,int>("polarFW_R03"   , kBlue+2  ), (path+"/C3Dstudies/SinglePiPt100Eta1p6_2p8_1_E"+ene+"_"+PU+"FEVT_polarFW_cone_R01_CMSSW_9_3_7_tc_R03.root"    ).c_str()) ,

        pair< pair<string,int>,string >( pair<string,int>("polarFWtc_R01"   , kViolet    ), (path+"/C3Dstudies/SinglePiPt100Eta1p6_2p8_1_E"+ene+"_"+PU+"FEVT_polarFWtc_cone_R01_CMSSW_9_3_7_tc_R01.root"    ).c_str()) ,
        pair< pair<string,int>,string >( pair<string,int>("polarFWtc_R02"   , kViolet+1  ), (path+"/C3Dstudies/SinglePiPt100Eta1p6_2p8_1_E"+ene+"_"+PU+"FEVT_polarFWtc_cone_R01_CMSSW_9_3_7_tc_R02.root"    ).c_str()) ,
        pair< pair<string,int>,string >( pair<string,int>("polarFWtc_R03"   , kViolet+2  ), (path+"/C3Dstudies/SinglePiPt100Eta1p6_2p8_1_E"+ene+"_"+PU+"FEVT_polarFWtc_cone_R01_CMSSW_9_3_7_tc_R03.root"    ).c_str()) ,
      
//        pair< pair<string,int>,string >( pair<string,int>("norm3x3_R01"   , kBlue    ), (path+"/C3Dstudies/SinglePiPt100Eta1p6_2p8_1_E"+ene+"_norm_R01_SR3.root"    ).c_str()) ,
//        pair< pair<string,int>,string >( pair<string,int>("norm3x3_R02"   , kPink    ), (path+"/C3Dstudies/SinglePiPt100Eta1p6_2p8_1_E"+ene+"_norm_R02_SR3.root"    ).c_str()) ,
//        pair< pair<string,int>,string >( pair<string,int>("norm3x3_R1"    , kViolet  ), (path+"/C3Dstudies/SinglePiPt100Eta1p6_2p8_1_E"+ene+"_norm_R1_SR3.root"     ).c_str()) ,
        
//        pair< pair<string,int>,string >( pair<string,int>("gen_R01"       , kBlue+2  ), (path+"/C3Dstudies/SinglePiPt100Eta1p6_2p8_1_E"+ene+"_cone_CMSSW_9_3_7_tc_gen_R01.root"    ).c_str()) ,
//        pair< pair<string,int>,string >( pair<string,int>("gen_R02"       , kPink+2  ), (path+"/C3Dstudies/SinglePiPt100Eta1p6_2p8_1_E"+ene+"_cone_CMSSW_9_3_7_tc_gen_R02.root"    ).c_str()) ,
//        pair< pair<string,int>,string >( pair<string,int>("gen_R1"        , kViolet+2), (path+"/C3Dstudies/SinglePiPt100Eta1p6_2p8_1_E"+ene+"_cone_CMSSW_9_3_7_tc_gen_R1.root"     ).c_str()) ,
   
        pair< pair<string,int>,string >( pair<string,int>("cone"          , kGreen   ), (path           +"/PhaseIITDRFall17DR__SinglePiPt100Eta1p6_2p8__GEN-SIM-RECO__PU200FEVT_93X_upgrade2023_realistic_v2-v1__0694AE79-EEB0-E711-A16F-A4BF0112DFA0_1.root").c_str() ),
//        pair< pair<string,int>,string >( pair<string,int>("multicone"     , kBlack   ), (pathMulticone  +"/PhaseIITDRFall17DR__SinglePiPt100Eta1p6_2p8__GEN-SIM-RECO__PU200FEVT_93X_upgrade2023_realistic_v2-v1__0694AE79-EEB0-E711-A16F-A4BF0112DFA0_1.root").c_str() ),
//        pair< pair<string,int>,string >( pair<string,int>("multicone02"   , kBlue-9  ), (pathMulticone02+"/PhaseIITDRFall17DR__SinglePiPt100Eta1p6_2p8__GEN-SIM-RECO__PU200FEVT_93X_upgrade2023_realistic_v2-v1__0694AE79-EEB0-E711-A16F-A4BF0112DFA0_1.root").c_str() ),
//        pair< pair<string,int>,string >( pair<string,int>("multiconeMR"   , kCyan-6  ), (pathMulticoneMR+"/PhaseIITDRFall17DR__SinglePiPt100Eta1p6_2p8__GEN-SIM-RECO__PU200FEVT_93X_upgrade2023_realistic_v2-v1__0694AE79-EEB0-E711-A16F-A4BF0112DFA0_1.root").c_str() )
    };

    const unsigned Nsamples = files.size();

    // define the cmsswsamples
    vector<unsigned> cmsswSamplesId;

    for( unsigned i=0; i<files.size(); i++ ){
        if( files.at(i).first.first == "cone"        || files.at(i).first.first == "multicone"   || 
            files.at(i).first.first == "multicone02" || files.at(i).first.first == "multiconeMR"
            )
            cmsswSamplesId.push_back(i);
    }
    

    /* cmssw */
    vector<HGC*> detectors;

    for( unsigned icmsswsample=0; icmsswsample<cmsswSamplesId.size(); icmsswsample++ ){
       
        cout << " >>> Opening: " << files[cmsswSamplesId.at(icmsswsample)].second << endl;
 
        TList* fList = new TList(); 
        fList->Add( new TObjString( TString(files[cmsswSamplesId.at(icmsswsample)].second) ) );
        
        HGC *detector = new HGC( fList, 
                                 true, true, true, true, true, 
                                 false,
                                 0 
            );
        
        detectors.push_back( detector );
        
    }

    /* prod */
    TFile *fsamples[Nsamples];
    TTree *tNewC3Ds[Nsamples];
    std::vector<HGCC3D> *c3ds[Nsamples][2]; // [sample][endcap]

    /* trees */  
    for( unsigned isample=0; isample<Nsamples; isample++ ) {
        
        if( find(cmsswSamplesId.begin(), cmsswSamplesId.end(), isample) != cmsswSamplesId.end() )
            continue;

        cout << " >>> Opening: " << files[isample].second.c_str() << endl;

        fsamples[isample] = TFile::Open( files[isample].second.c_str(), "read" );

        cout << " <<< Done." << endl;

        tNewC3Ds[isample] = (TTree*)fsamples[isample]->Get("newC3Ds");
        
        c3ds[isample][0]=0;
        c3ds[isample][1]=0;
        tNewC3Ds[isample]->SetBranchAddress( "endcap0", &(c3ds[isample][0]) );
        tNewC3Ds[isample]->SetBranchAddress( "endcap1", &(c3ds[isample][1]) );
        
    }

    cout << " >>> All files opened." << endl;

    /* number of events */
    nentries = nentries>0 ? nentries : tNewC3Ds[0]->GetEntries();

    /* features */
        
    /* histos */
    TH1D *hNc3d[Nsamples];
    TH1D *hNc3dZoom[Nsamples];
    TH1D *hNc2d[Nsamples];
    TH1D *hc3dPt[Nsamples];
    TH1D *hc3dFirstLayer[Nsamples];
    TH1D *hc3dMaxLayer[Nsamples];
    TH1D *hc3dGenDist[Nsamples];
    TH1D *hc3dGenDistX[Nsamples];
    TH1D *hc3dGenDistY[Nsamples];
    TH1D *hc3dGenDistZoom[Nsamples];
    TH1D *hc3dGenDistXzoom[Nsamples];
    TH1D *hc3dGenDistYzoom[Nsamples];
//density    TH1D *hc3dEnergyDensity[Nsamples];
//density    TGraph *gc3dEnergyDensityRatio[Nsamples];
//density    TH2D *hhc3dEnergyDensityRatio[Nsamples];
//density    TH2D *hhc3dEnergyDensityTCvsEnergy[Nsamples];
    //TH2D *hhc3dEnergyVsNtc[Nsamples];
    //TH1D *hEresolution[Nsamples];     
    TH1D *hNc3dWthinGenProj[Nsamples];
    TH2D *hhc3dc2dNumberPerLayer[Nsamples];
    TH1D *hPtHighestC3D[Nsamples];
    TH1D *hPtClosestC3D[Nsamples];
    TH1D *hPtAllC3D[Nsamples];

    TGraphErrors *gPtHighestC3D[Nsamples];
    TGraphErrors *gPtClosestC3D[Nsamples];
    TGraphErrors *gPtAllC3D[Nsamples];
    TH1D *hDistHighestC3Dgen[Nsamples];
    TH1D *hDistHighestC3DgenZoom[Nsamples];

    TH2D *hhDistHighestC3DgenVsEvt[Nsamples];
    TH2D *hhDistLowPopulatedC3DgenVsLayer[Nsamples];
    TH2D *hhDistLowPopulatedC3DgenVsLayerZoom[Nsamples];

    // c2d
    TH1D *hc2dGenDist;
    TH1D *hc2dGenDistLayer[HGCgeom::instance()->nLayers()];
    TH2D *hhc2dGenDistLayer;
    TH2D *hhc2dGenDistLayerW;
    TH2D *hhc2dMinDistLayer;
    TH2D *hhc2dMinDistLayerW;
    TH2D *hhc2dGenDistPt;
    TH1D *hc2dGenDistX;
    TH1D *hc2dGenDistY;
    TH2D *hhc2dGenDistLayerZoom;
    TH2D *hhc2dGenDistLayerWZoom;
    TH2D *hhc2dMinDistLayerZoom;
    TH2D *hhc2dMinDistLayerWZoom;
    TH2D *hhc2dGenDistPtZoom;
    TH1D *hc2dGenDistZoom;
    TH1D *hc2dGenDistXzoom;
    TH1D *hc2dGenDistYzoom;
    TH2D *hhc2dNumberPerLayer;
    
    //c2d W
    TH1D *hc2dGenWDist;
    TH1D *hc2dGenWDistLayer[HGCgeom::instance()->nLayers()];
    TH2D *hhc2dGenWDistLayer;
    TH1D *hc2dGenWDistX;
    TH1D *hc2dGenWDistY;
    TH2D *hhc2dGenWDistLayerZoom;
    TH1D *hc2dGenWDistZoom;
    TH1D *hc2dGenWDistXzoom;
    TH1D *hc2dGenWDistYzoom;

    // ghost
    TH1D *hc2dGenGhostDist;
    TH1D *hc2dGenGhostDistLayer[HGCgeom::instance()->nLayers()];
    TH2D *hhc2dGenGhostDistLayer;
    TH1D *hc2dGenGhostDistX;
    TH1D *hc2dGenGhostDistY;
    TH2D *hhc2dGenGhostDistLayerZoom;
    TH1D *hc2dGenGhostDistZoom;
    TH1D *hc2dGenGhostDistXzoom;
    TH1D *hc2dGenGhostDistYzoom;

    for(unsigned isample=0; isample<Nsamples; isample++) { 
        
        hNc3d[isample]            = new TH1D( ("hNc3d_"             + files[isample].first.first ).c_str(), ("hNc3d_"             + files[isample].first.first ).c_str(), 
                                              250 , 0, 2500 );
        hNc3dZoom[isample]        = new TH1D( ("hNc3dZoom_"         + files[isample].first.first ).c_str(), ("hNc3dZoom_"         + files[isample].first.first ).c_str(), 
                                              30  , 0, 30   );
        hNc2d[isample]            = new TH1D( ("hNc2d_"             + files[isample].first.first ).c_str(), ("hNc2d_"             + files[isample].first.first ).c_str(), 
                                              30  , 0, 30   );
        hc3dPt[isample]           = new TH1D( ("hc3dPt_"            + files[isample].first.first ).c_str(), ("hc3dPt_"            + files[isample].first.first ).c_str(), 
                                              75 , 0, 150  );
        hc3dFirstLayer[isample]   = new TH1D( ("hc3dFirstLayer_"    + files[isample].first.first ).c_str(), ("hc3dFirstLayer_"    + files[isample].first.first ).c_str(), 
                                              52  , 0, 52   );
        hc3dMaxLayer[isample]     = new TH1D( ("hc3dMaxLayer_"      + files[isample].first.first ).c_str(), ("hc3dMaxLayer_"      + files[isample].first.first ).c_str(), 
                                              52  , 0, 52   );

        hc3dGenDist[isample]      = new TH1D( ("hc3dGenDist_"       + files[isample].first.first ).c_str(), ("hc3dGenDist_"       + files[isample].first.first ).c_str(), 
                                              100 ,   0,  1  );
        hc3dGenDistX[isample]     = new TH1D( ("hc3dGenDistX_"      + files[isample].first.first ).c_str(), ("hc3dGenDistX_"      + files[isample].first.first ).c_str(), 
                                              200 ,  -1,  1  );
        hc3dGenDistY[isample]     = new TH1D( ("hc3dGenDistY_"      + files[isample].first.first ).c_str(), ("hc3dGenDistY_"      + files[isample].first.first ).c_str(), 
                                              200 ,  -1,  1  );
        hc3dGenDistZoom[isample]  = new TH1D( ("hc3dGenDistZoom_"   + files[isample].first.first ).c_str(), ("hc3dGenDistZoom_"   + files[isample].first.first ).c_str(), 
                                              100 ,   0, .05  );
        hc3dGenDistXzoom[isample] = new TH1D( ("hc3dGenDistXzoom_"  + files[isample].first.first ).c_str(), ("hc3dGenDistXzoom_"  + files[isample].first.first ).c_str(), 
                                              200 , -.05, .05  );
        hc3dGenDistYzoom[isample] = new TH1D( ("hc3dGenDistYzoom_"  + files[isample].first.first ).c_str(), ("hc3dGenDistYzoom_"  + files[isample].first.first ).c_str(), 
                                              200 , -.05, .05  );
//density        hc3dEnergyDensity[isample] = new TH1D( ("hc3dEnergyDensity_"  + files[isample].first.first ).c_str(), ("hc3dEnergyDensity_"  + files[isample].first.first ).c_str(), 
//density                                               400 , .0, 100  );
//density        hhc3dEnergyDensityRatio[isample] = new TH2D( ("hhc3dEnergyDensityRatio_"  + files[isample].first.first ).c_str(), ("hhc3dEnergyDensityRatio_"  + files[isample].first.first ).c_str(), 
//density                                                     1000 , .0, 3,
//density                                                     1000 , .0, 0.1 );
//density        hhc3dEnergyDensityTCvsEnergy[isample] = new TH2D( ("hhc3dEnergyDensityTCvsEnergy_"  + files[isample].first.first ).c_str(), ("hhc3dEnergyDensityTCvsEnergy_"  + files[isample].first.first ).c_str(), 
//density                                                          1000 , .0, 100,
//density                                                          100 , .0, 100 );
//density        
//density        //hhc3dEnergyDensityRatio[isample] = new TH2D( ("hhc3dEnergyDensityRatio_"  + files[isample].first.first ).c_str(), ("hhc3dEnergyDensityRatio_"  + files[isample].first.first ).c_str(), 
//density        //                                             1000 , .0, 3,
//density        //                                             1000 , .0, 0.1  );
//density        gc3dEnergyDensityRatio[isample] = new TGraph();
//density        gc3dEnergyDensityRatio[isample]->SetName( ("gc3dEnergyDensityRatio_"  + files[isample].first.first ).c_str() );

        //hEresolution[isample]     = new TH1D( ("hEresolution_"      + files[isample].first.first ).c_str(), ("hEresolutin_"       + files[isample].first.first ).c_str(), 500 ,   0.00001, 500  );
        hNc3dWthinGenProj[isample]= new TH1D( ("hNc3dWthinGenProj_" + files[isample].first.first ).c_str(), ("hNc3dWthinGenProj_" + files[isample].first.first ).c_str(), 
                                              20  , 0, 20  );
        hhc3dc2dNumberPerLayer[isample] = new TH2D( ("hhc2dNumberPerLayer_" + files[isample].first.first ).c_str() , ("hhc2dNumberPerLayer_" + files[isample].first.first ).c_str() ,
                                                    54  ,   0, 54,
                                                    30 ,   0,  30 );
        hPtHighestC3D[isample]    = new TH1D( ("hPtHighestC3D_"     + files[isample].first.first ).c_str(), ("hPtHighestC3D_"     + files[isample].first.first ).c_str(), 
                                              250 , 0.00001, 250   );
        hPtClosestC3D[isample]    = new TH1D( ("hPtClosestC3D_"     + files[isample].first.first ).c_str(), ("hPtClosestC3D_"     + files[isample].first.first ).c_str(), 
                                              250 , 0.00001, 250   );
        hPtAllC3D[isample]        = new TH1D( ("hPtAllC3D_"         + files[isample].first.first ).c_str(), ("hPtAllC3D_"         + files[isample].first.first ).c_str(), 
                                              250 , 0.00001, 250   );
        gPtHighestC3D[isample]    = new TGraphErrors();
        gPtClosestC3D[isample]    = new TGraphErrors();
        gPtAllC3D[isample]        = new TGraphErrors();
        
        hDistHighestC3Dgen[isample]    = new TH1D( ("hDistHighestC3Dgen_"    + files[isample].first.first ).c_str(), ("hDistHighestC3Dgen_"    + files[isample].first.first ).c_str(), 
                                                   100, 0  , 1 );
        hDistHighestC3DgenZoom[isample]= new TH1D( ("hDistHighestC3DgenZoom_"+ files[isample].first.first ).c_str(), ("hDistHighestC3DgenZoom_"+ files[isample].first.first ).c_str(), 
                                                   200, 0 , .05 );
        hhDistHighestC3DgenVsEvt[isample]= new TH2D( ("hhDistHighestC3DgenVsEvt_"+ files[isample].first.first ).c_str(), ("hDistHighestC3DgenVsEvt_"+ files[isample].first.first ).c_str(), 
                                                     nentries, 0, nentries-1, 
                                                     200, 0 , .05 
            );
        hhDistLowPopulatedC3DgenVsLayer[isample]= new TH2D( ("hhDistLowPopulatedC3DgenVsLayer_"+ files[isample].first.first ).c_str(), 
                                                            ("hhDistLowPopulatedC3DgenVsLayer_"+ files[isample].first.first ).c_str(), 
                                                            52  ,   0, 52,
                                                            500 ,   0,  1
            );
        hhDistLowPopulatedC3DgenVsLayerZoom[isample]= new TH2D( ("hhDistLowPopulatedC3DgenVsLayerZoom_"+ files[isample].first.first ).c_str(), 
                                                                ("hhDistLowPopulatedC3DgenVsLayerZoom_"+ files[isample].first.first ).c_str(), 
                                                                52  ,   0, 52,
                                                                500 ,   0,  .1
            );
        
    }

    /* c2d */
    hc2dGenDist      = new TH1D( "hc2dGenDist", "hc2dGenDist", 100 ,  0, 1  );
    for(unsigned ilayer=0; ilayer<HGCgeom::instance()->nLayers(); ilayer++) { 
        hc2dGenDistLayer[ilayer]= new TH1D( Form("hc2dGenDist_layer_%d", ilayer ) ,  
                                            Form("hc2dGenDist_layer_%d", ilayer ) ,  
                                            500 ,   0,  1  );
    }
    hhc2dGenDistLayer = new TH2D( "hhc2dGenDistLayer" , "hhc2dGenDistLayer" ,
                                  52  ,   0, 52,
                                  500 ,   0,  1 );
    hhc2dGenDistLayerW = new TH2D( "hhc2dGenDistLayerW" , "hhc2dGenDistLayerW" ,
                                   52  ,   0, 52,
                                   500 ,   0,  1 );
    hhc2dMinDistLayer = new TH2D( "hhc2dMinDistLayer" , "hhc2dMinDistLayer" ,
                                  52  ,   0, 52,
                                  500 ,   0,  1 );
    hhc2dMinDistLayerW = new TH2D( "hhc2dMinDistLayerW" , "hhc2dMinDistLayerW" ,
                                   52  ,   0, 52,
                                   500 ,   0,  1 );
    hhc2dGenDistPt = new TH2D( "hhc2dGenDistPt" , "hhc2dGenDistPt" ,
                               250 ,   0.00001, 250,
                               500 ,   0,  1 );
    hc2dGenDistX     = new TH1D( "hc2dGenDistX"    , "hc2dGenDistX"    , 200 , -1,    1  );
    hc2dGenDistY     = new TH1D( "hc2dGenDistY"    , "hc2dGenDistY"    , 200 , -1,    1  );
    hhc2dGenDistLayerZoom = new TH2D( "hhc2dGenDistLayerZoom" , "hhc2dGenDistLayerZoom" ,
                                      52  ,   0, 52,
                                      200 ,  0,  .1 );
    hhc2dGenDistLayerWZoom = new TH2D( "hhc2dGenDistLayerWZoom" , "hhc2dGenDistLayerWZoom" ,
                                       52  ,   0, 52,
                                       200 ,  0,  .1 );
    hhc2dGenDistPtZoom = new TH2D( "hhc2dGenDistPtZoom" , "hhc2dGenDistPtZoom" ,
                                   250 ,   0.00001, 250,
                                   200 ,  0, .1 );
    hhc2dMinDistLayerZoom = new TH2D( "hhc2dMinDistLayerZoom" , "hhc2dMinDistLayerZoom" ,
                                      52  ,   0, 52,
                                      500 ,   0,  .1 );
    hhc2dMinDistLayerWZoom = new TH2D( "hhc2dMinDistLayerWZoom" , "hhc2dMinDistLayerWZoom" ,
                                       52  ,   0, 52,
                                       500 ,   0,  .1 );
    hc2dGenDistZoom  = new TH1D( "hc2dGenDistZoom" , "hc2dGenDistZoom" , 
                                 200 ,  0,     .1   );
    hc2dGenDistXzoom = new TH1D( "hc2dGenDistXzoom", "hc2dGenDistXzoom", 
                                 200 , -.05,   .05  );
    hc2dGenDistYzoom = new TH1D( "hc2dGenDistYzoom", "hc2dGenDistYzoom", 
                                 200 , -.05,   .05  );
    hhc2dNumberPerLayer = new TH2D( "hhc2dNumberPerLayer" , "hhc2dNumberPerLayer" ,
                                    54  ,   0, 54,
                                    30 ,   0,  30 );



    /* c2d W */
    hc2dGenWDist      = new TH1D( "hc2dGenWDist", "hc2dGenWDist", 100 ,  0, 1  );
    for(unsigned ilayer=0; ilayer<HGCgeom::instance()->nLayers(); ilayer++) { 
        hc2dGenWDistLayer[ilayer]= new TH1D( Form("hc2dGenWDist_layer_%d", ilayer ) , Form("hc2dGenWDist_layer_%d", ilayer ),  
                                             500 ,   0,  1  );
    }
    hhc2dGenWDistLayer = new TH2D( "hhc2dGenWDist" , "hhc2dGenWDist" ,
                                   52  ,   0, 52,
                                   500 ,   0,  1 );
    hc2dGenWDistX     = new TH1D( "hc2dGenWDistX"    , "hc2dGenWDistX"    , 
                                  200 , -1,    1  );
    hc2dGenWDistY     = new TH1D( "hc2dGenWDistY"    , "hc2dGenWDistY"    , 
                                  200 , -1,    1  );
    hhc2dGenWDistLayerZoom = new TH2D( "hhc2dGenWDistZoom" , "hhc2dGenWDistZoom" ,
                                       52  ,   0, 52,
                                       200 ,  0,  .1 );
    hc2dGenWDistZoom  = new TH1D( "hc2dGenWDistZoom" , "hc2dGenWDistZoom" , 
                                  200 ,  0,     .1   );
    hc2dGenWDistXzoom = new TH1D( "hc2dGenWDistXzoom", "hc2dGenWDistXzoom", 
                                  200 , -.05,   .05  );
    hc2dGenWDistYzoom = new TH1D( "hc2dGenWDistYzoom", "hc2dGenWDistYzoom", 
                                  200 , -.05,   .05  );

    /* c2d ghost */
    hc2dGenGhostDist      = new TH1D( "hc2dGenGhostDist", "hc2dGenGhostDist", 100 ,  0, 1  );
    for(unsigned ilayer=0; ilayer<HGCgeom::instance()->nLayers(); ilayer++) { 
        hc2dGenGhostDistLayer[ilayer]= new TH1D( Form("hc2dGenGhostDist_layer_%d", ilayer ) ,  
                                                 Form("hc2dGenGhostDist_layer_%d", ilayer ) ,  
                                                 500 ,   0,  1  );
    }
    hhc2dGenGhostDistLayer = new TH2D( "hhc2dGenGhostDist" , "hhc2dGenGhostDist" ,
                                       52  ,   0, 52,
                                       500 ,   0,  1 );
    hc2dGenGhostDistX     = new TH1D( "hc2dGenGhostDistX"    , "hc2dGenGhostDistX"    , 
                                      200 , -1,    1  );
    hc2dGenGhostDistY     = new TH1D( "hc2dGenGhostDistY"    , "hc2dGenGhostDistY"    , 
                                      200 , -1,    1  );
    hhc2dGenGhostDistLayerZoom = new TH2D( "hhc2dGenGhostDistZoom" , "hhc2dGenGhostDistZoom" ,
                                           52  ,   0, 52,
                                           200 ,  0,  .1  );
    hc2dGenGhostDistZoom  = new TH1D( "hc2dGenGhostDistZoom" , "hc2dGenGhostDistZoom" , 
                                      200 ,  0,     .1   );
    hc2dGenGhostDistXzoom = new TH1D( "hc2dGenGhostDistXzoom", "hc2dGenGhostDistXzoom", 
                                      200 , -.05,   .05  );
    hc2dGenGhostDistYzoom = new TH1D( "hc2dGenGhostDistYzoom", "hc2dGenGhostDistYzoom", 
                                      200 , -.05,   .05  );
    
    vector< hName > hAll = { hName(  hNc3d                 , "Nc3d"                  , new hRange(0.00001, 1 ) ) ,
                             hName(  hNc3dZoom             , "Nc3dZoom"              , new hRange(0.00001, 1   ) ) ,
                             hName(  hNc2d                 , "Nc2d"                  , new hRange(0.00001, 0.8) ) ,
                             hName(  hc3dPt                , "Pt"                    , new hRange(0.00001, 0.3 ) ) ,
                             hName(  hc3dFirstLayer        , "FirstLayer"            , new hRange(0.00001, 0.1) ) ,
                             hName(  hc3dMaxLayer          , "MaxLayer"              , new hRange(0.00001, 0.1) ) ,
                             hName(  hc3dGenDist           , "c3dGenDist"            , new hRange(0.00001, 0.25) ) ,
                             hName(  hc3dGenDistX          , "c3dGenDistX"           , new hRange(0.00001, 0.25) ) ,
                             hName(  hc3dGenDistY          , "c3dGenDistY"           , new hRange(0.00001, 0.25) ) ,
                             hName(  hc3dGenDistZoom       , "c3dGenDistZoom"        , new hRange(0.00001, 0.10) ) ,
                             hName(  hc3dGenDistXzoom      , "c3dGenDistXzoom"       , new hRange(0.00001, 0.10) ) ,
                             hName(  hc3dGenDistYzoom      , "c3dGenDistYzoom"       , new hRange(0.00001, 0.10) ) ,
//                             hName(  hEresolution          , "EResolution"           , new hRange(0.00001, 0.4 ) ) ,
                             hName(  hNc3dWthinGenProj     , "Nc3dWthinGenProj"      , new hRange(0.00001, 1   ) ) ,
                             hName(  hPtHighestC3D         , "PtHighestC3D"          , new hRange(0.00001, 0.2 ) ) ,
                             hName(  hPtClosestC3D         , "PtClosestC3D"          , new hRange(0.00001, 0.2 ) ) ,
                             hName(  hPtAllC3D             , "PtAllC3D"              , new hRange(0.00001, 0.2 ) ) ,
                             hName(  hDistHighestC3Dgen    , "DistHighestC3Dgen"     , new hRange(0.00001, 0.25) ) ,
                             hName(  hDistHighestC3DgenZoom, "DistHighestC3DgenZoom" , new hRange(0.00001, 0.1 ) ) 
//density                             hName(  hc3dEnergyDensity     , "c3dEnergyDensity"      , new hRange(0.00001, 0.1 ) )
//                             hName(  hc2dGenDist           , "c2dGenDist"            , new hRange(0, 0.01) ) ,
//                             hName(  hc2dGenDistX          , "c2dGenDistX"           , new hRange(0, 0.01) ) ,
//                             hName(  hc2dGenDistY          , "c2dGenDistY"           , new hRange(0, 0.01) ) ,
//                             hName(  hc2dGenDistZoom       , "c2dGenDistZoom"        , new hRange(0, 0.01) ) ,
//                             hName(  hc2dGenDistXzoom      , "c2dGenDistXzoom"       , new hRange(0, 0.04) ) ,
//                             hName(  hc2dGenDistYzoom      , "c2dGenDistYzoom"       , new hRange(0, 0.04) )  
    };
    const unsigned Nfeatures = hAll.size();

    TH2D *h2FirstVsMaxLayer[Nsamples];

    for(unsigned isample=0; isample<Nsamples; isample++) {
        h2FirstVsMaxLayer[isample] = new TH2D( ("h2FirstVsMaxLayer" + files[isample].first.first ).c_str(),  
                                               ("h2FirstVsMaxLayer" + files[isample].first.first ).c_str(),
                                               52, 0, 52,
                                               52, 0, 52
            );
    }

    
    /* event Loop */
    cout << " >>> looping on " << nentries << " entries" << endl;
    
    for(int ievt=0; ievt<nentries; ievt++) {
        
        // get the gen part 
        vector<HGCgen*> allGen = detectors.at(0)->getGenAll();
        
        // get events from cmssw samples
        for( unsigned idetector=0; idetector<detectors.size(); idetector++ ){
            detectors.at(idetector)->getEvent(ievt);
        }
       
        // get events from prod 
        for(unsigned isample=0; isample<Nsamples; isample++) {
            if( find(cmsswSamplesId.begin(), cmsswSamplesId.end(), isample) != cmsswSamplesId.end() )
                continue;
            
            tNewC3Ds[isample]->GetEntry(ievt);
        } 
        
        //if( ievt%100 == 0 )
        cout << ievt << " events processed." << endl;
        
        /* sample loop */
        for(unsigned isample=0; isample<Nsamples; isample++) {

            unsigned nC3D=0;
            
            /* endcap loop */
            for(unsigned iendcap=0; iendcap<2; iendcap++) {
                
                
                /*** nc2ds loop ***/
                if( c2dFlag ) {
                    if( isample == 0 ) {
                        
                        // get all c2ds
                        vector<const HGCC2D*> c2ds = detectors.at(0)->getSubdet(iendcap, 3)->getAll<HGCC2D>();                    
                        unsigned nC2DperLayer[53];
                        std::uninitialized_fill_n(nC2DperLayer, 53, 0);
                        
                        for( unsigned ic2d=0; ic2d<c2ds.size(); ic2d++ ) {
                            
                            /* nC2Ds per layer*/
                            nC2DperLayer[c2ds.at(ic2d)->layer()+1]++;

                            /* c2d sub loop */
                            unsigned searchingLayerId = 0;
                            
                            if( HGCgeom::instance()->layerZisTriggerLayer( iendcap, c2ds.at(ic2d)->layer()+1 ) ){
                                searchingLayerId = c2ds.at(ic2d)->layer()+1;
                            }
                            else{
                                searchingLayerId = c2ds.at(ic2d)->layer()+2;
                            }
                            
                            // minimum distance of c2d belonging to consecutive trigger layers
                            if( searchingLayerId<=HGCgeom::instance()->nLayers() ) {

                                double minDist=10.;
                                double minPt=0;
                                
                                for( unsigned ic2dsub=0; ic2dsub<c2ds.size(); ic2dsub++ ) {
                                    
                                    if( c2ds.at(ic2d)->id() == c2ds.at(ic2dsub)->id() )
                                        continue;
                                    
                                    if( c2ds.at(ic2dsub)->layer() != searchingLayerId )
                                        continue;
                                    
                                    double XnormDist = c2ds.at(ic2d)->xNorm() - c2ds.at(ic2dsub)->xNorm();
                                    double YnormDist = c2ds.at(ic2d)->yNorm() - c2ds.at(ic2dsub)->yNorm();
                                    double normDist  = TMath::Sqrt( XnormDist*XnormDist + YnormDist*YnormDist );
                                    
                                    if( normDist < minDist ){
                                        minDist = normDist;
                                        minPt = c2ds.at(ic2dsub)->Pt();
                                    }
                                }

                                hhc2dMinDistLayer     ->Fill( searchingLayerId-1, minDist );
                                hhc2dMinDistLayerZoom ->Fill( searchingLayerId-1, minDist );
                                
                                hhc2dMinDistLayerW    ->Fill( searchingLayerId-1, minDist, minPt );
                                hhc2dMinDistLayerWZoom->Fill( searchingLayerId-1, minDist, minPt );

                            }
                            
                            /* gen loop */
                            for( unsigned igen=0; igen<allGen.size(); igen++ ) {
                                
                                if( allGen.at(igen)->getEndcapId() != (int)iendcap ) continue;
                                
                                double c2dPt = c2ds.at(ic2d)->Pt(); // weight
                                
                                double XnormDist = c2ds.at(ic2d)->xNorm() - allGen.at(igen)->xNorm();
                                double YnormDist = c2ds.at(ic2d)->yNorm() - allGen.at(igen)->yNorm();
                                double normDist  = TMath::Sqrt( XnormDist*XnormDist + YnormDist*YnormDist );
                                unsigned c2dLayer = c2ds.at(ic2d)->layer();
                                
                                hc2dGenDist     ->Fill( normDist  ); 
                                hc2dGenDistX    ->Fill( XnormDist ); 
                                hc2dGenDistY    ->Fill( YnormDist );
                                hc2dGenDistZoom ->Fill( normDist  ); 
                                hc2dGenDistXzoom->Fill( XnormDist ); 
                                hc2dGenDistYzoom->Fill( YnormDist );
                                
                                hc2dGenDistLayer[c2dLayer-1]->Fill( normDist ); 
                                hhc2dGenDistLayer           ->Fill(c2dLayer-1, normDist ); 
                                hhc2dGenDistLayerZoom       ->Fill(c2dLayer-1, normDist ); 
                                hhc2dGenDistLayerW          ->Fill(c2dLayer-1, normDist, c2dPt ); 
                                hhc2dGenDistLayerWZoom      ->Fill(c2dLayer-1, normDist, c2dPt ); 
                                hhc2dGenDistPt              ->Fill(c2dPt,      normDist ); 
                                hhc2dGenDistPtZoom          ->Fill(c2dPt,      normDist ); 
                                
                                // W
                                hc2dGenWDist     ->Fill( normDist  , c2dPt ); 
                                hc2dGenWDistX    ->Fill( XnormDist , c2dPt ); 
                                hc2dGenWDistY    ->Fill( YnormDist , c2dPt );
                                hc2dGenWDistZoom ->Fill( normDist  , c2dPt ); 
                                hc2dGenWDistXzoom->Fill( XnormDist , c2dPt ); 
                                hc2dGenWDistYzoom->Fill( YnormDist , c2dPt );
                                
                                hc2dGenWDistLayer[c2dLayer-1]->Fill( normDist , c2dPt ); 
                                hhc2dGenWDistLayer->Fill(c2dLayer-1, normDist, c2dPt ); 
                                hhc2dGenWDistLayerZoom->Fill(c2dLayer-1, normDist, c2dPt ); 
                                
                                /* ghost GEN */
                                double XnormDistGhost = c2ds.at(ic2d)->xNorm() - (-allGen.at(igen)->xNorm());
                                double YnormDistGhost = c2ds.at(ic2d)->yNorm() - (-allGen.at(igen)->yNorm());
                                double normDistGhost  = TMath::Sqrt( XnormDistGhost*XnormDistGhost + YnormDistGhost*YnormDistGhost );
                                //unsigned c2dLayerGhost = c2ds.at(ic2d)->layer();
                            
                                hc2dGenGhostDist     ->Fill( normDistGhost  ); 
                                hc2dGenGhostDistX    ->Fill( XnormDistGhost ); 
                                hc2dGenGhostDistY    ->Fill( YnormDistGhost );
                                hc2dGenGhostDistZoom ->Fill( normDistGhost  ); 
                                hc2dGenGhostDistXzoom->Fill( XnormDistGhost ); 
                                hc2dGenGhostDistYzoom->Fill( YnormDistGhost );
                                
                                hc2dGenGhostDistLayer[c2dLayer-1]->Fill( normDistGhost );
                                hhc2dGenGhostDistLayer->Fill( c2dLayer-1, normDistGhost );
                                hhc2dGenGhostDistLayerZoom->Fill( c2dLayer-1, normDistGhost );
                            
                            }// end gen loop
                            
                        }// end c2d loop

                        for(int ilayer=0; ilayer<53; ilayer++)
                            hhc2dNumberPerLayer->Fill(ilayer,  nC2DperLayer[ilayer]);

                    }
                }
                
                /*** c3ds loop ***/
                if( c3dFlag ) {
                    int nc3dinVec = 0;
                    if( find( cmsswSamplesId.begin(), cmsswSamplesId.end(), isample ) != cmsswSamplesId.end() ) {
                        int idetector = distance ( cmsswSamplesId.begin(), find( cmsswSamplesId.begin(), cmsswSamplesId.end(), isample ) );
                        nc3dinVec = detectors.at( idetector )->getSubdet(iendcap, 3)->getAll<HGCC3D>().size();
                    }
                    else {
                        nc3dinVec = c3ds[isample][iendcap]->size();
                    }
                    
                    vector< pair<int,int> > nC3dPerPenPart;
                    map<int,pair<float,HGCC3D> > closestC3DtoGen;
                    
                    float maxC3Dpt = 0;
                    unsigned maxC3DptId = 0;
                    float genDistHighestC3Dpt = 0;
                    float allC3Dpt = 0;
                    
                    // gen loop
                    //for( unsigned igen=0; igen<allGen.size(); igen++ ) {
                    //    
                    //    if( allGen.at(igen)->getEndcapId() != (int)iendcap ) continue;
                    //
                    //    HGCgen* gen = allGen.at(igen);
                    //    
                    //    for( int ic3d=0; ic3d<nc3dinVec; ic3d++ ) {
                    //        
                    //        HGCC3D c3d;
                    //        
                    //        if( find(cmsswSamplesId.begin(), cmsswSamplesId.end(), isample) != cmsswSamplesId.end() ){
                    //            int idetector = distance ( cmsswSamplesId.begin(), find( cmsswSamplesId.begin(), cmsswSamplesId.end(), isample ) );
                    //            c3d = *( detectors.at(idetector)->getSubdet(iendcap, 3)->getAll<HGCC3D>().at(ic3d) );
                    //            c3d.setNearestGen(allGen); // set the nearest if cone Id
                    //        }
                    //        else {
                    //            c3d = c3ds[isample][iendcap]->at(ic3d);
                    //        }
                    //    
                    //        
                    //
                    //    }
                    //
                    //}        
                        
                    // c3d loop
                    for( int ic3d=0; ic3d<nc3dinVec; ic3d++ ) {
                    
                        HGCC3D c3d;

                        if( find(cmsswSamplesId.begin(), cmsswSamplesId.end(), isample) != cmsswSamplesId.end() ){
                            int idetector = distance ( cmsswSamplesId.begin(), find( cmsswSamplesId.begin(), cmsswSamplesId.end(), isample ) );
                            c3d = *( detectors.at(idetector)->getSubdet(iendcap, 3)->getAll<HGCC3D>().at(ic3d) );
                            c3d.setNearestGen(allGen); // set the nearest if cone Id
                        }
                        else {
                            c3d = c3ds[isample][iendcap]->at(ic3d);
                        }
                        /******************/
                        /* !!! Pt CUT !!! */
                        /******************/
                        if( c3d.Pt() < ptCut ) continue;
                    
                        /* energy density */
//density                    float density = c3d.getEnergyDensity(0.01);
//density                    float densityTC = c3d.getEnergyDensity();
//density                    hc3dEnergyDensity[isample]->Fill( density );
//density                    hhc3dEnergyDensityTCvsEnergy[isample]->Fill( densityTC, c3d.Energy() );
                        //cout << densityTC << " " << c3d.Energy() << endl; 
                        /* total pt */
                        allC3Dpt += c3d.Pt();

                        /* get nearest gen */
                        HGCgen gen=c3d.getNearestGen();
                    
                        /* calculate features */
                        double XnormDist = c3d.xNorm() - gen.xNorm();
                        double YnormDist = c3d.yNorm() - gen.yNorm();
                        double normDist  = TMath::Sqrt( XnormDist*XnormDist + YnormDist*YnormDist );
                    
                        if( c3d.Pt() > maxC3Dpt ) {
                            maxC3Dpt  = c3d.Pt();
                            maxC3DptId= c3d.id();
                            genDistHighestC3Dpt = normDist;
                        }

                        if( closestC3DtoGen.find( gen.id() )==closestC3DtoGen.end() ) {
                            closestC3DtoGen[gen.id()] = pair<float,HGCC3D>(normDist, c3d);
                        } else {
                            if( normDist<closestC3DtoGen[gen.id()].first )
                                closestC3DtoGen[gen.id()] = pair<float,HGCC3D>(normDist, c3d);
                        }
                    
                        if( c3d.nclusters() < 3 ) { 

                            vector<unsigned> c2ds = c3d.clusters();
                        
                            for(unsigned ic2d=0; ic2d< c2ds.size(); ic2d++ ){
                                HGCC2D c2d;
                                detectors.at(0)->getSubdet(iendcap, 3)->get( c2ds.at(ic2d), c2d );
                                double c2dXnormDist = c2d.xNorm() - gen.xNorm();
                                double c2dYnormDist = c2d.yNorm() - gen.yNorm();
                                double c2dnormDist  = TMath::Sqrt( c2dXnormDist*c2dXnormDist + c2dYnormDist*c2dYnormDist );

                                hhDistLowPopulatedC3DgenVsLayer[isample]->Fill( c2d.layer(), c2dnormDist );
                                hhDistLowPopulatedC3DgenVsLayerZoom[isample]->Fill( c2d.layer(), c2dnormDist );

                            }

                        }

//                    if( XnormDist<0.01 && YnormDist<0.01 ){
                        if( normDist<0.01 ){

                            bool addGen=true;
                            for( unsigned igen=0; igen<nC3dPerPenPart.size(); igen++ ){
                                if( nC3dPerPenPart.at(igen).first == gen.id() ){
                                    addGen=false;
                                    nC3dPerPenPart.at(igen).second++;
                                    break;
                                }
                            }
                            if( addGen ) nC3dPerPenPart.push_back( pair<int,int>(gen.id(), 1) ); 
                        }
                        nC3D++;

                        /* loooping over the C2Ds components of the C3D */
                        vector<unsigned> c2dIds = c3d.clusters();
                        bool hit[HGCgeom::instance()->nLayers()];
                        std::fill_n(hit, HGCgeom::instance()->nLayers(), false);
                        float percentage[HGCgeom::instance()->nLayers()];
                        std::fill_n(percentage, HGCgeom::instance()->nLayers(), 0.);
                        unsigned nC2D[53];
                        std::uninitialized_fill_n(nC2D, 53, 0);
                    
                        for(vector<unsigned>::iterator c2dId=c2dIds.begin(); c2dId!=c2dIds.end(); c2dId++) {
                            HGCC2D c2d;
                            detectors.at(0)->getSubdet(iendcap, 3)->get<HGCC2D>(*c2dId, c2d);
                            hit[c2d.layer()] = true;
                            percentage[c2d.layer()] = c2d.Pt()/c3d.Pt();
                            nC2D[c2d.layer()]++;
                        }

                        /* Filling */ 
                        hNc2d[isample]             ->Fill( c3d.nclusters()    );
                        hc3dPt[isample]            ->Fill( c3d.Pt()           );
                        hc3dFirstLayer[isample]    ->Fill( c3d.getFirstLayer());
                        hc3dMaxLayer[isample]      ->Fill( c3d.getMaxLayer() );
                        h2FirstVsMaxLayer[isample] ->Fill( c3d.getMaxLayer(), c3d.getFirstLayer() );
                        hc3dGenDist[isample]       ->Fill( normDist  ); 
                        hc3dGenDistX[isample]      ->Fill( XnormDist ); 
                        hc3dGenDistY[isample]      ->Fill( YnormDist );
                        hc3dGenDistZoom[isample]   ->Fill( normDist  ); 
                        hc3dGenDistXzoom[isample]  ->Fill( XnormDist ); 
                        hc3dGenDistYzoom[isample]  ->Fill( YnormDist );
                    
                        for(int ilayer=0; ilayer<53; ilayer++)
                            hhc3dc2dNumberPerLayer[isample]->Fill( ilayer,  nC2D[ilayer] );

                    } // end c3ds loop
                
                    /* highest pt c3d */
                    HGCC3D c3dHighestPt;
//density                float clusterDensity, regionDensity;
                    if( find(cmsswSamplesId.begin(), cmsswSamplesId.end(), isample) != cmsswSamplesId.end() ){

                        int idetector  = distance ( cmsswSamplesId.begin(), find( cmsswSamplesId.begin(), cmsswSamplesId.end(), isample ) );
                        detectors.at(idetector)->getSubdet(iendcap, 3)->get<HGCC3D>(maxC3DptId, c3dHighestPt);
//density                    clusterDensity = c3dHighestPt.getEnergyDensity(0.01);
//density                    regionDensity  = c3dHighestPt.getEnergyDensity( detectors.at(idetector)->getSubdet(iendcap, 3)->getAll<HGCC3D>(), 0.1, 0.01 );
                    
                    }
                    else {
                        for( int ic3d=0; ic3d<nc3dinVec; ic3d++ ) {
                            if( maxC3DptId == c3ds[isample][iendcap]->at(ic3d).id() )
                                c3dHighestPt = c3ds[isample][iendcap]->at(ic3d);
                        }
                    
//density                    clusterDensity = c3dHighestPt.getEnergyDensity(0.01);
//density                    regionDensity  = c3dHighestPt.getEnergyDensity( c3ds[isample][iendcap], 0.1, 0.01 );
                    
                    }

//density                hhc3dEnergyDensityRatio[isample]->Fill( clusterDensity, 
//density                                                        regionDensity 
//density                    );
                
//density                gc3dEnergyDensityRatio[isample]->SetPoint(gc3dEnergyDensityRatio[isample]->GetN(),
//density                                                          clusterDensity, 
//density                                                          regionDensity 
//density                   );
            
                    

                    /* filling c3d's related histos*/
                    hPtHighestC3D[isample]->Fill( maxC3Dpt );
                    hPtAllC3D[isample]->Fill( allC3Dpt );
                    hDistHighestC3Dgen[isample]->Fill( genDistHighestC3Dpt );
                    hDistHighestC3DgenZoom[isample]->Fill( genDistHighestC3Dpt );
                    hhDistHighestC3DgenVsEvt[isample]->Fill( ievt, genDistHighestC3Dpt );

                    for(map<int,pair<float,HGCC3D> >::iterator it = closestC3DtoGen.begin(); it != closestC3DtoGen.end(); ++it) {
                        hPtClosestC3D[isample]->Fill( it->second.second.Pt() );
                    }


                    for( unsigned igen=0; igen<nC3dPerPenPart.size(); igen++ ){
                        hNc3dWthinGenProj[isample]->Fill( nC3dPerPenPart.at(igen).second );
                    }

                }
            }

            hNc3d[isample]->Fill( nC3D );            
            hNc3dZoom[isample]->Fill( nC3D );
            
        }

    } // end event loop

    cout << " >>> all events processed " << endl;
    
    /* canvases */
    TFile fOut(fOutName, "recreate");
    TCanvas *cAll[Nfeatures];
    TCanvas cResoHighest("cResoHighest", "cResoHighest", 1);
    TCanvas cResoClosest("cResoClosest", "cResoClosest", 1);
    TCanvas cResoAll("cResoAll", "cResoAll", 1);

    for( unsigned ifeature=0; ifeature<Nfeatures; ifeature++ ) {
        cAll[ifeature] = new TCanvas( ("c"+hAll.at(ifeature).name).c_str(), ("c"+hAll.at(ifeature).name).c_str(), 1 );
        cAll[ifeature]->Divide( 3, 2 );
    }

    /* c2d */
    for(unsigned ilayer=0; ilayer<HGCgeom::instance()->nLayers(); ilayer++) { 
        hc2dGenDistLayer[ilayer]->Write();
        hc2dGenWDistLayer[ilayer]->Write();
        hc2dGenGhostDistLayer[ilayer]->Write();
    }
    hhc2dGenDistLayer ->Write();
    hhc2dGenDistLayer ->ProfileX()->Write();
    hhc2dGenDistLayerW->Write();
    hhc2dGenDistLayerW->ProfileX()->Write();
    hhc2dMinDistLayer ->Write();
    hhc2dMinDistLayer ->ProfileX()->Write();
    hhc2dMinDistLayerW->Write();
    hhc2dMinDistLayerW->ProfileX()->Write();
    hhc2dGenDistPt    ->Write();
    hc2dGenDist       ->Write(); 
    hc2dGenDistX      ->Write(); 
    hc2dGenDistY      ->Write();
    hhc2dGenDistLayerZoom ->Write();
    hhc2dGenDistLayerZoom ->ProfileX()->Write();
    hhc2dGenDistLayerWZoom->Write();
    hhc2dGenDistLayerWZoom->ProfileX()->Write();
    hhc2dMinDistLayerZoom ->Write();
    hhc2dMinDistLayerZoom ->ProfileX()->Write();
    hhc2dMinDistLayerWZoom->Write();
    hhc2dMinDistLayerWZoom->ProfileX()->Write();
    hhc2dGenDistPtZoom->Write();
    hc2dGenDistZoom   ->Write(); 
    hc2dGenDistXzoom  ->Write(); 
    hc2dGenDistYzoom  ->Write(); 
    hhc2dNumberPerLayer->Write();

    hhc2dGenWDistLayer ->Write();
    hc2dGenWDist       ->Write(); 
    hc2dGenWDistX      ->Write(); 
    hc2dGenWDistY      ->Write();
    hhc2dGenWDistLayerZoom ->Write();
    hc2dGenWDistZoom   ->Write(); 
    hc2dGenWDistXzoom  ->Write(); 
    hc2dGenWDistYzoom  ->Write(); 

    hhc2dGenGhostDistLayer ->Write();
    hc2dGenGhostDist       ->Write(); 
    hc2dGenGhostDistX      ->Write(); 
    hc2dGenGhostDistY      ->Write();
    hhc2dGenGhostDistLayerZoom ->Write();
    hc2dGenGhostDistZoom   ->Write(); 
    hc2dGenGhostDistXzoom  ->Write(); 
    hc2dGenGhostDistYzoom  ->Write(); 

    
    for(unsigned isample=0; isample<Nsamples; isample++){
        hhDistHighestC3DgenVsEvt[isample]->Write();
        hhDistLowPopulatedC3DgenVsLayer[isample]->Write();
        hhDistLowPopulatedC3DgenVsLayerZoom[isample]->Write();
        hhc3dc2dNumberPerLayer[isample]->Write();
//density        gc3dEnergyDensityRatio[isample]->Write();
//density        hhc3dEnergyDensityRatio[isample]->Write();
//density        hhc3dEnergyDensityTCvsEnergy[isample]->Write();
    }

    /* plotting into canvases */
    TLegend gResoHighestLeg(0.75, 0.6, 1, 1);
    TLegend gResoClosestLeg(0.75, 0.6, 1, 1);
    TLegend gResoAllLeg(0.75, 0.6, 1, 1);
    for( unsigned ifeature=0; ifeature<Nfeatures; ifeature++ ) {

        TLegend *leg[6];
       
        for(int i=0; i<6; i++) {
            leg[i] = new TLegend(0.6, 0.575, 0.9, 0.9 ); 
        }

        for(unsigned isample=0; isample<Nsamples; isample++){
            
            int nPad=1;    
            string opt="hist";
            if ( isample!=0 )
                opt="hist,same";

            if ( files[isample].first.first == "cone"       ||           
                 files[isample].first.first == "multicone"   ||     
                 files[isample].first.first == "multicone02" ||
                 files[isample].first.first == "multiconeMR"
                ) {            
                nPad=1;
                gPtHighestC3D[isample]->SetMarkerStyle( 24 );
                gPtClosestC3D[isample]->SetMarkerStyle( 24 );
                gPtAllC3D[isample]->SetMarkerStyle( 24 );
                if( files[isample].first.first != "cone" ) 
                    opt="hist,same";
            }
            if ( files[isample].first.first == "forest"      ) {            
                nPad=2;
                gPtHighestC3D[isample]->SetMarkerStyle( 24 );
                gPtClosestC3D[isample]->SetMarkerStyle( 24 );
                gPtAllC3D[isample]->SetMarkerStyle( 24 );
            }
            if ( files[isample].first.first == "single"      ) {            
                nPad=2;
                gPtHighestC3D[isample]->SetMarkerStyle( 24 );
                gPtClosestC3D[isample]->SetMarkerStyle( 24 );
                gPtAllC3D[isample]->SetMarkerStyle( 24 );
                opt="hist,same";
            }
            if ( files[isample].first.first == "norm9x9_R003"   ||
                 files[isample].first.first == "norm9x9_R00425" ||
                 files[isample].first.first == "norm9x9_R01"    || 
                 files[isample].first.first == "norm9x9_R02"    || 
                 files[isample].first.first == "norm9x9_R03"    || 
                 files[isample].first.first == "norm9x9_R04"    || 
                 files[isample].first.first == "norm9x9_R1" 
                ) {
                nPad=3;
                gPtHighestC3D[isample]->SetMarkerStyle( 24 );
                gPtClosestC3D[isample]->SetMarkerStyle( 24 );
                gPtAllC3D[isample]->SetMarkerStyle( 24 );
            } 
            if ( files[isample].first.first == "norm5x5_R003"   ||
                 files[isample].first.first == "norm5x5_R00425" ||
                 files[isample].first.first == "norm5x5_R01"    || 
                 files[isample].first.first == "norm5x5_R02"    || 
                 files[isample].first.first == "norm5x5_R03"    || 
                 files[isample].first.first == "norm5x5_R04"    ||
                 files[isample].first.first == "norm5x5_R1" 
                ) {
                nPad=4;
                gPtHighestC3D[isample]->SetMarkerStyle( 25 );
                gPtClosestC3D[isample]->SetMarkerStyle( 25 );
                gPtAllC3D[isample]->SetMarkerStyle( 25 );
            } 
            if ( files[isample].first.first == "gen_R01" ||
                 files[isample].first.first == "gen_R02" ||
                 files[isample].first.first == "gen_R1"    
                ) {
                nPad=5;
                gPtHighestC3D[isample]->SetMarkerStyle( 26 );
                gPtClosestC3D[isample]->SetMarkerStyle( 26 );
                gPtAllC3D[isample]->SetMarkerStyle( 25 );
            } 


            leg[nPad-1]->AddEntry( hAll[ifeature].histo[isample], files[isample].first.first.c_str(), "l" );  

            // select the correct pad
            cAll[ifeature]->cd(nPad);
          
            // set line attributes
            (hAll[ifeature].histo)[isample]->SetLineColor( files[isample].first.second );
            if( (hAll[ifeature].histo)[isample]->Integral()>0 )
                (hAll[ifeature].histo)[isample]->Scale( 1/(hAll[ifeature].histo)[isample]->Integral() );
            
            (hAll[ifeature].histo)[isample]->Draw( opt.c_str() );
            (hAll[ifeature].histo)[isample]->GetYaxis()->SetRangeUser( hAll[ifeature].range->min, hAll[ifeature].range->max);

            if( files[isample].first.first == "cone"   ||
                files[isample].first.first == "multicone"   ||
                files[isample].first.first == "multiconeMR" ||
                files[isample].first.first == "gen_R1" ||
                files[isample].first.first == "forest" ||
                files[isample].first.first == "single" ||
                files[isample].first.first == "norm9x9_R1" ||
                files[isample].first.first == "norm5x5_R1" ||
                files[isample].first.first == "norm3x3_R1"
                ) {

                // select the correct pad
                cAll[ifeature]->cd(6);
          
                //
                TH1D* copy = (TH1D*)(hAll[ifeature].histo)[isample]->Clone();
                leg[5]->AddEntry( copy, files[isample].first.first.c_str(), "l" );  
                
                if( files[isample].first.first == "norm9x9_R01" )
                    copy->SetLineColor(kBlue);
                if( files[isample].first.first == "norm5x5_R01" )
                    copy->SetLineColor(kBlue+10);
                if( files[isample].first.first == "norm3x3_R01" )
                    copy->SetLineColor(kBlue-9);
                
                copy->Draw( opt.c_str() );
                copy->GetYaxis()->SetRangeUser( hAll[ifeature].range->min, hAll[ifeature].range->max);
                
                
            }
            
            /* graphs */
            float mean = hAll.at(ifeature).histo[isample]->GetMean(1);
            float rms  = hAll.at(ifeature).histo[isample]->GetRMS(1);

            // get the mean and rms for the highest pt c3d
            if( hAll.at(ifeature).name == "PtHighestC3D" ||
                hAll.at(ifeature).name == "PtClosestC3D" ||
                hAll.at(ifeature).name == "PtAllC3D"   
                ) {
                
                TLegend *legend;
                TGraphErrors *g;
                if( hAll.at(ifeature).name == "PtHighestC3D" ) {
                    cResoHighest.cd();
                    g = gPtHighestC3D[isample];
                    legend = &gResoHighestLeg;
                }
                if( hAll.at(ifeature).name == "PtClosestC3D" ) {
                    cResoClosest.cd();
                    g = gPtClosestC3D[isample];
                    legend = &gResoClosestLeg;
                }
                if( hAll.at(ifeature).name == "PtAllC3D" ) {
                    cResoAll.cd();
                    g = gPtAllC3D[isample];
                    legend = &gResoAllLeg;
                }
                
                g->SetMarkerColor( files[isample].first.second );
                g->SetLineColor( files[isample].first.second );

                g->SetPoint(0, mean/50, isample );
                g->SetPointError(0, rms/50, 0 );
                if(isample ==0)
                    g->Draw("AP");
                else
                    g->Draw("sameP");
               
                legend->AddEntry( g, files[isample].first.first.c_str(), "lep" );  
                legend->Draw();  
                g->GetYaxis()->SetRangeUser(-1, Nsamples+1);
                g->GetXaxis()->SetLimits(-0.1, 1.5);
                
                cResoClosest.SetGridx();
                cResoHighest.SetGridx();
                cResoAll.SetGridx();


                cResoClosest.Update();
                cResoHighest.Update();
                cResoAll.Update();
                
            }

        } // end samples

        for(int i=0; i<6; i++) {
            cAll[ifeature]->cd(i+1);
            leg[i]->Draw();
            if( hAll.at(ifeature).name == "PtClosestC3D" ||
                hAll.at(ifeature).name == "PtHighestC3D" ||
                hAll.at(ifeature).name == "PtAllC3D" ||
                hAll.at(ifeature).name == "Pt"
                ) {
                TVirtualPad::Pad()->SetLogy();

            }
        }
     
    } // end features

    /* storing results */
    for( unsigned ifeature=0; ifeature<Nfeatures; ifeature++ ) {

        cAll[ifeature]->Write();
        cAll[ifeature]->SaveAs( ( "pi_"+PU+"_E"+ene+"/"+hAll[ifeature].name+".png" ).c_str() );
        cAll[ifeature]->SaveAs( ( "pi_"+PU+"_E"+ene+"/"+hAll[ifeature].name+".pdf" ).c_str() );
        
        for(unsigned isample=0; isample<Nsamples; isample++){
            (hAll[ifeature].histo)[isample]->Write();
        }

    }

    cResoHighest.Write();
    cResoClosest.Write();
    cResoAll.Write();
    
    fOut.Close();

    app.Run();

    return 0;

} // macro's end
