
/* C/C++ */
#include <iostream>
#include <cmath>
#include <ctime>
#include <queue>
#include <getopt.h>

/* ROOT */
#include <TRandom3.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TString.h>
#include <TGraph.h>
#include <TH1D.h>

/* myLibs */
#include <URAM.h>
#include <memMan.h>



using namespace std;


struct sFevt{

    uint64_t BX;
    bool tbr;

};


int main(int argc, char **argv) {

    /* standard CMS */
    float LHCfreq = 40000000; // Hz
    float L1frq = 750000; // Hz
    float L1acceptBX = LHCfreq/L1frq;

    /* number of bx to simulate */ 
    uint64_t nBX = 1*pow(10, 5); // 2.5us

    /* out file */
    TString foutName;
    foutName = "foutDAQ.root";

    /* TMUX */
    uint64_t TMUXperiod = 1;
    
    /* Fat Event period and L1 latency */
    uint64_t FevtPeriod_BX = 107;
    double   L1_latency_BX = 400;
    
    /* INPUT params */
    uint64_t nLinksIn = 72;
    
    uint64_t nURAMin = 1;
    uint64_t URAM_wBX_speedIn  = 7;    // w/BX
    uint64_t nBXmaxEvtSizeIn   = 12;   // maximum size of an event in BX counts
    
    /* OUTPUT params */
    uint64_t nLinksOut = 4; //  

    uint64_t nURAMout = 2;
    uint64_t URAM_wBX_speedOut = 12;   // w/BX
    uint64_t nBXmaxEvtSizeOut  = 18;   // time multyplex
    
    /* Tevt */
    bool TevtOn = false;
    
    /* COMMON */
    uint64_t URAM_rBX_speed    = 12;   // w/BX
    uint64_t algoLatency_BX    = 0;    // latency of the algorithm [BX]
    bool ramWpersistent        = false;
    bool ramRpersistent        = false;
    bool verbose               = false;

    /* seeding the random number generator with unix time */
    std::time_t seed = std::time(0);

    /* options */
    int opt;
    const struct option longOptions[] = {
        {"help"          ,  no_argument     ,  0, 'h'},
        {"foutName"      , required_argument,  0, 'f'},
        {"nBX"           , required_argument,  0, 'n'},
        {"FevtPeriod"    , required_argument,  0, 'p'},
        {"L1latency"     , required_argument,  0, 'l'},
        {"tmux"          , required_argument,  0, 'x'},
        {"seed"          , required_argument,  0, 'e'},

        {"nLinksIn"      , required_argument,  0, 'i'},
        {"nURAMin"       , required_argument,  0, 'u'},
        {"URAMspeedIn"   , required_argument,  0, 's'},
        {"maxEvtSizeIn"  , required_argument,  0, 'm'},

        {"nLinksOut"     , required_argument,  0, 'o'},
        {"nURAMout"      , required_argument,  0, 'v'},
        {"URAMspeedOut"  , required_argument,  0, 'd'},
        {"maxEvtSizeOut" , required_argument,  0, 't'},

        {"algoLatency"   , required_argument,  0, 'a'},
        {"readoutSpeed"  , required_argument,  0, 'r'},

        {"WptrPersistent", required_argument,  0, 'y'},
        {"RptrPersistent", required_argument,  0, 'k'},

        {"tevton"        , required_argument,  0, 'j'},
        {"verbose"       , required_argument,  0, 'w'},
        {0,0,0,0}
    };
    
    int optIndex = 0;
    while ( (opt = getopt_long (argc, argv, "hf:n:p:l:i:u:s:m:o:v:d:t:a:r:ykwx:e:j", longOptions, &optIndex) ) != -1 ) {
        
        switch (opt)
        {
        case 'h':
            cout << "Help" << endl;
            cout << "h(--help         ) : \t shows this help." << endl;       
            cout << "f(--foutName     ) <fileName>: \t output file name." << endl;
            cout << "n(--nBX          ) <nBX>: \t number of BX to process (default: 10^5)." << endl;     
            cout << "p(--FevtPeriod   ) <FevtNbx> : \t Fat event period [BX] (default: 107 BX)." << endl;   
            cout << "l(--L1latency    ) <latencyBX>: \t Level1 decision latency [BX] (default: 400BX)" << endl;  
            cout << "i(--nLinksIn     ) <nLinks>: \t INPUT number of links (default: 72)" << endl;     
            cout << "u(--nURAMin      ) <nURAM>: \t INPUT number of URAMs blocks (default: 1, 1 block is 4096 bit deep)" << endl;      
            cout << "s(--URAMspeedIn  ) <URAMspeed>: \t INPUT URAM speed [w/BX] (default: 7w/BX)" << endl;  
            cout << "m(--maxEvtSizeIn ) <evtSize>: \t INPUT event maximum size [BX] (default: 12) " << endl;    
            cout << "o(--nLinksOut    ) <nLinks>: \t OUTPUT number of links (default: 4)" << endl;     
            cout << "v(--nURAMout     ) <URAMspeed>: \t OUTPUT number of URAMs blocks (default: 2, 1 block is 4096 bit deep)" << endl;      
            cout << "d(--URAMspeedOut ) <URAMspeed>: \t OUTPUT URAM speed [w/BX] (default: 12w/BX)" << endl;  
            cout << "t(--maxEvtSizeOut) <evtSize>: \t MUST BE set to max the TMUXperiod, OUTPUT event maximum size [BX] (default: 18BX) " << endl;
            cout << "a(--algoLatency  ) <algLatency>: \t algorithm latency [BX] (default: 2BX) " << endl;
            cout << "r(--readoutSpeed ) <commonRoutSpeed>: \t out link speed [w/BX] (default: 12w/BX)" << endl;
            cout << "y(--WptrPersistent): \t turns on the pensistency f the Wptr." << endl; 
            cout << "k(--RptrPersistent): \t turns on the pensistency f the Rptr." << endl; 
            cout << "w(--verbose): \t turns on the verbosity mode." << endl; 
            cout << "x(--tmux): \t sets the TMUX period (default: 1BX)." << endl; 
            cout << "e(--seed): \t sets the TMUX seed for the random generator (default: unixtime)." << endl; 
            return 0;
            break;
        case 'f':
            foutName = optarg;
            break;
        case 'n':
            nBX = strtoul( optarg, NULL, 0 );
            break;
        case 'p':
            FevtPeriod_BX = strtoul( optarg, NULL, 0 );
            break;
        case 'l':
            L1_latency_BX = strtoul( optarg, NULL, 0 );
            break;
        case 'i':
            nLinksIn = strtoul( optarg, NULL, 0 );
            break;
        case 'u':
            nURAMin = strtoul( optarg, NULL, 0 );
            break;
        case 's':
            URAM_wBX_speedIn = strtoul( optarg, NULL, 0 );
            break;
        case 'm':
            nBXmaxEvtSizeIn = strtoul( optarg, NULL, 0 );
            break;
        case 'o':
            nLinksOut = strtoul( optarg, NULL, 0 );
            break;
        case 'v':
            nURAMout = strtoul( optarg, NULL, 0 );
            break;
        case 'd':
            URAM_wBX_speedOut = strtoul( optarg, NULL, 0 );
            break;
        case 't':
            nBXmaxEvtSizeOut = strtoul( optarg, NULL, 0 );
            break;
        case 'a':
            algoLatency_BX = strtoul( optarg, NULL, 0 );
            break;
        case 'r':
            URAM_rBX_speed = strtoul( optarg, NULL, 0 );
            break;
        case 'y':
            ramWpersistent = true;
            break;
        case 'k':
            ramRpersistent = true;
            break;
        case 'w':
            verbose = true;
            break;
        case 'x':
            TMUXperiod = strtoul( optarg, NULL, 0 );
            break;
        case 'e':
            seed = strtoul( optarg, NULL, 0 );
            break;
        case 'j':
            TevtOn = true;
            break;
        default:
            return 0;
        }
        
    }

    
    
    /* open out file */
    TFile fout(foutName, "RECREATE");

    /***********************/
    /* PRINT CONFIGURATION */
    cout << endl
         << " ***** persistency ******************* " << endl; 

    cout << " >>> out file name      " <<  foutName          << endl;
    
    cout << endl
         << " ***** LHC/CMS params **************** " << endl; 
     
    cout << " >>> nBX                 " << nBX                << endl
         << " >>> FevtPeriod_BX [BX]  " << FevtPeriod_BX      << endl
         << " >>> LHCfreq [Hz]        " << LHCfreq            << endl
         << " >>> L1frq   [Hz]        " << L1frq              << endl
         << " >>> L1acceptPeriod [BX] " << L1acceptBX         << endl
         << " >>> L1Latency [BX]      " << L1_latency_BX      << endl;
    
    cout << endl
         << " ***** INPUT buf params ************** "         << endl; 
    
    cout << " >>> nLinks              " << nLinksIn       << endl
         << " >>> nURAMs              " << nURAMin    << endl
         << " >>> URAM_w_speed [w/BX] " << URAM_wBX_speedIn   << endl
         << " >>> nBXmaxEvtSize[w/BX] " << nBXmaxEvtSizeIn    << endl;
    
    cout << endl
         << " ***** OUTPUT buf params ************* "        << endl; 

    cout << " >>> nLinks              " << nLinksOut     << endl
         << " >>> nURAMs              " << nURAMout  << endl
         << " >>> URAM_w_speed [w/BX] " << URAM_wBX_speedOut << endl
         << " >>> nBXmaxEvtSize[w/BX] " << nBXmaxEvtSizeOut  << endl;
        
    cout << endl
         << " ***** COMMON params ***************** "        << endl; 

    cout << " >>> URAM_r_speed [w/BX] " << URAM_rBX_speed    << endl
         << " >>> algoLatency_BX      " << algoLatency_BX    << endl;
    cout << " >>> rnd gen seed        " << seed              << endl;
    cout << " >>> TMUXperiod [BX]     " << TMUXperiod        << endl;
    
    
    /* first l1 accept */
    TRandom3 r3(seed);
    uint64_t nextL1acceptBX = 0 + r3.Poisson( L1acceptBX );
    queue<uint64_t> previousL1acceptBX;
    queue<uint64_t> L1arrivalBX; // BX of when L1 arrives
    queue<uint64_t> L1BX;        // BX realtive to L1 accept arrived 

    L1arrivalBX.push( nextL1acceptBX+L1_latency_BX );
    L1BX.push( nextL1acceptBX );
    
    /* managers */
    memMan managerIn( nLinksIn, nURAMin,
                      URAM_wBX_speedIn, URAM_rBX_speed,
                      URAM_wBX_speedIn*nBXmaxEvtSizeIn,
                      L1_latency_BX,
                      FevtPeriod_BX,
                      TMUXperiod,
                      0,
                      false,
                      ramWpersistent, ramRpersistent,
                      verbose
        );
    memMan managerOut(nLinksOut, nURAMout,
                      URAM_wBX_speedOut, URAM_rBX_speed,
                      URAM_wBX_speedOut*nBXmaxEvtSizeOut,
                      L1_latency_BX,
                      FevtPeriod_BX,
                      TMUXperiod,
                      algoLatency_BX,
                      TevtOn,
                      ramWpersistent, ramRpersistent,
                      verbose
        );

    cout << endl
         << " ***** MANAGER IN ******************** "        << endl;
    managerIn.print();
    

    cout << endl
         << " ***** MANAGER OUT ******************* "        << endl;
    managerOut.print();
    
    
    /* pensistency */
    TGraph gAvailableSlotsIn, gAvailableSlotsOut;
    TGraph gSlotsAvailabilityIn, gSlotsAvailabilityOut;
    
    TH1D *hAvailableSlotsIn = new TH1D( "hAvailableSlotsIn",
                                        "hAvailableSlotsIn",
                                        100, 0, 100 );

    TH1D *hAvailableSlotsOut = new TH1D( "hAvailableSlotsOut",
                                         "hAvailableSlotsOut",
                                         100, 0, 100 );

    /********/
    /* LOOP */
    if( verbose )
        cout << "ibx"      << "\t"
             << "L1arr"   << "\t"
             << "L1BX"   << "\t"
             << "Fevt?"   << "\t"
            
             << "sInCur"   << "\t"
             << "sInAv"   << "\t"
             << "sInTBR"   << "\t"
             << "sInWr"   << "\t"
             << "sInRd"   << "\t"

             << "sOutCur"   << "\t"
             << "sOutAv"   << "\t"
             << "sOutTBR"   << "\t"
             << "sOutWr"   << "\t"
             << "sOutRd"   << "\t"
            
            
//<< "TMUXprd"  << "\t"
             //<< "FevtPrd"  << "\t"
             //<< "TMUXprd"  << "\t"
             //
             //<< "L1BX"     << "\t" 
             //<< "bxToRdI"  << "\t"
             //<< "bxToRdO"  << "\t"
             //<< "evtTypS"  << "\t"
             //<< "evtType"  << "\t"
             //<< "rdLink"   << "\t"
             //<< "ramWin"   << "\t" 
             //<< "1AddIn"   << "\t"
             //<< "rdAddIn"  << "\t"
             //<< "wrBXin"   << "\t"
             //<< "nBXwrIn"  << "\t"
             //<< "ramWouX"  << "\t" 
             //<< "ramWout"  << "\t" 
             //<< "wrBXout"  << "\t"
             //<< "nBXwrOu"  << "\t"
             << endl;
    
    for(uint64_t ibx=0; ibx<nBX; ibx++) {

        /***********************************/
        /* some messages and verbose debug */
        if( ibx%(nBX/100) == 0 )
            cout << " >>> " << ibx/(nBX/100) << "% bx processed" << endl;

        /************************/
        /* handling the next L1 */
        if( ibx == nextL1acceptBX ) {

            nextL1acceptBX = ibx + r3.Poisson( L1acceptBX ); // calculate next L1acc
            L1arrivalBX.push( nextL1acceptBX+L1_latency_BX );
            L1BX.push( nextL1acceptBX );

        }

        /***********************/
        /* update the memories */

        bool isL1acc = (L1arrivalBX.front()==ibx) ? true : false;
        
        managerOut.update( ibx, isL1acc, L1BX.front(), managerIn.isReading() );
        managerIn.update( ibx, isL1acc, L1BX.front(), managerOut.isReading() );
        
        /* next L1 to process */
        if( isL1acc ) {
            L1arrivalBX.pop();
            L1BX.pop();
        }

        hAvailableSlotsIn->Fill( managerIn.getNavailableSlots() );
        if( ramWpersistent ) {
            gAvailableSlotsIn.SetPoint( gAvailableSlotsIn.GetN(), ibx, managerIn.getNavailableSlots() );
            for(int islot=0; islot<managerIn.getNslots(); islot++ ) {
                if( !managerIn.isSlotAvailable(islot) )
                    gSlotsAvailabilityIn.SetPoint( gSlotsAvailabilityIn.GetN(), ibx, islot);
            }
        }
        hAvailableSlotsOut->Fill( managerOut.getNavailableSlots() );
        if( ramRpersistent ) {
            gAvailableSlotsOut.SetPoint( gAvailableSlotsOut.GetN(), ibx, managerOut.getNavailableSlots() );
            for(int islot=0; islot<managerOut.getNslots(); islot++ ) {
                if( !managerOut.isSlotAvailable(islot) )
                    gSlotsAvailabilityOut.SetPoint( gSlotsAvailabilityOut.GetN(), ibx, islot);
            }
        }
        
        /*********/
        /* infos */
        if( verbose ) {
                cout << ibx                             << "\t"
                     << L1arrivalBX.front()             << "\t"
                     << L1BX.front()                    << "\t"
                     << L1BX.front()%FevtPeriod_BX      << "\t"
                    
                     << managerIn.getCurrentSlot()      << "\t"
                     << managerIn.getNavailableSlots()  << "\t"
                     << managerIn.getEvtToread()        << "\t"
                     << managerIn.isWriting()           << "\t"
                     << managerIn.isReading()           << "\t"

                     << managerOut.getCurrentSlot()     << "\t"
                     << managerOut.getNavailableSlots() << "\t"
                     << managerOut.getEvtToread()       << "\t"
                     << managerOut.isWriting()          << "\t"
                     << managerOut.isReading()          << "\t"
                    
                    
                     //<< (int)ibx/TMUXperiod     << "\t"
                     //<< ibx%FevtPeriod_BX       << "\t"
                     //<< ibx%TMUXperiod          << "\t"
                     //<< nextL1acceptBX          << "\t"
                     //<< L1arrivalBX.front()     << "\t" 
                     //<< bxToReadInBuf.front()   << "\t"
                     //<< bxToReadOutBuf.front()  << "\t"
                     //<< evtTypeQueue.size()     << "\t"
                     //<< evtTypeQueue.front()    << "\t"
                     //<< readingLink             << "\t"
                     //<< URAMinBuf[0]->getWptr() << "\t" 
                     //<< firstAddrIn             << "\t"
                     //<< readingAddrIn           << "\t"
                     //<< writingBXidIn           << "\t"
                     //<< nBXwrittenInBuf         << "\t"
                     //<< URAMoutBuf[0]->read( URAMoutBuf[0]->getWptr(), ibx, false ) << "\t" 
                     //<< URAMoutBuf[0]->getWptr()<< "\t" 
                     //<< writingBXidOut          << "\t"
                     //<< nBXwrittenOutBuf        << "\t"
                     //<< ovwr                    << "\t"
                     //<< dist                    << "\t"
                     << endl;
        }

    } // end BX loop


    cout << "Writing to the output file: " << foutName << endl;

    if( ramWpersistent ) {
        managerIn.write("in");
        gAvailableSlotsIn.Write("in_availableSlots");
        gSlotsAvailabilityIn.Write("in_slotsAvailability");
    }
    hAvailableSlotsIn->Write("in_availableSlotsHisto");
    
    if( ramRpersistent ) {
        managerOut.write("out");
        gAvailableSlotsOut.Write("out_availableSlots");
        gSlotsAvailabilityOut.Write("out_slotsAvailability");
        
    }
    hAvailableSlotsOut->Write("out_availableSlotsHisto");

    fout.Close();
    
    cout << " done." << endl << endl;

    return 0;

}
