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
                   unsigned phiNbins, double phiMin, double phiMax ) {

        _rzNbins = rzNbins; 
        _phiNbins = phiNbins; 
        
        _rzMin = rzMin; 
        _phiMin = phiMin; 
        _rzMax = rzMax; 
        _phiMax = phiMax; 
        
        _rzBinWidth  = (_rzMax-_rzMin) / _rzNbins; 
        _phiBinWidth = (_phiMax-_phiMin) / _phiNbins; 
        
        _histo = new TH2D("histo", "histo", 
                          _phiNbins, _phiMin, _phiMax,
                          _rzNbins , _rzMin , _rzMax
            );
        
        _histoSums = new TH2D("histoSums", "histoSums", 
                              _phiNbins, _phiMin, _phiMax,
                              _rzNbins , _rzMin , _rzMax
            );
        
        _histoMaxima = new TH2D("histoMaxima", "histoMaxima", 
                                _phiNbins, _phiMin, _phiMax,
                                _rzNbins , _rzMin , _rzMax
            );
        
        _graph = new TGraph();
        
        _grid = new HGCbin*[_phiNbins];
        
        for (unsigned i=0; i<_phiNbins ; i++)
            _grid[i] = new HGCbin[_rzNbins];
        
        _binArea = new double[_rzNbins];
        
        
        double dR = (_rzMax-_rzMin)/_rzNbins;
        for(unsigned i=0; i<_rzNbins; i++){
            double r1 = _rzMin+ i   *dR;
            double r2 = _rzMin+(i+1)*dR;
            _binArea[i] = ( (phiMax-phiMin)/(phiNbins*2.) ) * ( pow(r2,2) - pow(r1,2) )  ;
        }
    }
    
    ~HGCpolarHisto() {
     
        delete _histo;
        delete _histoSums;
        delete _histoMaxima;
        delete _graph;
        
        for (unsigned i=0; i<_phiNbins ; i++)
            delete[] _grid[i];
        
        delete[] _grid;
        delete[] _binArea;
    }
    
    void addPoint(const T hit) {
        double phi = hit.Phi();
        double r   = hit.r();
        double z   = hit.z();
        double rz  = r/abs(z);
        
        unsigned rzBinId  = floor( ( rz - _rzMin ) / _rzBinWidth );
        unsigned phiBinId = floor( ( phi - _phiMin ) / _phiBinWidth );
    
//    cout << "r/z  "   << r/z     << " - p " << phi << endl;
//    cout << "br/z "   << rzBinId << " - p " << phiBinId << endl;
//    cout << phiBinId  << " "     << rzBinId << endl;
//
        if( phiBinId < _phiNbins && rzBinId < _rzNbins){
            //_histo->Fill( phi, rz, hit.Energy());
            _grid[phiBinId][rzBinId].addContent( hit.Energy(), hit.id() );
        }
        else
            cout << " >>> HGCpolarHisto: bin Id out of range." << endl;
        _graph->SetPoint( _graph->GetN(), phi, rz);
        
        _hitsMap[hit.id()] = hit;
        _hits.push_back( &(_hitsMap[hit.id()]) );
        
    }
    
    TH2D*   getHisto(bool weighted=true) {
        for (unsigned iphi=0; iphi<_phiNbins; iphi++) {
            for (unsigned irz=0; irz<_rzNbins; irz++) {
                float w = weighted ? _grid[iphi][irz].getContent() : 1 ;
                _histo->SetBinContent( iphi+1, irz+1, w );
            }
        }
        return _histo;
    }
    TH2D*   getHistoSums( unsigned *nBinsToSum ); // WARNING: must be the number of bins to sum per row it is supposed to be an odd number
    TH2D*   getHistoMaxima( unsigned *nBinsToSum ); // WARNING: must be the number of bins to sum per row it is supposed to be an odd number
    
    TGraph* getGraph();

    vector<maximaT> getMaxima( unsigned *nBinsToSum ); // in x/z and y/z

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



