#ifndef HGCHT_H
#define HGCHT_H

// C++
#include <iostream>
#include <vector>
#include <utility>
#include <cmath>

// myClasses
#include "HGCC3Dbuild.h"
#include "HGCline.h"
#include "HGChit.h"

// ROOT
#include <TF1.h>
#include <TH2D.h>
#include <TGraph.h>


class HGCht : public HGCC3Dbuild { 

public:
      
    HGCht() : HGCC3Dbuild(), _zOffset(0.), _slopeCorrection(1.) { }
    
    HGCht( unsigned nX, float minX, float maxX, unsigned nY, float minY, float maxY ) 
        : HGCC3Dbuild( nX, minX, maxX, nY, minY, maxY ), _zOffset(0.), _slopeCorrection(1.) { }


    void setZoffset(float zOffset);
    void setSlopeCorrection(float SC);

    void addPoint(const HGChit* hit , double w=1.);
    
    vector<TF1> getTF1s(float min, float max);  // the tranformed lines 
    void        getXYgraph( TGraph &g );        // get the graph in physical space

    void clear();

private:

    vector<pair<float,float>> _XY;

    float _zOffset;
    float _slopeCorrection;
    
    vector<HGCline> _linesCollection;
    
    
};

#endif
