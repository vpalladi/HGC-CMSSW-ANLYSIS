#ifndef MEMMAN_H
#define MEMMAN_H

#include <iostream>

using namespace std;

class URAMslot {

public:

    URAMslot(unsigned nMaxWords, unsigned slotId, unsigned nLinks) {
        _nMaxWords = nMaxWords;
        _firstAddr = slotId*_nMaxWords; 
        _lastAddr  = (slotId+1)*_nMaxWords-1;
        _nLinks    = nLinks;
        _linksWordContent = new uint64_t[_nLinks];
        this->reset();
    }
    
    bool isAvailable;
    bool isActive;
    uint64_t BXcontent;
    bool markedToRead;
    bool reading;
    
    unsigned firstAddr() { return _firstAddr; } 
    unsigned lastAddr()  { return _lastAddr;  } 

    int write() {

        for(uint64_t ilink=0; ilink<_nLinks; ilink++) {
            if( _linksWordContent[ilink] < _nMaxWords ) {
                _linksWordContent[ilink]++;
            }
            else {
                cout << "WARNING (URAMslot): link is full." << endl;
                return -1;
            }
        }
        
        return 0;
            
    }
    
    int read(uint64_t ilink) {
        
        if( _linksWordContent[ilink]>0){
            _linksWordContent[ilink]--;
            return 0;
        }
        
        cout << "WARNING (URAMslot): link is empty." << endl;
        return -1;
        
    }

    bool isLinkEmpty(uint64_t ilink) {
        if( _linksWordContent[ilink]==0 )
            return 1;
        else
            return 0;
    }

    bool isLinkFull(uint64_t ilink) {
        if( _linksWordContent[ilink]==_nMaxWords )
            return 1;
        else
            return 0;
    }
    

    bool isEmpty() {
        for(uint64_t ilink=0; ilink<_nLinks; ilink++)
            if( _linksWordContent[ilink] != 0 )
                return false;
            return true;
        
    }
    
    bool isFull() {
        for(uint64_t ilink=0; ilink<_nLinks; ilink++)
            if( _linksWordContent[ilink] != _nMaxWords )
                return false;
        return true;
    }
    
    void reset() {
        isAvailable = true; // is slot available? 
        isActive = false; // is this the slot I am usign? 
        BXcontent = 0;
        for(uint64_t ilink=0; ilink<_nLinks; ilink++)
            _linksWordContent[ilink]=0;
        markedToRead = false;
        reading = false;
    }

private:
    unsigned _nMaxWords;
    uint64_t _firstAddr;
    uint64_t _lastAddr;
//uint64_t _nWritteWords;
    uint64_t _nLinks;
    uint64_t *_linksWordContent;
    
    
};


class memMan {

public:

    memMan(unsigned nLinks, unsigned nURAM,
           unsigned ramWrate, unsigned ramRrate, // rate of oprations per BX W=wr R=Rd
           uint64_t evtSizeWord,
           unsigned L1latency,
           unsigned FevtPeriod=107,
           unsigned TMUXperiod=1,
           unsigned algoLatency=0, // expressed in BX 
           bool     transmitThinEvts=false,
           bool     wPensistency=false, bool rPensistency=false,
           bool     verbose=false
        );
    
    int getActiveSlotId();

    int update(uint64_t BxId, bool isL1, uint64_t L1bx, bool readVeto=false);

    uint64_t getCurrentSlot() { return _iSlot; }
    
    unsigned getEvtToread()   { return _slotToRead.size(); }

    bool isWriting() { return _slots[_iSlot]->isActive; }
    bool isReading() {
        if( _slotToRead.size()>0 )
            return _slots[_slotToRead.front()]->reading;
        return false;
    }
    
    unsigned getNavailableSlots();
    unsigned getNslots() { return _nSlots; }

    unsigned isSlotAvailable(unsigned iSlot) { return _slots[iSlot]->isAvailable; }

    void printSlotsAvailability();
    void printSlotsActive();
    void print();

    void write(TString baseName);
    
private:

    bool _verbose;
    
    bool _wPensistency, _rPensistency;
    
    unsigned _FevtPeriod, _TMUXperiod;
    unsigned _nLinks; 

    unsigned _L1latency;
    unsigned _algoLatency;
    bool _transmitThinEvts;

    URAM** _URAMs;
    uint64_t _depth;
    uint64_t _evtSizeWord;
    uint64_t _ramWrate, _ramRrate;
    
    unsigned _nSlots;
    URAMslot **_slots;

