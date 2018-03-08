#ifndef HGCHT_H
#define HGCHT_H

// C++
#include <iostream>
#include <vector>
#include <utility>
#include <cmath>

// myClasses
#include <HGCline.h>
#include <HGCbin.h>

// ROOT
#include <TF1.h>
#include <TH2D.h>

class HGCbin;

class HGCht{

public:
    
    HGCht();
    /*
      X, Y = transformed variables 
     */
    HGCht( unsigned nX, float minX, float maxX,
           unsigned nY, float minY, float maxY );

    ~HGCht();

    void setParams( unsigned nX, float minX, float maxX,
                    unsigned nY, float minY, float maxY );

    void addPointPhysicalSpace(float x, float y, int id, double w=1.);
    
    vector<TF1>         getTF1s(float min, float max);  // the tranformed lines 
    vector<HGCbin> getBinsAboveThr(double thr);    // all the bins above thr
    vector<HGCbin> getBinsLocalMaxima(double thr); // get all local maxima bins
    void                getTransformedHisto(TH2D &histo, double Thr=0 ); // get the tranformed plane
    void                getXYgraph( TGraph &g );        // get the graph in physical space
    
    // clear the transform
    void clear();


private:

    vector<pair<float,float>> _XY; 
    HGCbin** _grid;
    unsigned _nX, _nY;
    float _minX, _minY;
    float _maxX, _maxY;
    float _dX, _dY;

    vector<HGCline> _linesCollection;
    
};


#endif
