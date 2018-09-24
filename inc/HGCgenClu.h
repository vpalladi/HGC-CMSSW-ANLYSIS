#ifndef HGCGENCLU_H
#define HGCGENCLU_H 1

/* c/c++ */
#include <iostream>

/* mylibs */
#include "HGCgeom.h"
#include "HGChit.h"
#include "HGCgen.h"


class HGCgenClu : public HGChit {

public:

    HGCgenClu();
    ~HGCgenClu();
    
    void addGen( HGCgen *newGen ); 
    bool addGen( HGCgen *newGen, float normDist ); // returns true if added 
    bool isIncluded( HGCgen *newGen );

    vector<HGCgen*> getConstituents();

    void printConstituents();


private:

    

    vector<HGCgen*> _gens;

};

#endif