    unsigned _iSlot; 
    unsigned _nextAvailableSlot;
    unsigned _lastAvailableSlot;

    unsigned _readingLinkId;
    queue<uint64_t> _slotToRead;
    
};


memMan::memMan(unsigned nLinks, unsigned nURAM,
               unsigned ramWrate, unsigned ramRrate, // rate of oprations per BX W=wr R=Rd
               uint64_t evtSizeWord,
               unsigned L1latency,
               unsigned FevtPeriod,
               unsigned TMUXperiod,
               unsigned algoLatency, // expressed in BX 
               bool     transmitThinEvts,
               bool     wPensistency, bool rPensistency,
               bool     verbose
    ) {

    
    /* pensistency and verbose flags */
    _verbose = verbose;
    _wPensistency = wPensistency;
    _rPensistency = rPensistency;
    
    /* memory */
    _L1latency = L1latency;
    _depth = nURAM*4096;
    _evtSizeWord = evtSizeWord;
    _nLinks = nLinks;
    
    _URAMs = new URAM*[_nLinks];
    for( uint64_t i=0; i<_nLinks; i++ )
        _URAMs[i] = new URAM(_depth);
    
    _ramWrate = ramWrate;
    _ramRrate = ramRrate;
    
    /* periods */
    _FevtPeriod = FevtPeriod;
    _TMUXperiod = TMUXperiod;

    /* slots */
    _nSlots = _depth/_evtSizeWord;
    
    _slots = new URAMslot*[_nSlots];
    
    for(unsigned islot=0; islot<_nSlots; islot++)
        _slots[islot] = new URAMslot(_evtSizeWord, islot, _nLinks);
    
    _iSlot = 0;
    _nextAvailableSlot = 1;
    _lastAvailableSlot = _nSlots-1;
    
    /* latency and co. */
    _algoLatency = algoLatency;
    _transmitThinEvts = transmitThinEvts; 
    
}

int memMan::getActiveSlotId() {
    
    for(unsigned islot=0; islot<_nSlots; islot++ )
        if( _slots[islot]->isActive )
            return islot;
    return -1;
    
}


int memMan::update(uint64_t BxId, bool isL1, uint64_t L1bx, bool readVeto) {
        
    if( BxId<_L1latency )
        return -1;

    uint64_t current_ibx = BxId;
    uint64_t ibx = BxId-_algoLatency;
        
    /* writing buffers */
    bool isFevt = (ibx%_FevtPeriod == 0) && (ibx%_TMUXperiod == 0);
    bool isTevt =  ibx%_TMUXperiod == 0;

    bool isRelevant = (isFevt && !_transmitThinEvts) || (isTevt && _transmitThinEvts);


    /* write events */
    if( ( isRelevant || _slots[_iSlot]->isActive ) && ibx!=0 ) { // do I need to store the Event?

        /* checking if relevant */
        if( !_slots[_iSlot]->isFull() ) {
            if( isRelevant ) {
                
                _slots[_iSlot]->isAvailable = false; 
                _slots[_iSlot]->isActive    = true;
                _slots[_iSlot]->BXcontent   = ibx;
                
            }
            
            /* looping over links */
            for( uint64_t ilink=0; ilink<_nLinks; ilink++ ) {
                signed ptr = isRelevant ? _slots[_iSlot]->firstAddr() : -1;                
                
                /* read */
                for( uint64_t iw=0; iw<_ramWrate ; iw++ ) { 
                    _URAMs[ilink]->write( _slots[_iSlot]->BXcontent, current_ibx, ptr, _wPensistency ); // write into ram
                    ptr=-1;
                }
            }
        
            /* end of the event check */
            for( uint64_t iw=0; iw<_ramWrate ; iw++ ) 
                _slots[_iSlot]->write();
            
        }

        /* next slot */
        if( _slots[_iSlot]->isFull() ) {
            
            if( _slots[_nextAvailableSlot]->isAvailable ) {
                _slots[_iSlot]->isActive     = false;
                _iSlot = _nextAvailableSlot;
            }
            else {
                _slots[_iSlot]->isActive     = true;
            }

            /* next available slot */
            unsigned nextAvailableSlot = _nextAvailableSlot;
            do {
                
                nextAvailableSlot = (nextAvailableSlot==_nSlots-1) ? 0 : nextAvailableSlot+1 ;
                if( _slots[nextAvailableSlot]->isAvailable ){
                    _nextAvailableSlot = nextAvailableSlot;
                    break;
                }
                
            } while( nextAvailableSlot != _nextAvailableSlot );
            
        }   
                
    } // end writing 

    
    /* update the L1 requests */
    if( isL1 ) {

        /* check if there is an event to read out */
        if( _transmitThinEvts || (L1bx%_FevtPeriod==0) ) 
            for(unsigned islot=0; islot<_nSlots; islot++) 
                if( _slots[islot]->BXcontent == L1bx ) {
                    _slotToRead.push(islot);
                    _slots[islot]->markedToRead = true;
                }
        
    }// end of L1 actions

    
    /* read event */
    if( !_slotToRead.empty() && !readVeto ) {
        
        _slots[_slotToRead.front()]->reading = true;
        
        signed ptr=-1;

        /* link reader */
        if( _slots[_slotToRead.front()]->isFull() ) {
            _readingLinkId=0;
            ptr = _slots[_slotToRead.front()]->firstAddr();
        }
        else if( _slots[_slotToRead.front()]->isLinkEmpty(_readingLinkId) ) { 
            _readingLinkId++;
            ptr = _slots[_slotToRead.front()]->firstAddr();
        }
        else {
            ptr = -1;
        }

        if( _verbose )
            cout << " >>> Reading Link " << _readingLinkId << endl;

        /* reding */
        for( uint64_t ir=0; ir<_ramRrate ; ir++ ) { 
            _URAMs[_readingLinkId]->read( current_ibx, ptr, _rPensistency ); // read ram
            _slots[_slotToRead.front()]->read(_readingLinkId);
            if(_verbose)
                cout << ibx << " " << _URAMs[_readingLinkId]->getRptr() << endl;
            ptr=-1;
        }

        /* check f reading is over */
        if( _slots[_slotToRead.front()]->isEmpty() ) {
            _slots[_slotToRead.front()]->reset(); // free the memory
            _slotToRead.pop();          // go to the next slot to read 
        }
        
    }
        
    /* update ram parameters */
    for(unsigned islot=0; islot<_nSlots; islot++) {
        if( !_slots[islot]->isAvailable && !_slots[islot]->markedToRead ) {
            if( (ibx-_slots[islot]->BXcontent) > _L1latency ) 
                _slots[islot]->reset();
        }
    }
        
    return 0;
        
}


