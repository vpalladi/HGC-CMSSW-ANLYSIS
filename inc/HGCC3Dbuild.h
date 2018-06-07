
#ifndef HGCC3DBUILD_H
#define HGCC3DBUILD_H

// C++
#include <iostream>
#include <vector>
#include <utility>
#include <cmath>

// my classes
#include "HGCC3D.h"
#include "HGCbin.h"

// ROOT 
#include <TH2D.h>

class HGCbin;

class HGCC3Dbuild{

public:

    HGCC3Dbuild();

    /*
      X, Y = transformed variables 
     */
    HGCC3Dbuild( unsigned nX, float minX, float maxX,
                 unsigned nY, float minY, float maxY );

    ~HGCC3Dbuild();

    void setParams( unsigned nX, float minX, float maxX,
                    unsigned nY, float minY, float maxY );

//    virtual void addPoint(HGChit* hit, double w=1.) = 0;
    
//    vector<TF1>         getTF1s(float min, float max);  // the tranformed lines 
    vector<HGCbin*> getBinsAboveThr    ( double thr ) const;    // all the bins above thr
    vector<HGCbin*> getBinsLocalMaxima (              double thr=0., unsigned nNeighboursSearch=9, unsigned nNeighboursSum=0 ) const; // get all local maxima bins
    void            getHistoLocalMaxima( TH2D &histo, double thr=0., unsigned nNeighboursSearch=9, unsigned nNeighboursSum=0 ) const; // get all local maxima bins
    void            getTransformedHisto( TH2D &histo, double Thr=0 ) const ; // get the tranformed plane
    
    int   nX()   const; 
    int   nY()   const;
    float minX() const;
    float maxX() const;
    float minY() const;
    float maxY() const;

    void addNewC3D( HGCC3D c3d );

    HGCC3D*         getC3D(unsigned id);
    vector<HGCC3D> getNewC3Ds();

    // clear the transform
    void clear();

protected:

    HGCbin** _grid;
    unsigned _nX, _nY;
    float _minX, _minY;
    float _maxX, _maxY;
    float _dX, _dY;

private:

    std::map<unsigned, HGCC3D> _newC3Ds;
    vector<HGCC3D*> _newC3DsVec;


};


#endif
