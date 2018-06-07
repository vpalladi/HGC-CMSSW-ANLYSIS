
#ifndef HGCSINGLEC3D_H
#define HGCSINGLEC3D_H

/* c++ */
#include <map>

/* mylibs */
#include "HGCC2D.h"
#include "HGCC3D.h"

class HGCsingleC3D {// : public HGCC3Dbuild {

public:

    HGCsingleC3D();
    ~HGCsingleC3D();

    void addNewC3D( HGCC3D c3d );

    void addPoint(const HGCC2D c2d);

    HGCC3D*        getC3D(unsigned id);
    vector<HGCC3D> getNewC3Ds();
    
    void clear();

private:
    
    std::map<unsigned, HGCC3D> _newC3Ds;
    vector<HGCC3D*> _newC3DsVec;

};

#endif
