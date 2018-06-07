
#ifndef HGCNORM_H
#define HGCNORM_H

/* c++ */
#include <map>

/* mylibs */
#include "HGCC3Dbuild.h"
#include "forestSearch.h"
#include "HGCC2D.h"
#include "HGCC3D.h"

class HGCnorm : public HGCC3Dbuild {

public:

//    using HGCC3Dbuild::HGCC3Dbuild;
    HGCnorm();
    HGCnorm ( unsigned nX, float minX, float maxX,
              unsigned nY, float minY, float maxY );
    ~HGCnorm();

    void addPoint(const HGCC2D* hit, double w=1.);

    vector<HGCC3D> getNewC3Ds( unsigned nNeighboursSearch, double radius, vector<HGCgen*> gens=vector<HGCgen*>() );
    
    void clear();

private:

    //void buildNewC3Ds( unsigned nNeighboursSearch=9, unsigned nNeighboursSum=0, vector<HGCgen*> gens=vector<HGCgen*>() );  // not in use
    void buildNewC3Ds( unsigned nNeighboursSearch, double radius, vector<HGCgen*> gens=vector<HGCgen*>() );    
    std::map<unsigned, HGCC2D> _C2Ds;
    std::vector<HGCC2D*> _vecC2Ds;

};

#endif
