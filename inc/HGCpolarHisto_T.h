#ifndef HGC_POLAR_HISTO_T
#define HGC_POLAR_HISTO_T

#include <iostream>
#include <vector>
#include <utility>
#include <map>

#include <math.h>

#include "HGCbin.h"
#include "HGCC2D.h"
#include "HGCC3DbuildBase.h"

#include "TH2D.h"

typedef pair<double,double> maximaT;

template<class T>
class HGCpolarHisto : public HGCC3DbuildBase<T> {

public:
    
    HGCpolarHisto();
    HGCpolarHisto( unsigned rzNbins , double rzMin , double rzMax , 
                   unsigned phiNbins, double phiMin, double phiMax );

    ~HGCpolarHisto();
    
    void addPoint(const T hit);

    TH2D*   getHisto();
    TH2D*   getHistoSums( unsigned *nBinsToSum ); // WARNING: must be the number of bins to sum per row it is supposed to be an odd number
    TH2D*   getHistoMaxima( unsigned *nBinsToSum ); // WARNING: must be the number of bins to sum per row it is supposed to be an odd number

    TGraph* getGraph();

    vector<maximaT> getMaxima( unsigned *nBinsToSum );

    vector<HGCC3D> getNewC3Ds( double radius, unsigned *nBinsToSum );

    void clear();

private:

    void addHitToC3D( HGCC3D *c3ds, unsigned c3dId, const T *hit );

    void buildNewC3Ds( HGCsubdet *sdet );    
    map<unsigned,T> _hitsMap;

    vector<T*> _hits;

    unsigned _rzNbins, _phiNbins;

    double _rzMin, _rzMax;
    double _phiMin, _phiMax;
    double _phiBinWidth, _rzBinWidth;
    double *_binArea;

    vector<maximaT> _maxima;

    TH2D* _histo;
    TH2D* _histoSums;
    TH2D* _histoMaxima;
    TGraph* _graph; 

    HGCbin **_grid;

};


#endif



