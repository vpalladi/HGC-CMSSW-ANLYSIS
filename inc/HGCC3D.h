#ifndef HGCC3D_H
#define HGCC3D_H 1

/* c/c++ */
#include <iostream>
#include <map>     // needed to correctly genreate dictionary
#include <vector>

/* mylibs */
#include "detId.h"
#include "HGChit.h"
#include "HGCC2D.h"
#include "HGCC3D.h"
#include "HGCgen.h"

/* ROOT */
#include "TLorentzVector.h"
#include "Math/Point3D.h"

using namespace std;

class HGCC3D : public HGChit {

 public:

    HGCC3D();
    ~HGCC3D();

    /* add */
    void addC2D( const HGCC2D* c2d);

    /* set methods*/
    void setClusters(vector<unsigned> clusters) ;
    void setCells(vector<unsigned> cells)       ;
    void setNearestGen(HGCgen &gen)             ;
    void setNearestGen(vector<HGCgen*> gens)    ;
    void setXnorm(float xNorm) ;
    void setYnorm(float yNorm) ;
    void setFirstLayer(float firstLayer);
    void setMaxLayer(float maxLayer);

    /* get methods */
    unsigned         nclusters() const;
    vector<unsigned> clusters() const;

    unsigned         ncells() const;
    vector<unsigned> cells() const;

    TLorentzVector P4() const;

    unsigned getFirstLayer() const;
    unsigned getMaxLayer() const;

    ROOT::Math::RhoEtaPhiPoint getZprojection( double z );

    float xNorm() const ;
    float yNorm() const ;

    HGCgen getNearestGen() const;

//    bool isGolden(int maxHoleLayers, int minNumberOfLayers);

    void print();

 private:

    unsigned _firstLayer;
    unsigned _maxLayer;

    vector<unsigned> _clusters ; // vector of HGCDetId for all the C2D
    vector<unsigned> _cells    ; // vector of HGCDetId for all the TC

    HGCgen _nearestGen;

    float _xNorm, _yNorm;
  

    ClassDef( HGCC3D,2 )

};

#endif 
