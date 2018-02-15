#ifndef HGCCONE_H
#define HGCCONE_H

/* c++ */
#include <iostream>

/* ROOT */
#include "TMath.h"

class HGCCone{

public:
    HGCCone(double aperture, double theta=0, double phi=0);
    
    bool isContained(double x, double y, double z);

private:
    
    double aperture_;
    double theta_, phi_;
    
    double cone(double x, double y, double z);

};

#endif

