#ifndef HGCTC_H
#define HGCTC_H 1

/* c/c++ */
#include <iostream>
#include <map>    // needed to generate the dict

/* mylibs */
#include "HGChit.h"
#include "detId.h"
#include "HGCgeom.h"

/* ROOT */
#include "TObject.h"
#include "TMath.h"
#include "TLorentzVector.h"

using namespace std;


class HGCTC : public HGChit {
    
public:
    
    HGCTC();
    ~HGCTC();
    
    /* set methods */
    void     setZside(int zside)         ;
    void     setWafer(int wafer)         ;
    void     setWaferType(int wafertype) ;
    void     setCell(int cell)           ;
    void     setData(int data)           ;
    
    /* get methods */
    int      zside()     ; // -1 and 1
    int      wafer()     ;
    int      wafertype() ;
    int      cell()      ;
    unsigned data()      ;
    
    /* get Pt and MipT */
    float MipT();
    
    /* get the diamond info */
    int third();
    
private:
    
    int      _zside     ;
    int      _wafer     ;
    int      _wafertype ;
    int      _cell      ;
    unsigned _data      ;

    ClassDef(HGCTC,1)

};

#endif 