unsigned memMan::getNavailableSlots() {

        unsigned n=0;
        
        for(unsigned islot=0; islot<_nSlots; islot++) 
            if(_slots[islot]->isAvailable)
                n++;

        return n;            
       
    }


void memMan::printSlotsAvailability() {
    
    for(unsigned islot=0; islot<_nSlots; islot++) {
        cout << "Slot " << islot ;
        
        if( _slots[islot]->isAvailable )
                cout << " available" << endl; 
        else
            cout << " NOT available" << endl; 
        
    }
}


void memMan::printSlotsActive() {
    
    for(unsigned islot=0; islot<_nSlots; islot++) {
        cout << "Slot " << islot ;
        
        if( _slots[islot]->isActive )
            cout << " active" << endl; 
        else
            cout << " NOT active" << endl; 
        
    }
}


void memMan::print() {
    
    cout << " FevtPeriod    "  << _FevtPeriod    << endl;
    cout << " TMUXperiod    "  << _TMUXperiod    << endl;
    cout << " nLinks        "  << _nLinks        << endl;
    cout << " L1latency     "  << _L1latency       << endl;
    cout << " algoLatency   "  << _algoLatency       << endl;
    cout << " transmitThinEvts "  << _transmitThinEvts << endl;
    cout << " depth         "  << _depth         << endl;
    cout << " evtSizeWord   "  << _evtSizeWord   << endl;
    cout << " ramWrate      "  << _ramWrate      << endl;
    cout << " ramRrate      "  << _ramRrate      << endl;
    cout << " nSlots        "  << _nSlots        << endl;
    
}


void memMan::write(TString plotName) {

    for( uint64_t ilink=0; ilink<_nLinks; ilink++ ) {

        TString sW = plotName;
        TString sR = plotName;
        sW.Append( Form("_W_%u",ilink) );
        sR.Append( Form("_R_%u",ilink) );
        
        TGraph* gW = _URAMs[ilink]->getWptrGraph();
        TGraph* gR = _URAMs[ilink]->getRptrGraph();
        if(_wPensistency)
            gW->Write(sW);
        if(_rPensistency)
            gR->Write(sR);
        
    }
    
}



#endif
