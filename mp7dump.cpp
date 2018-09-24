
/* C++ */
#include <iostream>
#include <vector>
#include <sstream>
#include <stdlib.h>
#include <getopt.h>

/* ROOT */
//#include <TString.h>
//#include <TFile.h>
//#include <TChain.h>
//#include <TTree.h>
//#include <TApplication.h>
//#include <TH1.h>
//#include <TH2.h>
//#include <TGraph.h>
//#include <TMultiGraph.h>
//#include <TCanvas.h>
//#include <TList.h>
//#include <TF1.h>
//#include <TLine.h>
//
/* myIncludes*/
#include "HGCgeom.h"
//#include "HistoContainer.h"
#include "HGC.h"
#include "HGCTC.h"
#include "HGCC2D.h"
#include "HGCROC.h"
//#include "Ntuplizer.h"
//#include "HGCht.h"
//#include "HGCpolarHisto_T.h"

#include "MP7gen.h"

using namespace std;


/* some handy functions */
bool isIndexEven(HGCTC i){

    static int index = 0;

    if( index++ % 2 == 0 )
        return true;
    else
        return false;

}


bool isIndexOdd(HGCTC i){

    static int index = 0;

    if( index++ % 2 !=0 )
        return true;
    else
        return false;

}


bool sortByRZ (HGCTC a, HGCTC b) { 
    
    float A = a.r()/abs(a.z());
    float B = b.r()/abs(b.z()); 

    return (A<B); 

}


bool isBit(unsigned word, unsigned bit) { 
    
    if( (word & bit) == bit )
        return true;
    return false;

}

/* VERBOSE CONTROL */
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

const unsigned nLinks = 72;


