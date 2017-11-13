#ifndef HGCC3D_H
#define HGCC3D_H 1

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


class HGCC3D {

 public:

    HGCC3D();

    void setId(unsigned int id) { _id = id; }   
    void setPt(float pt)        { _pt = pt; }
    void setEnergy(float energy)  { _energy = energy; }
    void setEta(float eta)      { _eta = eta; }
    void setPhi(float phi)      { _phi = phi; }
    void setClusters(vector<unsigned int> clusters)    { _clusters = clusters; }
    void setCells(vector<unsigned int> cells)    { _cells = cells; }

    unsigned int id()        { return _id; }   
    float Pt()        { return _pt; }
    float Energy()    { return _energy; }
    float Eta()       { return _eta; }
    float Phi()       { return _phi; }

    unsigned int nclusters()    { return _clusters.size(); }
    vector<unsigned int> clusters()     { return _clusters; }

    unsigned int ncells()        { return _cells.size(); }
    vector<unsigned int> cells() { return _cells; }

    TLorentzVector P4();

    void print();

 private:

    unsigned int         _id     ;
    float                _pt     ;
    float                _energy ;
    float                _eta    ;
    float                _phi    ;
    vector<unsigned int> _clusters  ; // vector of HGCDetId for all the C2D 
    vector<unsigned int> _cells  ; // vector of HGCDetId for all the TC

};

#endif 
