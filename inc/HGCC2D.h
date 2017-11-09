#ifndef HGCC2D_H
#define HGCC2D_H 1

/* c/c++ */
#include <iostream>
#include <vector>
#include <utility> // pair

/* mylibs */
#include "detId.h"

/* ROOT */
#include "TObject.h"
#include "TMath.h"
#include "TVector3.h"


using namespace std;


class HGCC2D : public TObject {

 public:

    HGCC2D();

    /* set C2D parameters */
    void setId(unsigned id)                  { _id = id; }
    void setSubdet(int subdet)               { _subdet = subdet; }
    void setPt(float pt)                     { _pt = pt; }
    void setEnergy(float energy)             { _energy = energy; }
    void setEta(float eta)                   { _eta = eta; }
    void setPhi(float phi)                   { _phi = phi; }
    void setX(float x)                       { _x = x; }
    void setY(float y)                       { _y = y; }
    void setZ(float z)                       { _z = z; }
    void setLayer(int layer)                 { _layer = layer; }
    void setCells(vector<unsigned> cells)    { _cells = cells; }

    /* get C2D parameters */
    unsigned id()            { return _id; }
    int      subdet()        { return _subdet; }
    float    Pt()            { return _pt; }
    float    Energy()        { return _energy; }
    float    Eta()           { return _eta; }
    float    Phi()           { return _phi; }
    float    x()             { return _x; }
    float    y()             { return _y; }
    float    z()             { return _z; }
    float    layer()         { return _layer; }
    float    ncells()        { return _cells.size(); }
    vector<unsigned> cells() { return _cells; }

    /* the layer is corrected using the subdet and is progressive STARTING form 1 and not 0 */
    int correctedLayer();

    /* HGCROC info*/
    unsigned HGCROCn();
    TVector3 centre();

    /* get useful info */
    bool isTCcontained( HGCC2D tc );
    void getEtaSpan( double &minEta, double &maxEta );
    void getPhiSpan( double &minPhi, double &maxPhi );
    

    /* print to stdout some useful info */
    void print();

 private:

    unsigned int         _id     ;
    int                  _subdet ;
    float                _pt     ;
    float                _energy ;
    float                _eta    ;
    float                _phi    ;
    float                _x      ;
    float                _y      ;
    float                _z      ;
    int                  _layer  ;
    int                  _ncells ;
    vector<unsigned>     _cells  ; // vector of HGCDetId for all the TC 

    ClassDef(HGCC2D, 1)

};

#endif 