int main(int argc, char **argv){

    /* PARSING THE COMMAND LINE ARGUMENTS */
    int opt;

    // files and event control
    TString inputFileName;
    string  outputFileName("tmp.root");
    int iEvt = 0;
    
    // graphics
//    bool persistentGraphics=false;

    const struct option longOptions[] = {
        {"help",         no_argument,        0, 'h'},
        {"iFile",        required_argument,  0, 'i'},
        {"oFile",        required_argument,  0, 'o'},
        {"iEvt",         required_argument,  0, 'e'},
        {0,0,0,0}
    };

    int optIndex = 0;
    while ( (opt = getopt_long (argc, argv, "hi:o:e:", longOptions, &optIndex) ) != -1 ) {
        
        switch (opt)
        {
        case 'h':
            cout << "Help" << endl;
            cout << " -h(--help        ) :\t shows this help." << endl; 
            cout << " -i(--iFile       ) <ifile> :\t single file input." << endl; 
            cout << " -o(--oFile       ) <ofile> :\t output file." << endl; 
            cout << " -e(--iEvt        ) <iEvt>  :\t event to process." << endl; 
            return 0;
            break;
        case 'i':
            inputFileName = optarg;
            break;
	case 'o':
            outputFileName = optarg;
            break;
	case 'e':
            iEvt = atoi(optarg);
            break;
        default:
            return 0;
        }
    }
    
    if( isBit(verboselevel, VERBOSE_OPTIONS) ){
        cout << "Options "           << endl;
        cout << "  OutputFile: "     << outputFileName << endl;
        cout << "  InputFile: "      << inputFileName  << endl;
        cout << "  iEvt: "           << iEvt           << endl;
    }

    /***************************************/
    /* get the files and fill the detector */

    TList *fList = new TList();

    fList->Add( new TObjString( inputFileName ) );

    /* build the detector */
    HGC detector( fList, 
                  flagTCs, flagC2D, flagC3D, flagGen, flagGenPart, 
                  true,
                  verboselevel 
        );   

    /***************/
    /* phi sectors */
    
    unsigned nPhiSectors = 12;

    double minPhis[nPhiSectors];
    double maxPhis[nPhiSectors];
    double pi = TMath::Pi();
    double dPhi = ( 2*pi )/( nPhiSectors ); // NO overlap
    for(unsigned isector=0; isector<nPhiSectors; isector++){

        minPhis[isector] = - pi + dPhi * (isector) ; // NO overlap
        maxPhis[isector] = - pi + dPhi * (isector+1) ;

    }       


    /*********************/
    /* link to layer map */

    unsigned sector_wedge[12] = { 0, 0, 0, 0,
                                  1, 1, 1, 1,
                                  2, 2, 2, 2  };




    int layer_link[8][53]; // 4 TIMES 30deg phi sector TIMES 2 links per sector, layer    
    layer_link[0][0] = -1; /* not a layer */           
    layer_link[0][1] = 0;                    
    layer_link[0][2] = -1;                   
    layer_link[0][3] = 8;                    
    layer_link[0][4] = -1;                   
    layer_link[0][5] = 16;                   
    layer_link[0][6] = -1;                   
    layer_link[0][7] = 24;                   
    layer_link[0][8] = -1;                   
    layer_link[0][9] = 32;                   
    layer_link[0][10] = -1;                  
    layer_link[0][11] = 40;                  
    layer_link[0][12] = -1;                  
    layer_link[0][13] = 48;                  
    layer_link[0][14] = -1;                  
    layer_link[0][15] = 56;                  
    layer_link[0][16] = -1;                  
    layer_link[0][17] = 64;                  
    layer_link[0][18] = -1;                  
    layer_link[0][19] = 0;                   
    layer_link[0][20] = -1;                  
    layer_link[0][21] = 8;                   
    layer_link[0][22] = -1;                  
    layer_link[0][23] = 16;                  
    layer_link[0][24] = -1;                  
    layer_link[0][25] = 24;                  
    layer_link[0][26] = -1;                  
    layer_link[0][27] = 32;                  
    layer_link[0][28] = -1;                  
    layer_link[0][29] = 40;                  
    layer_link[0][30] = 48;                  
    layer_link[0][31] = 56;                  
    layer_link[0][32] = 64;                  
    layer_link[0][33] = 0;                   
    layer_link[0][34] = 8;                   
    layer_link[0][35] = 16;                  
    layer_link[0][36] = 24;                  
    layer_link[0][37] = 32;                  
    layer_link[0][38] = 40;                  
    layer_link[0][39] = 48;                  
    layer_link[0][40] = 56;                  
    layer_link[0][41] = 64;                  
    layer_link[0][42] = 0;                   
    layer_link[0][43] = 8;                   
    layer_link[0][44] = 40;                  
    layer_link[0][45] = 40;                  
    layer_link[0][46] = 48;                  
    layer_link[0][47] = 48;                  
    layer_link[0][48] = 56;                  
    layer_link[0][49] = 56;                  
    layer_link[0][50] = 64;                  
    layer_link[0][51] = 64;                  
    layer_link[0][52] = 64;                      
    
    for(int iw=1; iw<8; iw++) 
        for(int il=0; il<53; il++) 
            layer_link[iw][il] = (layer_link[iw-1][il] == -1) ? -1 : (layer_link[iw-1][il]+1);
                

    /* Get Entry */
    detector.getEvent( iEvt );
        
    /********************/
    /* looping over endcaps */
    
    /* endcap loop */
    for(unsigned iendcap=0; iendcap<2; iendcap++) {
        
        HGCsubdet* subdetector = detector.getSubdet(iendcap, 3);
        
        vector<HGCTC> data_tc[3][nLinks]; // 3 120deg wedges times nLinks links
            
        /* sector loop */
        for(unsigned isector=0; isector<nPhiSectors; isector++){
            /* get tcs and c2ds vectors */
            vector<const HGCTC*>   tcs = subdetector->getAllInRegion<HGCTC> ( 0, std::numeric_limits<double>::max(), 
                                                                              minPhis[isector], maxPhis[isector] );
            vector<const HGCC2D*> c2ds = subdetector->getAllInRegion<HGCC2D>( 0, std::numeric_limits<double>::max(), 
                                                                              minPhis[isector], maxPhis[isector] );

            /* TCs LOOP */
            unsigned nTC = tcs.size();
            for( unsigned itc=0; itc<nTC; itc++ ){
                    
                /* get the trigger cell */
                const HGCTC* tc = tcs.at(itc);
                    
                if( layer_link[0][tc->layer()] == -1 )
                    continue;
                    
                int layer = tc->layer(); 
                int wedge = sector_wedge[isector];

                int link0 = layer_link[ 2*isector%8 ][ layer ];
                int link1 = layer_link[ 2*isector%8+1 ][ layer ];
               
                data_tc[ wedge ][ link0 ].push_back(*tc);
                data_tc[ wedge ][ link1 ].push_back(*tc);

            }// end loop tc
           
        }// end sector loop

        /* sorting and split data over 2 links removing evey second odd/even (for odd/even link) data */
        for(unsigned iwedge=0; iwedge<3; iwedge++) {
            for(unsigned ilink=0; ilink<nLinks; ilink++){
                
                // sorting
                std::sort( data_tc[iwedge][ilink].begin(), data_tc[iwedge][ilink].end(),sortByRZ );
                
                // split data
                if( ilink % 2==0 ) data_tc[iwedge][ilink].erase( std::remove_if( data_tc[iwedge][ilink].begin(), data_tc[iwedge][ilink].end(),
                                                                                 isIndexOdd ), data_tc[iwedge][ilink].end());
                else  data_tc[iwedge][ilink].erase( std::remove_if( data_tc[iwedge][ilink].begin(), data_tc[iwedge][ilink].end(),
                                                                    isIndexEven ), data_tc[iwedge][ilink].end());                        
                
            } // end link loop
        } // end wedge loop
        
        
        for(int iw=0; iw<3; iw++){
                
            stringstream oName ;
            oName << outputFileName 
                  << "Endcap" << Form("%d", iendcap) 
                  << "Wedge"  << Form("%d", iw) 
                  << ".mp7";
            cout << oName.str() << endl;
            MP7gen<HGCTC> mp7tc( oName.str().c_str(), data_tc[iw]);
                
        }
            
    }// end endcap loop

    return 0;

}


