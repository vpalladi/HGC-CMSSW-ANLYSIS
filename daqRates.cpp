

#include <iostream>
#include <cmath>
#include <ctime>
#include <queue>


#include <TRandom3.h>
#include <TApplication.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TString.h>
#include <TGraph.h>
#include <TH1D.h>


using namespace std;

class uRAM {

public:

    uRAM() {
        this->init();
    }

    uRAM( float clockFreq ) {
        this->init();        
        _clkFreq=clockFreq;
    }

    unsigned getWptr() { return _wptr; }
    //unsigned getRptr() { return _rptr; }

    void write( unsigned w, int bx, bool addToGraph=true ) {

        _ram[_wptr] = w;

        if( addToGraph ) _gWptr->SetPoint( _gWptr->GetN(), bx, _wptr );

        if( _wptr == (4000-1) ) 
            _wptr = 0;
        else 
            _wptr++;
        
    }
    
    unsigned read( unsigned ptr, int bx, bool addToGraph=true ) {
        
        if(ptr>=4000){
            return 0; 
        }
        else {
//            cout << _gRptr->GetN() << " " << bx << " " << ptr << endl;
            if( addToGraph ) _gRptr->SetPoint( _gRptr->GetN(), bx, ptr );
            return _ram[ptr];
        }

    }

    void dump() {

        cout << "RAM content" << endl;

        for (int i=0; i<4000; i++)
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
    }
    
    unsigned _ram[4000];
    unsigned _wptr, _rptr;
    float _clkFreq;
    TGraph *_gWptr;
    TGraph *_gRptr;

};


struct sFEvt{

    unsigned BX;
    bool tbr;

};


