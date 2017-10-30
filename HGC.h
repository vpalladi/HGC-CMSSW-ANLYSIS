#ifndef HGC_H
#define HGC_H 1

/* c/c++ */
#include <iostream>

/* mylibs */
#include "detId.h"
#include "HGCTC.h"
#include "HGCC2D.h"
#include "HGCROC.h"

/* ROOT */
#include "TMath.h"

using namespace std;

const unsigned Nlayers = 53; // layers' name starts at 1 

class HGC {

public:

    HGC() {
        for(unsigned i=0; i<Nlayers; i++) {
            _TC[i]  = new vector<HGCTC>  ;
            _C2D[i] = new vector<HGCC2D> ;
            _TD[i]  = new vector<HGCROC> ;
            _TC[i] ->reserve(10000); 
            _C2D[i]->reserve(600); 
            _TD[i] ->reserve(5000);
        }
    };
    ~HGC() {};

    void addTC( HGCTC tc ) { 
        const unsigned tcLayer = tc.getCorrectedLayer();
        _TC[tcLayer]->push_back( tc ); 
        HGCROC TD( tc );
        /* add to the correct HGCROC */
        bool found = false;
        for(unsigned int ihgcroc=0; ihgcroc<_TD[tcLayer]->size(); ihgcroc++ ){ 
            if( _TD[tcLayer]->at(ihgcroc).tcIsContained( tc ) ){
                found = true;
                _TD[tcLayer]->at(ihgcroc).addTC( tc );
                break;
            }
        }
        if( !found ) _TD[tcLayer]->push_back( TD );
        
    }
    void addC2D(HGCC2D c2d) { _C2D[c2d._layer]->push_back( c2d ); }

    vector<HGCTC>  *getTC(unsigned layer)  { return _TC[layer];  }
    vector<HGCC2D> *getC2D(unsigned layer) { return _C2D[layer]; }
    vector<HGCROC> *getTD(unsigned layer)  { return _TD[layer];  }

    vector<HGCTC>  *getTCall() {  
        vector<HGCTC> *data = new vector<HGCTC>;
        for(unsigned i=0; i<Nlayers; i++)
            data->insert(data->end(), _TC[i]->begin(), _TC[i]->end() );
        return data;
    }
    vector<HGCC2D> *getC2Dall() {  
        vector<HGCC2D> *data = new vector<HGCC2D>;
        for(unsigned i=0; i<Nlayers; i++)
            data->insert(data->end(), _C2D[i]->begin(), _C2D[i]->end() );
        return data;
    }
    vector<HGCROC> *getTDall() {
        vector<HGCROC> *data = new vector<HGCROC>;
        for(unsigned i=0; i<Nlayers; i++)
            data->insert(data->end(), _TD[i]->begin(), _TD[i]->end() );
        return data;
    }

    void clear() {
        for(unsigned i=0; i<Nlayers; i++){
            _TC[i]->clear();
            _C2D[i]->clear();
            _TD[i]->clear();
        }    
    }

private:
 
    vector<HGCTC>  *_TC[Nlayers];
    vector<HGCC2D> *_C2D[Nlayers];
    vector<HGCROC> *_TD[Nlayers];

};

#endif





