
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


using namespace std;

class URAM {

public:

    URAM() {
        _URAMdepth=4096;
        this->init();
    }

    URAM( unsigned depth ) {
        _URAMdepth=depth;
        this->init();
        
    }

    void clear() {

        for (unsigned i=0; i<_URAMdepth; i++)
            _ram[i] = 0;

    }
    
    unsigned getWptr() { return _wptr; }
    //unsigned getRptr() { return _rptr; }

    void write( unsigned w, int bx, bool addToGraph=true ) {

        _ram[_wptr] = w;

        if( addToGraph ) _gWptr->SetPoint( _gWptr->GetN(), bx, _wptr );

        if( _wptr == (_URAMdepth-1) ) 
            _wptr = 0;
        else 
            _wptr++;
        
    }
    
    unsigned read( unsigned ptr, int bx, bool addToGraph=true ) {
        
        if(ptr>=_URAMdepth){
            return 0; 
        }
        else {

            if( addToGraph ) _gRptr->SetPoint( _gRptr->GetN(), bx, ptr );
            return _ram[ptr];
        }

    }

    void dump() {

        cout << "RAM content" << endl;

        for (unsigned i=0; i<_URAMdepth; i++)
            cout << i << " " << _ram[i] << endl;

    }

    TGraph* getWptrGraph() { return _gWptr; }
    TGraph* getRptrGraph() { return _gRptr; }
    
private:
    void init() {
        _wptr = 0;
        _rptr = 0;
        _gRptr = new TGraph();
        _gWptr = new TGraph();
        _gWptrBX = new TGraph();
        _ram = new uint64_t[_URAMdepth];
        this->clear();
    }

    unsigned _URAMdepth;
    
    uint64_t *_ram;
    uint64_t _wptr, _rptr;
    //float _clkFreq;
    TGraph *_gWptr;
    TGraph *_gRptr;

    TGraph *_gWptrBX;

};


struct sFevt{

    uint64_t BX;
    bool tbr;

};


