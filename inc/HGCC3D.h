#ifndef HGCC3D_H
#define HGCC3D_H 1

/* c/c++ */
#include <iostream>
#include <map>     // needed to correctly genreate dictionary
#include <vector>

/* mylibs */
#include "detId.h"
#include "HGChit.h"
#include "HGCTC.h"
#include "HGCC2D.h"
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
    void addC2D( const HGCC2D* c2d );
    void addTC ( const HGCTC*  tc  );

    /* set methods*/
    void setClusters(vector<unsigned> clusters) ;
    void setCells(vector<unsigned> cells)       ;
    void setNearestGen(HGCgen &gen)             ;
    void setNearestGen(vector<HGCgen*> gens)    ;
    void setXnorm(float xNorm) ;
    void setYnorm(float yNorm) ;
    void setFirstLayer(unsigned firstLayer);
    void setMaxLayer(unsigned maxLayer);
    void setLastLayer(unsigned lastLayer);
    void setShowerLength(unsigned showerLength);

    /* get methods */
    unsigned         nclusters() const;
    vector<unsigned> clusters() const;

    unsigned         ncells() const;
    vector<unsigned> cells() const;

    TLorentzVector P4() const;

    unsigned getFirstLayer() const;
    unsigned getShowerLength() const; // in layers
    unsigned getMaxLayer() const;
    unsigned getLastLayer() const;

    ROOT::Math::RhoEtaPhiPoint getZprojection( double z );

    float xNorm() const ;
    float yNorm() const ;

    HGCgen getNearestGen() const;

    float getEnergyDensity(float radiusNorm) const;
    float getEnergyDensity() const; // uses the number of tc 
    float getEnergyDensity(vector<const HGCC3D*> C3Ds, double radiusRegionNorm, double radiusNorm) const;
    float getEnergyDensity(vector<HGCC3D> *C3Ds, double radiusRegionNorm, double radiusNorm) const;
 
//    bool isGolden(int maxHoleLayers, int minNumberOfLayers);

    void print();

 private:

    unsigned _firstLayer;
    unsigned _lastLayer;
    unsigned _maxLayer;
    unsigned _showerLength;

    vector<unsigned> _clusters ; // vector of HGCDetId for all the C2D
    vector<unsigned> _cells    ; // vector of HGCDetId for all the TC

    HGCgen _nearestGen;

    float _xNorm, _yNorm;

    ClassDef( HGCC3D,2 )

};

#endif 
