#ifndef HGC_HT
#define HGC_HT

// C++
#include <iostream>
#include <vector>
#include <utility>
#include <cmath>

// myClasses
#include <HGCline.h>
#include <HGChoughBin.h>
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
    
    vector<TF1>         getTF1s(float min, float max);
    vector<HGChoughBin> getBinsAboveThr(double thr);
    vector<HGChoughBin> getBinsLocalMaxima();
    TH2D*               getTransformedHisto(TString name="houghTransform" );
    TGraph              getXYgraph( TString Name );
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
