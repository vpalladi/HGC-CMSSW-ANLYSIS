
#ifndef HGCSINGLEC3D_H
#define HGCSINGLEC3D_H

/* c++ */
#include <map>

/* mylibs */
#include "HGCC3DbuildBase.h"
#include "HGCC2D.h"
#include "HGCC3D.h"

class HGCsingleC3D : public HGCC3DbuildBase<HGCC2D> {

public:

    HGCsingleC3D();
    ~HGCsingleC3D();

    void addPoint(const HGCC2D c2d);

    void clear();

};

#endif
