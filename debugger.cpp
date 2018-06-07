#include <iostream>

#include <TApplication.h>
#include <TROOT.h>
#include <TStyle.h>
#include <TList.h>
#include <TTree.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TH1D.h>
#include <TLegend.h>

#include <HGC.h>
#include <HGCC3D.h>
#include <HGCgen.h>

using namespace std;

int main(int argc, char** argv){

    cout << "opening file" << argv[1] << endl;
    TFile *fsamples = TFile::Open( argv[1], "read");


    TApplication app("the app", &argc, argv);

    int nentries = -10;

//    gStyle->SetOptStat(0);
//    gStyle->SetOptTitle(0);

    double ptCut=0.5; // Pt cut for C3D (as in CMSSW L1Trigger/L1THGCal/python/hgcalTriggerPrimitiveDigiProducer_cfi.py )
    
    /* samples */
    
    //const unsigned Nfeatures = (const unsigned) features.size();
    const unsigned Nsamples  = 1;
    

    /* histos */
    TH1D *hNc3d            = new TH1D( "hNc3d"             , "hNc3d"             , 200  , 0  , 4000 );
    TH1D *hNc2d            = new TH1D( "hNc2d"             , "hNc2d"             , 20   , 0  , 20   );
    TH1D *hc3dPt           = new TH1D( "hc3dPt"            , "hc3dPt"            , 400  , 0  , 200  );
    TH1D *hc3dFirstLayer   = new TH1D( "hc3dFirstLayer"    , "hc3dFirstLayer"    , 52   , 0  , 52   );
    TH1D *hc3dMaxLayer     = new TH1D( "hc3dMaxLayer"      , "hc3dMaxLayer"      , 52   , 0  , 52   );
    TH1D *hc3dGenDistX     = new TH1D( "hc3dGenDistX"      , "hc3dGenDistX"      , 5000 , -1 , 1    );
    TH1D *hc3dGenDistY     = new TH1D( "hc3dGenDistY"      , "hc3dGenDistY"      , 5000 , -1 , 1    );
    TH1D *hc3dGenDist      = new TH1D( "hc3dGenDist"       , "hc3dGenDist"       , 2500 , 0  , 1    );
    TH1D *hc3dGenDistXzoom = new TH1D( "hc3dGenDistXzoom"  , "hc3dGenDistXzoom"  , 500  , -.1, .1   );
    TH1D *hc3dGenDistYzoom = new TH1D( "hc3dGenDistYzoom"  , "hc3dGenDistYzoom"  , 500  , -.1, .1   );
    TH1D *hEresolution     = new TH1D( "hEresolution"      , "hEresolutin"       , 400  , 0  , 200  );
    TH1D *hNc3dWthinGenProj= new TH1D( "hNc3dWthinGenProj" , "hNc3dWthinGenProj" , 20   , 0  , .20  );

    TH2D *h2PtVsDistance   = new TH2D( "h2PtVsDistance"    , "h2PtVsDistance"    , 400  , 0  , 200, 2500 , 0  , 1 );

    vector< pair<TObject*, string> > hAll      = { 
        std::pair<TH1D*,string>( hNc3d             , "Nc3d"            ), 
        std::pair<TH1D*,string>( hNc2d             , "Nc2d"            ),
        std::pair<TH1D*,string>( hc3dPt            , "Pt"              ),
        std::pair<TH1D*,string>( hc3dFirstLayer    , "FirstLayer"      ),
        std::pair<TH1D*,string>( hc3dMaxLayer      , "MaxLayer"        ),
        std::pair<TH1D*,string>( hc3dGenDistX      , "c3dGenDistX"     ),
        std::pair<TH1D*,string>( hc3dGenDistY      , "c3dGenDistY"     ),
        std::pair<TH1D*,string>( hc3dGenDist       , "c3dGenDist"      ),
        std::pair<TH1D*,string>( hc3dGenDistXzoom  , "c3dGenDistXzoom" ),
        std::pair<TH1D*,string>( hc3dGenDistYzoom  , "c3dGenDistYzoom" ),
        std::pair<TH1D*,string>( hEresolution      , "EResolution"     ),
        std::pair<TH1D*,string>( hNc3dWthinGenProj , "Nc3dWthinGenProj"),
        std::pair<TH2D*,string>( h2PtVsDistance    , "h2PtVsDistance"  )
    };

    const unsigned Nfeatures = hAll.size();

    vector< pair<float,float> > featuresYranges = { std::pair<float,float>(0, 0.4),             // Nc3d"            
                                                    std::pair<float,float>(0, 0.6),             // Nc2d"            
                                                    std::pair<float,float>(0.00000001, 0.5),    // Pt"              
                                                    std::pair<float,float>(0, 0.3),             // FirstLayer"      
                                                    std::pair<float,float>(0, 0.3),             // MaxLayer"        
                                                    std::pair<float,float>(0, 0.01),            // c3dGenDistX"     
                                                    std::pair<float,float>(0, 0.01),            // c3dGenDistY"     
                                                    std::pair<float,float>(0, 0.01),            // c3dGenDist"      
                                                    std::pair<float,float>(0, 0.02),            // c3dGenDistXzoom" 
                                                    std::pair<float,float>(0, 0.02),            // c3dGenDistYzoom" 
                                                    std::pair<float,float>(0.00000001, 0.75),   // EResolution"     
                                                    std::pair<float,float>(0, 0.3),             // Nc3dWthinGenProj"
                                                    std::pair<float,float>(0, 1  )              // PtVsDist
    };
    

    TString name("");
    
    TH2D *h2FirstVsMaxLayer;

//    vector<TH2D**> h2All      = { h2FirstVsMaxLayer };

    h2FirstVsMaxLayer = new TH2D( "h2FirstVsMaxLayer",  
                                   "h2FirstVsMaxLayer",
                                   52, 0, 52,
                                   52, 0, 52
        );


    /* cmssw */
    //TList* fList = new TList(); 
    //fList->Add( new TObjString( TString(fileNames[coneId]) ) );
    
    //HGC detector( fList, 
    //               false, false, true, false, false, 
    //              false,
    //              0 
    //    );  

    /* prod */
    TTree *t[2]; 
    std::vector<HGCC3D> *c3ds[2]; // [sample][endcap]
    
    /* trees */            
    c3ds[0]=0;
    c3ds[1]=0;
    t[0] = (TTree*)fsamples->Get("subdet_endcap0_section3");
    t[1] = (TTree*)fsamples->Get("subdet_endcap1_section3");
    
    t[0]->SetBranchAddress("c3d_norm", &c3ds[0]);
    t[1]->SetBranchAddress("c3d_norm", &c3ds[1]);
    
    /* number of events */
    nentries = nentries>0 ? nentries : t[0]->GetEntries();
    
    /* Loop */
    cout << " >>> looping on " << nentries << " entries" << endl;
    
    for(int ievt=0; ievt<nentries; ievt++) {
     
//        detector.getEvent(ievt);

        t[0]->GetEntry(ievt);
        t[1]->GetEntry(ievt);        

        if( ievt%100 == 0 )
            cout << ievt << " events processed." << endl;
        
        unsigned nC3D=0;
        
        for(unsigned iendcap=0; iendcap<2; iendcap++) {
            
            int nc3dinVec = c3ds[iendcap]->size();
            
            /* looping over c3ds */
            for(int ic3d=0; ic3d<nc3dinVec; ic3d++) {
                
                const HGCC3D c3d = c3ds[iendcap]->at(ic3d);
                
                if( c3d.Pt() < ptCut ) {
                    cout << "event " << ievt 
                         << endl;
                    
                    continue; 
                }
                
                HGCgen gen=c3d.getNearestGen();
                
                ROOT::Math::RhoEtaPhiPoint genProj = gen.getZprojection( c3d.z() );
                
                double XnormDist = c3d.xNorm() - genProj.X()/genProj.Z();
                double YnormDist = c3d.yNorm() - genProj.Y()/genProj.Z();
                double normDist = TMath::Sqrt(XnormDist*XnormDist+YnormDist*YnormDist );

                hNc2d         ->Fill( c3d.nclusters()    );
                hc3dPt        ->Fill( c3d.Pt()           );
                hc3dFirstLayer->Fill( c3d.getFirstLayer());
                hc3dMaxLayer  ->Fill( c3d.getMaxLayer() );
                h2FirstVsMaxLayer->Fill ( c3d.getMaxLayer(), 
                                          c3d.getFirstLayer()
                    );
                h2PtVsDistance->Fill( c3d.Pt(),
                                      normDist
                    );
                

                hc3dGenDistX      ->Fill( XnormDist ); 
                hc3dGenDistY      ->Fill( YnormDist );
                hc3dGenDist       ->Fill( normDist );
                hc3dGenDistXzoom  ->Fill( XnormDist ); 
                hc3dGenDistYzoom  ->Fill( YnormDist );
                if( XnormDist<0.01 && YnormDist<0.005 ){
                    hEresolution->Fill( c3d.Pt() );
                }
                nC3D++;
                
            }

        hNc3d->Fill( nC3D );

        }
        
    }
  
    /* canvases */
    TCanvas *cAll[Nfeatures];

    for( unsigned ifeature=0; ifeature<Nfeatures; ifeature++ ) {
        cAll[ifeature]= new TCanvas( ("c"+hAll.at(ifeature).second).c_str(), ("c"+hAll.at(ifeature).second).c_str(), 1 );
    }

    /* plotting into canvases */
    
    string opt="hist";
    
    for( unsigned ifeature=0; ifeature<Nfeatures; ifeature++ ) {
        cAll[ifeature]->cd();
        
        //if( ((TH1D*)(hAll[ifeature].first))->Integral()>0 )
        //    ((TH1D*)(hAll[ifeature].first))->Scale( 1/((TH1D*)(hAll[ifeature].first))->Integral() );
        
        hAll[ifeature].first->Draw( opt.c_str() );
        
    }
    
    
    for( unsigned ifeature=0; ifeature<Nfeatures; ifeature++ ) {
        //   ((TH1D*)(hAll[ifeature].first))->GetYaxis()->SetRangeUser( featuresYranges[ifeature].first, featuresYranges[ifeature].second);    
    }
    
    /* storing results */
    app.Run();

    return 0;

} // macro's end
