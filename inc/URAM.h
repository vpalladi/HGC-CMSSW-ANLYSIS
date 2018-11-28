#ifndef URAM_H
#define URAM_H

#include <iostream>

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
    unsigned getRptr() { return _rptr; }

    void write( unsigned w, int bx, signed ptr=-1, bool addToGraph=true ) {

        if( ptr!=-1 )
            _wptr = ptr;

        _ram[_wptr] = w;

        if( addToGraph ) _gWptr->SetPoint( _gWptr->GetN(), bx, _wptr );

        if( _wptr == (_URAMdepth-1) ) 
            _wptr = 0;
        else 
            _wptr++;
        
    }
    
    unsigned read( int bx, signed ptr=-1, bool addToGraph=true ) {

        unsigned value;
        
        if( (ptr != -1) && (ptr >= _URAMdepth) ){
            std::cout << " >>> WARING (URAM): address out of range. Requested"
                      << ptr << " but Max is " <<  _URAMdepth << std::endl;
            return -1; 
        }
        else if( ptr > -1 ){
            _rptr = ptr;
        }

        value = _ram[_rptr];
            
        if( addToGraph ) {
            _gRptr->SetPoint( _gRptr->GetN(), bx, _rptr );
        }

        /* update the rpointer */
        _rptr = _rptr==(_URAMdepth-1) ? 0 : (_rptr+1);

        return value;

    }

    void dump() {

        std::cout << "RAM content" << std::endl;

        for (unsigned i=0; i<_URAMdepth; i++)
            std::cout << i << " " << _ram[i] << std::endl;

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

#endif
