#ifndef HGCHT_H
#define HGCHT_H

// C++
#include <iostream>
#include <vector>
#include <utility>
#include <cmath>

// myClasses
#include <HGCline.h>
#include <HGChoughBin.h>

// ROOT
#include <TF1.h>
#include <TH2D.h>

class HGCht{

public:
    
    HGCht();
    /*
      X, Y = transformed variables 
     */
    HGCht( unsigned nX, float minX, float maxX,
           unsigned nY, float minY, float maxY );

    void setParams( unsigned nX, float minX, float maxX,
                    unsigned nY, float minY, float maxY );

    void addPointPhysicalSpace(float x, float y, int id, double w=1.);
    
    vector<TF1>         getTF1s(float min, float max);  // the tranformed lines 
    vector<HGChoughBin> getBinsAboveThr(double thr);    // all the bins above thr
    vector<HGChoughBin> getBinsLocalMaxima(double thr); // get all local maxima bins
    TH2D*               getTransformedHisto(TString name="houghTransform" ); // get the tranformed plane
    TH2D*               getTransformedHistoThr(TString name="houghTransformThr", double Thr=0 ); // get the tranformed plane with a thr
    TGraph              getXYgraph( TString Name );     // get the graph in physical space
    
    // clear the transform
    void clear();


private:

    vector<pair<float,float>> _XY; 
    HGChoughBin** _grid;
    unsigned _nX, _nY;
    float _minX, _minY;
    float _maxX, _maxY;
    float _dX, _dY;


    vector<HGCline> _linesCollection;
    
};


#endif
