#ifndef HGCC3D_H
#define HGCC3D_H 1

/* c/c++ */
#include <iostream>
#include <map>     // needed to correctly genreate dictionary
#include <vector>

/* mylibs */
#include "detId.h"

/* ROOT */
#include "TObject.h"
#include "TLorentzVector.h"

using namespace std;


class HGCC3D : public TObject {

 public:

    HGCC3D();
    ~HGCC3D();

    /* set methods*/
    void setId(unsigned id)                     ;
    void setPt(float pt)                        ;
    void setEnergy(float energy)                ;
    void setEta(float eta)                      ;
    void setPhi(float phi)                      ;
    void setClusters(vector<unsigned> clusters) ;
    void setCells(vector<unsigned> cells)       ;

    /* get methods */
    unsigned id()  ;
    float Pt()     ;
    float Energy() ;
    float Eta()    ;
    float Phi()    ;

    unsigned         nclusters() ;
    vector<unsigned> clusters()  ;

    unsigned         ncells()    ;
    vector<unsigned> cells()     ;

    TLorentzVector P4();

    void print();

 private:

    unsigned         _id       ;
    float            _pt       ;
    float            _energy   ;
    float            _eta      ;
    float            _phi      ;
    vector<unsigned> _clusters ; // vector of HGCDetId for all the C2D
    vector<unsigned> _cells    ; // vector of HGCDetId for all the TC

    ClassDef( HGCC3D,1 )

};

#endif 
