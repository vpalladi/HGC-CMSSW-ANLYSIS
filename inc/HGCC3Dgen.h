#ifndef HGCC3DGEN_H
#define HGCC3DGEN_H

/* c++ */
#include <map>

/* mylibs */
#include "HGCC3DbuildBase.h"
#include "HGCC2D.h"
#include "HGCC3D.h"

class HGCC3Dgen : public HGCC3DbuildBase<HGCC2D> {

public:

    HGCC3Dgen();
    ~HGCC3Dgen();

    void addPoint(const HGCC2D c2d); // not used
    void addPoint(const HGCC2D c2d, vector<HGCgen*> gens, double radius);

    void clear();
    
private:
    
    std::map<unsigned, unsigned> _genC3D;

};

#endif
