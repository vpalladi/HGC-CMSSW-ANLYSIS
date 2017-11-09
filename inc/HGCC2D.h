#ifndef HGCC2D_H
#define HGCC2D_H 1

/* c/c++ */
#include <iostream>
#include <vector>
#include <utility> // pair

/* mylibs */
#include "detId.h"

/* ROOT */
#include "TMath.h"
#include "TVector3.h"
#include "TLorentzVector.h"


using namespace std;


class HGCC2D {

 public:

    HGCC2D();

    void setId(unsigned int id) { _id = id; }
    void setSubdet(int subdet)  { _subdet = subdet; }
    void setPt(float pt)        { _pt = pt; }
    void setEnergy(float energy)  { _energy = energy; }
    void setEta(float eta)      { _eta = eta; }
    void setPhi(float phi)      { _phi = phi; }
    void setZ(float z)          { _z = z; }
    void setLayer(int layer)    { _layer = layer; }
    void setCells(vector<unsigned int> cells)    { _cells = cells; }


    unsigned int id()        { return _id; }
    int   subdet()    { return _subdet; }
    float Pt()        { return _pt; }
    float Energy()    { return _energy; }
    float Eta()       { return _eta; }
    float Phi()       { return _phi; }
    float x()         { return _z*cos(_phi)/sinh(_eta); }
    float y()         { return _z*sin(_phi)/sinh(_eta); }
    float z()         { return _z; }
    float layer()     { return _layer; }
    float ncells()    { return _cells.size(); }
    vector<unsigned int> cells()     { return _cells; }

    int correctedLayer();
    unsigned HGCROCn();
    TVector3 Centre();
    bool isTCcontained( HGCC2D tc );
    TLorentzVector P4();

    void print();

 private:

    unsigned int         _id     ;
    int                  _subdet ;
    float                _pt     ;
    float                _energy ;
    float                _eta    ;
    float                _phi    ;
    float                _z    ;
    int                  _layer  ;
    int                  _ncells ;
    vector<unsigned int> _cells  ; // vector of HGCDetId for all the TC 

};

#endif 
