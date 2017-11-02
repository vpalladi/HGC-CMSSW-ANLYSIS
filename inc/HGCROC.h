#ifndef HGCROC_H
#define HGCROC_H 1

/* c++ */
#include <iostream>
#include <vector>

/* myIncludes */
#include "HGCTC.h"

using namespace std;

class HGCROC {

public:

    HGCROC() {};
    HGCROC( HGCTC tc ) { 
        zside = tc._zside;
        layer = tc.getCorrectedLayer();
        wafer = tc._wafer;
        third = tc.getThird();
        if( addTC( tc ) == -1 ){
            cout << "----ERROR-TC-not-FOUND----" << endl;
            print();
            tc.print();
            cout << "++++++++++++++++++++++++++" << endl;
        } 
    }

    int addTC( HGCTC tc ) { 
        if(  tcIsContained( tc ) ) {
            _tcs.push_back( tc );
            return 0;
        }
        else {
            return -1;
        }
    }

    bool tcIsContained( HGCTC &tc ){
        if( (zside == tc._zside) and
            (layer == tc.getCorrectedLayer()) and
            (wafer == tc._wafer) and
            (third == tc.getThird()) ) {
            return true;
        }
        else {
            return false;    
        }
 
    }

    /* get TCs related info*/
    std::vector<HGCTC> getTCs() { return _tcs; }

    unsigned getTCsN()          { return _tcs.size(); }

    unsigned getTCsNifOverMipT(float thrMipT) { 
        unsigned n=0;
        for( unsigned itc=0; itc<_tcs.size(); itc++ )
            n = ( _tcs.at(itc).getMipT() > thrMipT ) ? n+1 : n;
        return n;
    }

    unsigned getTCsInMipTrange(float min, float max) { 
        unsigned n=0;
        for( unsigned itc=0; itc<_tcs.size(); itc++ )
            n = ( _tcs.at(itc).getMipT() > min ) ? n+1 : n;
        return n;
    }


    /* STDout print */
    void print(){

        cout << "----------------------------------------" << endl;
        cout << ">>> HGCROC : "
             << "zside " << zside << "; "
             << "layer " << layer << "; "
             << "wafer " << wafer << "; "
             << "third " << third << endl; 
        cout << "      Contains " << _tcs.size() << " TCs" << endl;
        cout << "      Energy   " << getEnergy() << " GeV" << endl;
        cout << "      Energy   " << getMipT()   << " MipT" << endl;
        cout << "++++++++++++++++++++++++++++++++++++++++" << endl;
    }

    /* HGCROC energies */
    float getEnergy() {
        
        float ene=0;
        for(unsigned itc=0; itc<_tcs.size(); itc++)
            ene += _tcs.at( itc )._energy;
        return ene;

    }

    float getMipT() { 

        float mipt=0;
        for(unsigned itc=0; itc<_tcs.size(); itc++)
            mipt += _tcs.at( itc ).getMipT();
        return mipt;

    }


private:
    
    std::vector<HGCTC> _tcs;
    int zside;
    int layer;
    int wafer;
    int third;

};

#endif 





