
#ifndef HGCBIN_H
#define HGCBIN_H

/* C++ */
#include <iostream>
#include <vector>
#include <utility>

/* ROOT */
#include <TGraph.h>

/* myLibs */
//#include "HGCsubdet.h"

using namespace std;

class HGCsubdet;

class HGCbin{

public:

    HGCbin();
    HGCbin(float x, float y);
    
    ~HGCbin();
    
    void setCentre(float x, float y);
    
    void setCentreId(int xBin, int yBin); 
    void addContent( float content, unsigned id ); 
    
    void clear();
    
    pair<float,float> getCentre()   ;
    pair<int,int>     getCentreId() ;
    vector<unsigned>  getIds()      ;
    float             getContent()  ;
    
    void getLongitudinalEnergyProfile(HGCsubdet *det, TGraph &g);
    
    void getLongitudinalPtProfile(HGCsubdet *det, TGraph &g);

    bool isAboveThr(double thr);

private:

    float _content;
    vector<unsigned> _ids;
    pair<float,float> _centre;   /* first=x; second=y */
    pair<int,int>     _centreId; /* first=x; second=y */

};

#endif
