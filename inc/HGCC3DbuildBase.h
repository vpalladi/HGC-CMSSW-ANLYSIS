#ifndef HGCC3DBUILDBASE_H
#define HGCC3DBUILDBASE_H

/* c++ */
#include <map>

/* mylibs */
#include "HGCC3D.h"

template<class T>
class HGCC3DbuildBase {// : public HGCC3Dbuild {

public:

    HGCC3DbuildBase() { }
    ~HGCC3DbuildBase() { }

    void addNewC3D( HGCC3D c3d ) {
        _newC3Ds[c3d.id()] = c3d; 
        _newC3DsVec.push_back( &_newC3Ds[c3d.id()] );   
    }
    
    virtual void addPoint(const T hit) = 0;

    HGCC3D*        getC3D(unsigned id) { return &_newC3Ds[id]; }
    vector<HGCC3D> getNewC3Ds() {
        vector<HGCC3D> vec;
        
        for( int i=0; i<_newC3DsVec.size(); i++ ) {
            vec.push_back( *_newC3DsVec.at(i) );
        }
        
        return vec;
    }
    
    void clear() {
        _newC3DsVec.clear();
        _newC3Ds.clear();
    }

private:
    
    std::map<unsigned, HGCC3D> _newC3Ds;
    vector<HGCC3D*> _newC3DsVec;

};

#endif
