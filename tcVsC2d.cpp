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

#include <plotRatio.h>

using namespace std;

class listItem {

public:
    
    listItem( char *h1, char *h2, char *c, bool ly=false ){
        h1name = h1;
        h2name = h2;
        cName  = c;
        logY = ly; 
    }
    char *h1name;
    char *h2name;
    char *cName;
    bool logY;

};

int main() {

//    TFile f("pi_noPU_E50/canvases.root", "OPEN");
    TFile f("pi_PU200_E50/canvases.root", "OPEN");

    vector<listItem> listToPlot;
    
    bool logy=true;

    listToPlot.push_back( listItem("hc3dPt_polarFW_R02", "hc3dPt_polarFWtc_R02",     "PtC2DvsTC_R02"   , logy) );
    listToPlot.push_back( listItem("hc3dPt_polarFW_R02", "hc3dPt_cone",              "PtC2DvsCone_R02" , logy) );
    listToPlot.push_back( listItem("hc3dPt_polarFW_R01", "hc3dPt_polarFWtc_R01",     "PtC2DvsTC_R01"   , logy) );
    listToPlot.push_back( listItem("hc3dPt_polarFW_R01", "hc3dPt_cone",              "PtC2DvsCone_R01" , logy) );
    listToPlot.push_back( listItem("hc3dPt_polarFW_R01", "hc3dPt_polarFW_R02",       "PtC2D_R01vsR02"  , logy) );
    listToPlot.push_back( listItem("hc3dPt_polarFW_R02", "hc3dPt_polarFW_R03",       "PtC2D_R02vsR03"  , logy) );
    listToPlot.push_back( listItem("hc3dPt_polarFW_R01", "hc3dPt_polarFW_R03",       "PtC2D_R01vsR03"  , logy) );
    
    listToPlot.push_back( listItem("hc3dGenDistZoom_polarFW_R01", "hc3dGenDistZoom_polarFWtc_R01",       "GenDistC2DvsTC_R01")  );
    listToPlot.push_back( listItem("hc3dGenDistZoom_polarFW_R02", "hc3dGenDistZoom_polarFWtc_R02",       "GenDistC2DvsTC_R02")  );
    listToPlot.push_back( listItem("hc3dGenDistZoom_polarFW_R03", "hc3dGenDistZoom_polarFWtc_R03",       "GenDistC2DvsTC_R03")  );
    
    listToPlot.push_back( listItem("hc3dGenDistZoom_polarFW_R01", "hc3dGenDistZoom_cone",       "GenDistC2DvsCONE_R01")  );
    
    for( auto item = listToPlot.begin(); item != listToPlot.end(); item++ ) {

        TH1D* h1 = (TH1D*)f.Get(item->h1name);
        TH1D* h2 = (TH1D*)f.Get(item->h2name);

        if( h1!= NULL && h2!=NULL)
            plotRatio(h1, h2, item->cName, '/', item->logY);
        else 
            cout << item->h1name << " or " << item->h2name << " not found in file." << endl;

    }

//    TH1D *h  = new TH1D("h", "h", 100, 0, 1);
//    TH1D *h2 = new TH1D("h2", "h2", 100, 0, 1);
//    
//    h->FillRandom("gaus", 1000);
//    h2->FillRandom("landau", 1000);
//    
//    plotRatio(h, h2, "canvasName", '/');

    return 0;

}
