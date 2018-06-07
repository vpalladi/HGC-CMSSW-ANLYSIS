
#ifndef HGCFOREST_H
#define HGCFOREST_H

/* c++ */
#include <map>

/* mylibs */
#include "HGCC3Dbuild.h"
#include "forestSearch.h"
#include "HGCC2D.h"
#include "HGCC3D.h"


class HGCforest : public HGCC3Dbuild {

public:

//    using HGCC3Dbuild::HGCC3Dbuild;
    HGCforest();
    HGCforest ( unsigned nX, float minX, float maxX,
                unsigned nY, float minY, float maxY );
    ~HGCforest();
    
    void setNormRadius(double radius);
    void setDACbit(int nbits);
    void addPoint(const HGCC2D hit, double w=1.);

    void clear();

private:

    double _normRadius;
    int _nbits;

    ForestSearch::Search< HGCC2D , 13 , 13 > *_aTree;

};

#endif