int main(int argc, char **argv) {

    //unsigned long int nBX = pow(10, 6); // number of bx to simulate 1h
    unsigned long int nBX = 2.4*pow(10, 9); // number of bx to simulate 1m
//unsigned long int nBX = 144*pow(10, 9); // number of bx to simulate 1h
    //unsigned long int nBX = 3456*pow(10, 9); // number of bx to simulate 1d
    TString foutName;
    if(argc==0)
        foutName="foutDAQ.root";
    else
        foutName=argv[1];
    TFile fout(foutName, "RECREATE");

    TApplication app("app", &argc, argv);

    /* input link */
    //float 12GbNominalSpeed_Gbs = 10; // Gb/s or b/ns
    //float 12GbWordWidth = 32; // b
    //float 12GbNominalSpeed_ws = 12GbWordWidth/12GbNominalSpeed_Gbs; // w/ns or GHz

    unsigned FEvtPeriod_BX = 107;
    float LHCfreq = 40000000; // Hz
    float L1frq = 750000; // Hz
    float L1acceptBX = LHCfreq/L1frq;
    double latency_BX = 400;

//    unsigned nBXforFEvtTransmission = 1512; // BX

    unsigned nLinks = 72;
    unsigned link10Gbs_speed_wBX = 7;
    unsigned link10Gbs_speed_rBX = 7;
    unsigned nBXmaxEvtSize = 12; // maximum size of an event in BX counts
    
    cout << " >>> nBX                " << nBX << endl
         << " >>> FEvtPeriod_BX [BX] " << FEvtPeriod_BX << endl
         << " >>> LHCfreq [Hz]       " << LHCfreq       << endl
         << " >>> L1frq   [Hz]       " << L1frq         << endl
         << " >>> L1acceptFreq [BX]  " << L1acceptBX    << endl;

    std::time_t t = std::time(0);
    TRandom3 r3(t);
    
    unsigned nextL1acceptBX = 0 + r3.Poisson( L1acceptBX );

//    unsigned nFEvtToAcquire = 0;
    
//    unsigned transmittingFEvt = 0;
    unsigned MAXqueueForFEvtTransmission = 0;

    vector<unsigned> L1as; // bx id of the fevt to be read
    
    /* memories (uRAM) */
    uRAM* ram[nLinks];
    for( unsigned i=0; i<nLinks; i++ )
        ram[i] = new uRAM(240); // MHz
    
    /* FIFOs */
    queue<unsigned> bxFEvtToRead; // list of BXid to be read
    queue<unsigned> bxIdL1aArrival; // when I need to start reading the memnory? [bx]

    bool writingFEvt = false;
    unsigned writingBXid = 0;
    unsigned nBXwrittenFEvt = 0;

    bool readingFEvt = false;
    unsigned readingBXid = 0;

    TGraph distWptrRptr;

    map<unsigned, unsigned> firstWordAddr; // address map 
    unsigned firstAddr = 0;
    unsigned readingAddr = 0;
    unsigned readingLink = 0;
 
    for(unsigned ibx=0; ibx<nBX; ibx++) {
        
        if( ibx%(nBX/100) == 0 )
            cout << " >>> " << ibx/(nBX/100) << "% bx processed" << endl;

//cout << ibx << "\t"
        //     << nextL1acceptBX << "\t"
        //     << ram[0]->getWptr() << "\t" 
        //     << firstAddr << "\t"
        //     << readingLink << "\t"
        //     << readingAddr << "\t"
        //     << readingBXid << "\t"
        //     << bxIdL1aArrival.front() << endl;

        /* writing FEVT */
        if( ((ibx%FEvtPeriod_BX == 0) | writingFEvt) && ibx!=0 ) { // do I need to store the Fat Event?
            
            if( ibx%FEvtPeriod_BX == 0 )
                writingBXid = ibx;

            writingFEvt = true;

            if( ibx%FEvtPeriod_BX == 0 )
                firstWordAddr[ibx] = ram[0]->getWptr();

            /* input 12Gb/s link 12w/BX; 1word(w)=32bit */
            for( unsigned ilink=0; ilink<nLinks; ilink++ ) {
                // bool written = false;
                for( unsigned iw=0; iw<link10Gbs_speed_wBX ; iw++ ) {
                    
                    //  unsigned overwritingBX = ram[ilink]->read( ram[ilink]->getWptr(), ibx, false );
                    
                    ram[ilink]->write( writingBXid, ibx, false );
                    
                    //if( bxFEvtToRead.size()>0 and !written ) {
                    //    //cout << bxFEvtToRead.front() << " " << overwritingBX << endl;
                    //    distWptrRptr.SetPoint( distWptrRptr.GetN(), ibx, ( bxFEvtToRead.front()-overwritingBX ) );
                    //    written = true;
                    //}
                }
            
            }
            
            if( ++nBXwrittenFEvt == nBXmaxEvtSize ) {
                nBXwrittenFEvt = 0;
                writingFEvt = false;
            }

        }

        /* checking if FEVT needs to be read */
        if( ibx == nextL1acceptBX ) { // do I need to store the L1?
            nextL1acceptBX = ibx + r3.Poisson( L1acceptBX ); // calculate next L1acc
            L1as.push_back(ibx);

            if( ibx%FEvtPeriod_BX == 0 ) {
                bxFEvtToRead.push( ibx );
                bxIdL1aArrival.push( ibx+latency_BX );
            }
        }

        /* reading FEVT if needed */
        if( ( ibx >= bxIdL1aArrival.front() ) && ( ( bxFEvtToRead.size()>0 ) | readingFEvt ) ) {

            if( !readingFEvt ) {
                readingBXid = bxFEvtToRead.front();
                bxFEvtToRead.pop();
                firstAddr = firstWordAddr[readingBXid];
                readingAddr = firstAddr;
                //ram[0]->dump();
            }
            
            readingFEvt = true;
            
            for( unsigned ir=0; ir<link10Gbs_speed_rBX ; ir++ ){
                   
//                cout << "readingAddr   " << readingAddr 
//                     << "\tfirstAddr   " << firstAddr 
//                     << "\treadingLink " << readingLink  
//                     << endl;

                bool vetoIncrement=false;

                if( ram[readingLink]->read(readingAddr, ibx, false) != readingBXid ) {
                    
                    int dist =  readingBXid-ram[readingLink]->read( ram[readingLink]->getWptr(), ibx, false);
                    if(dist < 1000) {
                        distWptrRptr.SetPoint( distWptrRptr.GetN(), ibx,  dist );
                    }

                    if( readingLink<(nLinks-1) ) {
                        readingLink++;
                        ir--;
                        readingAddr = firstAddr;
                    }
                    else {
                     
                        readingLink=0;
                        bxIdL1aArrival.pop();
                        
                        if( ( ibx >= bxIdL1aArrival.front() ) && ( bxFEvtToRead.size()>0 ) ) {
                            readingBXid = bxFEvtToRead.front();
                            bxFEvtToRead.pop();
                            firstAddr = firstWordAddr[readingBXid];
                            readingAddr = firstAddr;
                            vetoIncrement = true;
                        }
                        else {
                            readingFEvt = false;
                        }
                    }
                    
                }

                if( !vetoIncrement ) {
                    if( readingAddr == 4000-1 )
                        readingAddr = 0;
                    else 
                        readingAddr++;
                }
            }

//
//            cout << "ram " << ram[0]->read(readingAddr) << endl;
//            
//            if( ram[0]->read(readingAddr) != readingBXid ) {
//                bxIdL1aArrival.pop();
//                readingFEvt = false;
//            }
           
            //  for( unsigned ilink=0; ilink<nLinks; ilink++ )
        }

    }
    
    cout << "nL1Accept "          << L1as.size() << endl;
    cout << "nL1AcceptFreq [BX] " << (float)nBX/(float)L1as.size() << endl;
    //cout << "nFEvt     "          << nFEvt     << endl;
    //cout << "nFEvtFreq [BX]     " << (float)nBX/(float)FEvts.size() << endl;
//    cout << "nFEvtFreq [Hz]     " << LHCfreq / ( (float)nBX/(float)FEvts.size() )     << endl;
    
//    for(int i=0; i<1000; i++)
//        if( FEvtRepetition[i] > 0 ) cout << "n " << i << " consecutive FEvt appeared " << FEvtRepetition[i] << " times" << endl; 

    cout << "MAXqueueFEvt [BX]  " << MAXqueueForFEvtTransmission << endl;

    //TCanvas c("c", "c", 1);

//    TH1D *hFEvtDist = new TH1D("hFEvtDist", "hFEvtDist", 50000, 0, 50000);

//    unsigned lastBX=0;

//    for(auto ev : FEvts) {
//        
//        if( ev.tbr ) {
//
//            unsigned dist = ev.BX-lastBX;
//            if( dist > 0 )
//                hFEvtDist->Fill( dist );
//            lastBX = ev.BX;
//        }
//
//    }

//    hFEvtDist->Draw();

    
    cout << " writing output file (" << foutName << ") " << endl;
    for( unsigned ilink=0; ilink<nLinks; ilink++ ) {
        TGraph *wGprah = ram[ilink]->getWptrGraph();
        wGprah->Write( Form("Wptr_%d", ilink) );
        TGraph *rGprah = ram[ilink]->getRptrGraph();
        rGprah->Write( Form("Rptr_%d", ilink) );
        distWptrRptr.Write( Form("dist_%d", ilink) );
//
    }
//    app.Run();

    cout << " done." << endl << endl;

    return 0;

}
