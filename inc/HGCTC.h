#ifndef HGCTC_H
#define HGCTC_H 1

/* c/c++ */
#include <iostream>
#include <map>    // needed to generate the dict

/* mylibs */
#include "detId.h"

/* ROOT */
#include "TObject.h"
#include "TMath.h"
#include "TLorentzVector.h"

using namespace std;


class HGCTC : public TObject {

 public:

    HGCTC();
    ~HGCTC();

    /* set methods */
    void     setId(unsigned id)          ;
    void     setSubdet(int subdet)       ;
    void     setZSide(int zside)         ;
    void     setLayer(int layer)         ;
    void     setWafer(int wafer)         ;
    void     setWaferType(int wafertype) ;
    void     setCell(int cell)           ;
    void     setData(int data)           ;
    void     setEnergy(float energy)     ;
    void     setEta(float eta)           ;
    void     setPhi(float phi)           ;
    void     setX(float x)               ;
    void     setY(float y)               ;
    void     setZ(float z)               ;

    /* get methods */
    unsigned id()        ;
    int      subdet()    ;
    int      zside()     ;
    int      layer()     ;
    int      wafer()     ;
    int      wafertype() ;
    int      cell()      ;
    unsigned data()      ;
    float    Energy()    ;
    float    Eta()       ;
    float    Phi()       ;
    float    z()         ;
    float    x()         { return _x; }
    float    y()         { return _y; }
    float    r()         { return TMath::Sqrt( x()*x() + y()*y() ); }

    /* get Pt and MipT */
    float Pt()           ;
    //float MipT()         ;

    /* Layer is corrected using subdet, first layer id is 1 NOT 0 */
    int correctedLayer();

    /* get the diamond info */
    int third();
    float MipT();
    TLorentzVector P4();
  
    /* get some useful info printed on the stdout */
    void print();
    
    /* operators overloading */
    bool operator==( const HGCTC &comp );


 private:

    float _MipT()       ;

    unsigned _id        ;
    int      _subdet    ;
    int      _zside     ;
    int      _layer     ;
    int      _wafer     ;
    int      _wafertype ;
    int      _cell      ;
    unsigned _data      ;
    float    _energy    ;
    float    _eta       ;
    float    _phi       ;
    float    _x, _y, _z ;

    /* derived */
    float    _pt        ;
    float    _mipt        ;

    ClassDef(HGCTC,1)

};

#endif 





