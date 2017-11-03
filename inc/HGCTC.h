#ifndef HGCTC_H
#define HGCTC_H 1

/* c/c++ */
#include <iostream>

/* mylibs */
#include "detId.h"

/* ROOT */
#include "TMath.h"

using namespace std;


class HGCTC {

 public:

    HGCTC();

    void setId(unsigned int id) { _id = id; }
    void setSubdet(int subdet)  { _subdet = subdet; }
    void setZSide(int zside)    { _zside = zside; }
    void setLayer(int layer)    { _layer = layer; }
    void setWafer(int wafer)    { _wafer = wafer; }
    void setWaferType(int wafertype)    { _wafertype = wafertype; }
    void setCell(int cell)      { _cell = cell; }
    void setData(int data)      { _data = data; }
    void setEnergy(float energy)  { _energy = energy; }
    void setEta(float eta)      { _eta = eta; }
    void setPhi(float phi)      { _phi = phi; }
    void setZ(float z)          { _z = z; }


    unsigned int id()        { return _id; }
    int subdet()    { return _subdet; }
    int zside()     { return _zside; }
    int layer()     { return _layer; }
    int wafer()     { return _wafer; }
    int waferyype() { return _wafertype; }
    int cell()      { return _cell; }
    unsigned int data()      { return _data; }
    float Energy()    { return _energy; }
    float Eta()       { return _eta; }
    float Phi()       { return _phi; }
    float z()         { return _z; }    
    
    float Pt()        { return _energy/cosh(_eta); }

    int correctedLayer();

    int third();

    float MipT();

    void print();

    
    /* operators overloading */
    bool operator==( const HGCTC &comp );


 private:

    unsigned int _id        ;
    int          _subdet    ;
    int          _zside     ;
    int          _layer     ;
    int          _wafer     ;
    int          _wafertype ;
    int          _cell      ;
    unsigned int _data      ;
    float        _energy    ;
    float        _eta       ;
    float        _phi       ;
    float        _z         ;

};

#endif 