int main(int argc, char **argv) {

    /* standard CMS */
    float LHCfreq = 40000000; // Hz
    float L1frq = 750000; // Hz
    float L1acceptBX = LHCfreq/L1frq;

    // number of bx to simulate 
    uint64_t nBX = 1*pow(10, 5); // 2.5us
//    uint64_t nBX = 2.4*pow(10, 9); // number of bx to simulate 1m
//    uint64_t nBX = 4.8*pow(10, 9); // number of bx to simulate 2m

//    uint64_t nBX = 24*pow(10, 9); // number of bx to simulate 10m

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
    uint64_t algoLatency_BX    = 2;    // latency of the algorithm [BX]
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

    /* URAM depth */
    const unsigned URAMdepth = pow(2, 12);
    
    unsigned FevtURAMdepthIn  = nURAMin  * URAMdepth;
    unsigned FevtURAMdepthOut = nURAMout * URAMdepth; 
    
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
         << " >>> FevtURAMdepth [w]   " << FevtURAMdepthIn    << endl
         << " >>> URAM_w_speed [w/BX] " << URAM_wBX_speedIn   << endl
         << " >>> nBXmaxEvtSize       " << nBXmaxEvtSizeIn    << endl;
    
    cout << endl
         << " ***** OUTPUT buf params ************* "        << endl; 

    cout << " >>> nLinks              " << nLinksOut     << endl
         << " >>> FevtURAMdepth [w]   " << FevtURAMdepthOut  << endl
         << " >>> URAM_w_speed [w/BX] " << URAM_wBX_speedOut << endl
         << " >>> nBXmaxEvtSize       " << nBXmaxEvtSizeOut  << endl;
        
    cout << endl
         << " ***** COMMON params ***************** "        << endl; 

    cout << " >>> URAM_r_speed [w/BX] " << URAM_rBX_speed    << endl
         << " >>> algoLatency_BX    "   << algoLatency_BX    << endl;
    cout << " >>> rnd gen seed "        << seed              << endl;
    cout << " >>> TMUXperiod [BX]"      << TMUXperiod        << endl;
    
    cout << endl
         << " ***** PROCESSING ******************** "        << endl;
    
    /* first l1 accept */
    TRandom3 r3(seed);
    uint64_t nextL1acceptBX = 0 + r3.Poisson( L1acceptBX );
    queue<uint64_t> previousL1acceptBX;

//    uint64_t nFevtToAcquire = 0;
    
//    uint64_t transmittingFevt = 0;
//    uint64_t MAXqueueForFevtTransmission = 0;
    
    /* memories (URAM) */
    URAM* URAMinBuf[nLinksIn];
    for( uint64_t i=0; i<nLinksIn; i++ )
        URAMinBuf[i] = new URAM(FevtURAMdepthIn);

    URAM* URAMoutBuf[nLinksOut];
    for( uint64_t i=0; i<nLinksOut; i++ )
        URAMoutBuf[i] = new URAM(FevtURAMdepthOut); 
 
    /* FIFOs */
    queue<uint64_t> bxToReadInBuf;   // list of BXid to be read
    queue<uint64_t> bxToReadOutBuf;   // list of BXid to be read
    queue<uint64_t> evtTypeQueue;   // list of BXid to be read
    queue<uint64_t> L1arrivalBX; // when I need to start reading the memnory? [bx]

    /* in buff */
    map<uint64_t, uint64_t> firstAddrInBuf; // maps the bxid with the address 
    map<uint64_t, uint64_t> lastAddrInBuf; // maps the bxid with the address 
    
    uint64_t firstAddrIn = 0;
    uint64_t lastAddrIn = 0;
    uint64_t readingAddrIn = 0;
    uint64_t readingLink = 0;

    /* out buff */
    map<uint64_t, uint64_t> firstAddrOutBuf; // maps the bxid with the address 
    map<uint64_t, uint64_t> lastAddrOutBuf; // maps the bxid with the address 

    uint64_t firstAddrOut   = 0;
    uint64_t lastAddrOut   = 0;
    uint64_t readingAddrOut = 0;

    /* control */
    bool     writingInBuf      = false;
    uint64_t writingBXidIn     = 0;
    uint64_t nBXwrittenInBuf   = 0;

    bool     writingOutBuf     = false;
    uint64_t writingBXidOut    = 0;
    uint64_t nBXwrittenOutBuf = 0;

    //uint64_t readingBXid       = 0;
    bool     readingBuf       = false;
    bool     readingBufIn     = false;
    bool     readingBufOut    = false;

//    bool     readingTevt       = false;
  
    /* pensistency */
    TGraph distWptrRptrIn, distWptrRptrOut;
    
    TH1D **hDistWptrRptr = new TH1D*[nLinksIn];
    for(unsigned ilink=0; ilink<nLinksIn; ilink++)
        hDistWptrRptr[ilink] = new TH1D( Form("hDistWptrRptrIn_%d",ilink),
                                           Form("hDistWptrRptrIn_%d",ilink),
                                           600, 0, 6000 );

    TH1D **hDistWptrRptrOut = new TH1D*[nLinksOut];
    for(unsigned ilink=0; ilink<nLinksOut; ilink++)
        hDistWptrRptrOut[ilink] = new TH1D( Form("hDistWptrRptrOut_%d",ilink),
                                            Form("hDistWptrRptrOut_%d",ilink),
                                            600, 0, 6000 );

    /********/
    /* LOOP */
    if( verbose )
        cout << "ibx"      << "\t"
             << "TMUXprd"  << "\t"
             << "FevtPrd"  << "\t"
             << "TMUXprd"  << "\t"
             << "nextL1"   << "\t"
             << "L1BX"     << "\t" 
             << "bxToRdI"  << "\t"
             << "bxToRdO"  << "\t"
             << "evtTypS"  << "\t"
             << "evtType"  << "\t"
             << "rdLink"   << "\t"
             << "ramWin"   << "\t" 
             << "1AddIn"   << "\t"
             << "rdAddIn"  << "\t"
             << "wrBXin"   << "\t"
             << "nBXwrIn"  << "\t"
             << "ramWouX"  << "\t" 
             << "ramWout"  << "\t" 
             << "wrBXout"  << "\t"
             << "nBXwrOu"  << "\t"
             << endl;
    
    for(uint64_t ibx=0; ibx<nBX; ibx++) {

        /***********************************/
        /* some messages and verbose debug */
        if( ibx%(nBX/100) == 0 )
            cout << " >>> " << ibx/(nBX/100) << "% bx processed" << endl;

        unsigned ovwr = URAMoutBuf[0]->read( URAMoutBuf[0]->getWptr(), ibx, false );
        unsigned dist = (bxToReadOutBuf.size()>0 && ovwr>0) ? (bxToReadOutBuf.front() - ovwr) : 1;
        
        /* infos */
        if( verbose ) {
                cout << ibx                     << "\t"
                     << (int)ibx/TMUXperiod     << "\t"
                     << ibx%FevtPeriod_BX       << "\t"
                     << ibx%TMUXperiod          << "\t"
                     << nextL1acceptBX          << "\t"
                     << L1arrivalBX.front()     << "\t" 
                     << bxToReadInBuf.front()   << "\t"
                     << bxToReadOutBuf.front()  << "\t"
                     << evtTypeQueue.size()     << "\t"
                     << evtTypeQueue.front()    << "\t"
                     << readingLink             << "\t"
                     << URAMinBuf[0]->getWptr() << "\t" 
                     << firstAddrIn             << "\t"
                     << readingAddrIn           << "\t"
                     << writingBXidIn           << "\t"
                     << nBXwrittenInBuf         << "\t"
                     << URAMoutBuf[0]->read( URAMoutBuf[0]->getWptr(), ibx, false ) << "\t" 
                     << URAMoutBuf[0]->getWptr()<< "\t" 
                     << writingBXidOut          << "\t"
                     << nBXwrittenOutBuf        << "\t"
                     << ovwr                    << "\t"
                     << dist                    << "\t"
                     << endl;
        }
        
        /*************/
        /* some vars */
        bool isFevt = (ibx%FevtPeriod_BX == 0) && (ibx%TMUXperiod == 0);

        
        /******************************************************************************/
        /* calculate next L1accept BX id and check if current BX is a Fevt candidate  */
        if( ibx == nextL1acceptBX ) { // do I need to store the L1?

            previousL1acceptBX.push( nextL1acceptBX );
            nextL1acceptBX = ibx + r3.Poisson( L1acceptBX ); // calculate next L1acc

            if( ibx > algoLatency_BX ) {
            
                unsigned evtType = 10; // 10 invalid ; 0 Tevt ; 1 Fevt 

                evtType = TevtOn ? 0 : evtType;
                evtType = isFevt ? 1 : evtType;
                
                if( ( isFevt || (TevtOn && ibx%TMUXperiod == 0 ) ) && (ibx>0) ) {

                    bxToReadOutBuf.push( ibx );
                    if( evtType == 1 )
                        bxToReadInBuf.push( ibx );
                        
                    L1arrivalBX.push( ibx+L1_latency_BX );
                    evtTypeQueue.push( evtType );
                }
            }
            
        }

        
        /*************************/
        /* writing input buffers */
        if( ( isFevt || writingInBuf ) && ibx!=0 ) { // do I need to store the Event?

            writingInBuf = true;
            
            if( isFevt ) {
                writingBXidIn = ibx;
                firstAddrInBuf[writingBXidIn] = URAMinBuf[0]->getWptr();
            }
            
            /* looping over links */
            for( uint64_t ilink=0; ilink<nLinksIn; ilink++ ) {
                for( uint64_t iw=0; iw<URAM_wBX_speedIn ; iw++ ) {

                    unsigned ovwr = URAMinBuf[ilink]->read( URAMinBuf[ilink]->getWptr(), ibx, false );
                    unsigned dist = (bxToReadInBuf.size()>0 && ovwr>0) ? (bxToReadInBuf.front() - ovwr)/TMUXperiod : 1;
                    
                    if( dist>0 ) {
                        URAMinBuf[ilink]->write( writingBXidIn, ibx, ramWpersistent );
                    }

                    if( bxToReadInBuf.size()>0 && ovwr>0) {
                        hDistWptrRptr[ilink]->Fill(dist);
                        if( ramWpersistent || ramRpersistent )
                            distWptrRptrIn.SetPoint( distWptrRptrIn.GetN(), ibx, dist );
                    }
                    
                }
            }

            if( ++nBXwrittenInBuf == nBXmaxEvtSizeIn ) {
                nBXwrittenInBuf = 0;
                writingInBuf = false;
                lastAddrInBuf[writingBXidIn] = URAMinBuf[0]->getWptr();
                writingBXidIn = 0;
            }
            
        }
        
        /**************************/
        /* writing output buffers */
        uint64_t ibx_delay   = ibx-algoLatency_BX;
        bool     isFevtDelay = (ibx_delay%FevtPeriod_BX == 0) && (ibx_delay%TMUXperiod == 0); 
        if( ( ibx > algoLatency_BX ) &&
            ( ( (!TevtOn && isFevtDelay) || ( TevtOn && ibx_delay%TMUXperiod == 0 ) ) || writingOutBuf ) ) { // do I need to store the Event?
            
            writingOutBuf = true;
            
            if( (!TevtOn && isFevtDelay) || ( TevtOn && ibx_delay%TMUXperiod == 0 ) ) {

                writingBXidOut = ibx_delay;
                firstAddrOutBuf[writingBXidOut] = URAMoutBuf[0]->getWptr();

            }
            
            /* looping over links */
            for( uint64_t ilink=0; ilink<nLinksOut; ilink++ ) {
                for( uint64_t iw=0; iw<URAM_wBX_speedOut ; iw++ ) {
                    unsigned ovwr = URAMoutBuf[ilink]->read( URAMoutBuf[ilink]->getWptr(), ibx, false );
                    unsigned dist = (bxToReadOutBuf.size()>0 && ovwr>0) ? (bxToReadOutBuf.front() - ovwr) : 1;
                    
                    if( dist>0 ) {
                        URAMoutBuf[ilink]->write( writingBXidOut, ibx_delay, ramWpersistent );
                    }
                    
                    if( bxToReadOutBuf.size()>0 && ovwr>0) {

                        hDistWptrRptrOut[ilink]->Fill(dist);

                        if( ramWpersistent || ramRpersistent )
                            distWptrRptrOut.SetPoint( distWptrRptrOut.GetN(), ibx, dist );       

                    }
                }
            }

            if( ++nBXwrittenOutBuf == nBXmaxEvtSizeOut ) {
                nBXwrittenOutBuf = 0;
                writingOutBuf = false;
                lastAddrOutBuf[writingBXidOut] = URAMoutBuf[0]->getWptr();
                writingBXidOut = 0;
            }
            
        }

        
        /****************************/
        /* reading events if needed */
        if( ibx >= L1arrivalBX.front() &&
            ( ( evtTypeQueue.size()>0 ) || readingBuf ) ) {
            
            if( !readingBuf ) {

                if( evtTypeQueue.front() == 1 ) {
                    firstAddrIn    = firstAddrInBuf[bxToReadInBuf.front()];
                    lastAddrIn     = lastAddrInBuf[bxToReadInBuf.front()];
                    readingAddrIn  = firstAddrIn;
                    readingBufIn   = false;
                }
                
                firstAddrOut   = firstAddrOutBuf[bxToReadOutBuf.front()];
                lastAddrOut    = lastAddrOutBuf[bxToReadOutBuf.front()];
                readingAddrOut = firstAddrOut;
                readingBufOut  = true; // reading out buff first

            }
            
            readingBuf = true;
        
            for( uint64_t ir=0; ir<URAM_rBX_speed ; ir++ ) {
                
                if( readingBufOut ) { /* Out buf */

                    if( verbose )
                        
                    cout << "readingAddrOut   " << readingAddrOut 
                         << "\tfirstAddrOut   " << firstAddrOut
                         << "\tlastAddrOut    " << lastAddrOut
                         << "\treadingLinkOut " << readingLink;

                    bool vetoIncrement=false;

                    if( readingAddrOut == lastAddrOut ) { // if I am at the end of the data change link or end

                        if( verbose ) {
                            unsigned data = URAMoutBuf[readingLink]->read( readingAddrOut, ibx, false );
                            
                            cout << " \tdata " << data
                                 << " \tNOT READ"
                                 << endl;
                        }
                        
                        if( readingLink<(nLinksOut-1) ) {

                            readingLink++; // update the link 
                            ir--;
                            readingAddrOut = firstAddrOut;
                            
                            vetoIncrement = true;
                            
                        } 
                        else {
                            
                            readingLink = 0;
                            if( evtTypeQueue.front() == 0 ) {
                                readingBufOut = false;
                                readingBufIn  = false;
                                readingBuf    = false;
                                evtTypeQueue.pop();
                                bxToReadOutBuf.pop();
                                L1arrivalBX.pop(); // go to the next L1 accept
                                break;
                            }
                            else if( evtTypeQueue.front() == 1 ) {
                                readingBufOut = false;
                                readingBufIn  = true;
                                bxToReadOutBuf.pop();
                                ir--;
                            }
                            
                         }
                         
                    }
                    else{
                        
                        /* read the ram */
                        unsigned data = URAMoutBuf[readingLink]->read( readingAddrOut, ibx, ramRpersistent );
                        if( verbose )
                            cout << " \tdata " << data << endl;
                        
                    }
                    
                    /* Rptr handling */
                    if( !vetoIncrement ) {
                        if( readingAddrOut == FevtURAMdepthOut-1 )
                            readingAddrOut = 0;
                        else 
                            readingAddrOut++;
                    }

                }
                else if( readingBufIn ) { /* in buf */

                    if( verbose )
                        cout << "readingAddrIn   " << readingAddrIn 
                             << "\tfirstAddrIn   " << firstAddrIn
                             << "\tlastAddrIn    " << lastAddrIn 
                             << "\treadingLinkIn " << readingLink
                             << endl;
                    
                    bool vetoIncrement=false;
                    
                    if( readingAddrIn == lastAddrIn ) {
                        
                        if( readingLink<(nLinksIn-1) ) {

                            readingLink++;
                            ir--;
                            readingAddrIn = firstAddrIn;

                            vetoIncrement = true;

                        }
                        else {
                            
                            readingLink=0;
                            
                            readingBufIn = false;
                            readingBuf   = false;
                            evtTypeQueue.pop();
                            bxToReadInBuf.pop();    // go to the next evt to read
                            L1arrivalBX.pop(); // go to the next L1 accept
                            break;
//                            }
                        }
                        
                    }
                    else{
                        
                        /* read the ram */
                        URAMinBuf[readingLink]->read( readingAddrIn, ibx, ramRpersistent );                   

                    }

                    /* Rptr handling */
                    if( !vetoIncrement ) {
                        if( readingAddrIn == FevtURAMdepthIn-1 )
                            readingAddrIn = 0;
                        else 
                            readingAddrIn++;
                    }
            }
            
        }
            
        } // lend bx reading 
        
       
        
    } // end BX loop
    
    
//    URAMoutBuf[0]->dump();
       
    cout << " writing output file (" << foutName << ") " << endl;

    if( ramWpersistent || ramRpersistent ) {
        for( unsigned ilink=0; ilink<nLinksIn; ilink++ ) {
            
            TGraph *wGprah = URAMinBuf[ilink]->getWptrGraph();
            wGprah->Write( Form("Wptr_%d", ilink) );
            TGraph *rGprah = URAMinBuf[ilink]->getRptrGraph();
            rGprah->Write( Form("Rptr_%d", ilink) );
            distWptrRptrIn.Write( Form("dist_%d", ilink) );
        
        }
        
        for( unsigned ilink=0; ilink<nLinksOut; ilink++ ) {
            
            TGraph *wGprah = URAMoutBuf[ilink]->getWptrGraph();
            wGprah->Write( Form("Wptr_out_%d", ilink) );
            TGraph *rGprah = URAMoutBuf[ilink]->getRptrGraph();
            rGprah->Write( Form("Rptr_out_%d", ilink) );
            distWptrRptrOut.Write( Form("dist_out_%d", ilink) );
            
        }
    }

    for( unsigned ilink=0; ilink<nLinksIn; ilink++ ) 
        hDistWptrRptr[ilink]->Write();

    for( unsigned ilink=0; ilink<nLinksOut; ilink++ ) 
        hDistWptrRptrOut[ilink]->Write();

    
    fout.Close();
    
    cout << " done." << endl << endl;

    return 0;

}
