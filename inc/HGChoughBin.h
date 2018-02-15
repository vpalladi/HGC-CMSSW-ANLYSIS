
#ifndef HGCHOUGHBIN_H
#define HGCHOUGHBIN_H

/* C++ */
#include <iostream>
#include <vector>
#include <utility>

/* ROOT */
#include <TGraph.h>

/* myLibs */
#include <HGC.h>

using namespace std;

class HGChoughBin{

public:

    HGChoughBin() { ; }

    HGChoughBin(float x, float y) { 
        this->setCentre(x, y); 
    }
    ~HGChoughBin() { ; }
    
    void setCentre(float x, float y) { 
        _centre.first  = x; 
        _centre.second = y;
    }

    void setCentreId(int xBin, int yBin) { 
        _centreId.first = xBin; 
        _centreId.second = yBin;
    }

    void addContent( float content, unsigned id ) { 
        _content += content; 
        _ids.push_back( id ); 
    }

    void clear() {
        _content=0;
        _ids.clear();
    }

    pair<float,float> getCentre()   { return _centre;   }
    pair<int,int>     getCentreId() { return _centreId; }
    vector<unsigned>  getIds()      { return _ids;      }
    float             getContent()  { return _content;  }

    void getLongitudinalEnergyProfileC2D(HGC &det, TGraph &g){
        
        for( std::vector<unsigned>::iterator id=_ids.begin(); id!=_ids.end(); id++ )            
            g.SetPoint( g.GetN(), det.getC2D( (*id) ).z(), det.getC2D( (*id) ).Energy() );
        
    }

    void getLongitudinalPtProfileC2D(HGC &det, TGraph &g){
        
        for( std::vector<unsigned>::iterator id=_ids.begin(); id!=_ids.end(); id++ )            
            g.SetPoint( g.GetN(), det.getC2D( (*id) ).z(), det.getC2D( (*id) ).Pt() );
        
    }

    bool isAboveThr(double thr) { 
        if( _content > thr ) return true; 
        return false; 
    }

private:

    float _content;
    vector<unsigned> _ids;
    pair<float,float> _centre;   /* first=x; second=y */
    pair<int,int>     _centreId; /* first=x; second=y */

};

#endif
