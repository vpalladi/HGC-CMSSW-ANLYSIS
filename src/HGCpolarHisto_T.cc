
#include "HGCpolarHisto_T.h"

template<class T>
HGCpolarHisto<T>::HGCpolarHisto() {

}

template<class T>
HGCpolarHisto<T>::HGCpolarHisto( unsigned rzNbins , double rzMin , double rzMax, 
                                 unsigned phiNbins, double phiMin, double phiMax 
    ){

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


template<class T>
HGCpolarHisto<T>::~HGCpolarHisto() { 
    
    delete _histo;
    delete _histoSums;
    delete _histoMaxima;
    delete _graph;

    for (unsigned i=0; i<_phiNbins ; i++)
        delete[] _grid[i];

    delete[] _grid;
    delete[] _binArea;

}

template<class T>
void HGCpolarHisto<T>::addPoint(const T hit) {

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

template<class T>
TH2D* HGCpolarHisto<T>::getHisto() {

    for (unsigned iphi=0; iphi<_phiNbins; iphi++)
        for (unsigned irz=0; irz<_rzNbins; irz++)
            _histo->SetBinContent( iphi+1, irz+1, _grid[iphi][irz].getContent() );

    return _histo;

}

template<class T>
TH2D* HGCpolarHisto<T>::getHistoSums( unsigned *nBinsToSum ) {
    
    for (unsigned irz=0; irz<_rzNbins; irz++) {
        int nBinsSide = (nBinsToSum[irz]-1)/2;
        
        for (unsigned iphi=0; iphi<_phiNbins; iphi++) {
            
            double content = _grid[iphi][irz].getContent();
            doubel weight  = 1;
            for(int isbin=1; isbin<=nBinsSide; isbin++ ){
                
                int binToSumLeft = iphi;                 
                binToSumLeft = binToSumLeft-isbin; 
                if( binToSumLeft<0 ) binToSumLeft = _phiNbins+binToSumLeft;
                
                int binToSumRight = iphi;   
                binToSumRight = binToSumRight+isbin;
                if( binToSumRight>_phiNbins-1 ) binToSumRight = binToSumRight-_phiNbins;                
                
                content += ( _grid[binToSumLeft][irz].getContent()  / pow( 2, isbin) ); // quadratic kernel
                content += ( _grid[binToSumRight][irz].getContent() / pow( 2, isbin) ); // quadratic kernel
                
                weight += 2*(1/pow( 2, isbin))

            }
            
            double area = _binArea[irz]*weight;

            _histoSums->SetBinContent(iphi+1, irz+1, content/area);
            
        }
    }
    
    TH2D *histoSums = (TH2D*) _histoSums->Clone("histoSumsClone");
    
    for (unsigned irz=1; irz<=_rzNbins; irz++) {
        
        for (unsigned iphi=1; iphi<=_phiNbins; iphi++) {

            double content   = 0.;
            double contentUp = 0.;
            double contentDw = 0.;
            
            content = histoSums->GetBinContent(iphi, irz);
            if( irz < _rzNbins ) contentUp = histoSums->GetBinContent(iphi, irz+1) * .5;
            if( irz > 0 )        contentDw = histoSums->GetBinContent(iphi, irz-1) * .5;
            
            _histoSums->SetBinContent(iphi, irz, content+contentUp+contentDw);

        }
    }

    return _histoSums;

}


template<class T>
TH2D* HGCpolarHisto<T>::getHistoMaxima( unsigned *nBinsToSum ) {

    /* filling _histoSums */
    this->getHistoSums( nBinsToSum );

    /* searching for maxima */
    for (unsigned irz=0; irz<_rzNbins; irz++) {

        double rows[_phiNbins][3];
        
        for (unsigned iphi=0; iphi<_phiNbins; iphi++) {
            
            if ( irz == 0 ) {
                rows[iphi][2] = _histoSums->GetBinContent(iphi+1, irz+2);
                rows[iphi][1] = _histoSums->GetBinContent(iphi+1, irz+1);
                rows[iphi][0] = 0.;
            }
            else if ( irz == _rzNbins-1 ) {
                rows[iphi][2] = 0.;
                rows[iphi][1] = _histoSums->GetBinContent(iphi+1, irz+1);
                rows[iphi][0] = _histoSums->GetBinContent(iphi+1, irz);
            }
            else {
                rows[iphi][2] = _histoSums->GetBinContent(iphi+1, irz+2);
                rows[iphi][1] = _histoSums->GetBinContent(iphi+1, irz+1);
                rows[iphi][0] = _histoSums->GetBinContent(iphi+1, irz);
            }
    
        }

        int nBinsSide = nBinsToSum[irz]-1/2;

        for (unsigned iphi=0; iphi<_phiNbins; iphi++) {
            
            bool isMaxima = true;
            double centralValue = rows[iphi][1];

            if( !( centralValue > rows[iphi][2] ) || !( centralValue >= rows[iphi][0] ) ) 
                isMaxima = false;

            if( isMaxima ) {
                for( int isphi=1; isphi<=nBinsSide; isphi++ ) {
                    
                    int binToSearchLeft = iphi;                 
                    binToSearchLeft = binToSearchLeft-isphi; 
                    if( binToSearchLeft<0 ) binToSearchLeft = _phiNbins+binToSearchLeft;
                    
                    int binToSearchRight = iphi;   
                    binToSearchRight = binToSearchRight+isphi;
                    if( binToSearchRight>_phiNbins-1 ) binToSearchRight = binToSearchRight-_phiNbins;                
                    
                    if( !( centralValue >= rows[binToSearchRight][0] ) ||
                        !( centralValue >= rows[binToSearchRight][1] ) ||
                        !( centralValue >= rows[binToSearchRight][2] ) )
                        isMaxima = false;
                    
                    if( !( centralValue >  rows[binToSearchLeft][0] )  ||
                        !( centralValue >  rows[binToSearchLeft][1] )  ||
                        !( centralValue >  rows[binToSearchLeft][2] )  )
                        isMaxima = false;
                    
                    if( !isMaxima )
                        break;
                }
            }
                        
            if( isMaxima ){
                
                _histoMaxima->SetBinContent(iphi+1, irz+1, rows[iphi][1]);
                
                vector<unsigned> idsBin = _grid[iphi][irz].getIds();
                
                maximaT maxima(0.,0.);
                
                for( auto id : idsBin ){
                    T *hit = &(_hitsMap[id]);
                                       
                    maxima.first = maxima.first + hit->xNorm(); 
                    maxima.second = maxima.second + hit->yNorm();
                    
                }
              
                if( idsBin.size() == 0 ) continue;
                maxima.first = maxima.first / idsBin.size(); 
                maxima.second = maxima.second / idsBin.size(); 

                _maxima.push_back( maxima );

            }

        }

    }

    return _histoMaxima;
    
}


template<class T>
vector<maximaT> HGCpolarHisto<T>::getMaxima( unsigned *nBinsToSum ){

    _maxima.clear();
    
    this->getHistoMaxima( nBinsToSum );
    
    return _maxima;

}


template<class T>
vector<HGCC3D> HGCpolarHisto<T>::getNewC3Ds( double radius, unsigned *nBinsToSum ) {
    
    this->getMaxima( nBinsToSum );

    HGCC3D c3ds[_maxima.size()];

    for(unsigned ihit=0; ihit<_hits.size(); ihit++){
        
        const T* hit = _hits.at( ihit );
        
        unsigned c3dIdToAdd=0;
        double distance=1000;
        unsigned i=0;

       for( auto c3d : c3ds ) {
       
           double dist = sqrt( pow( _maxima.at(i).first-hit->xNorm() , 2 ) + pow( _maxima.at(i).second-hit->yNorm(), 2 ) );
       
           if( distance>dist ) {
               distance = dist; 
               c3dIdToAdd = i;
           }
       
           i++;
       }
        
       /* check if the distance works */
       if( distance<=radius )
           this->addHitToC3D( c3ds, c3dIdToAdd, &(_hitsMap[hit->id()]) );
//           c3ds[c3dIdToAdd].addC2D( _hitsMap[hit->id()] );

    }

    for( auto c3d : c3ds ) {
        if(c3d.nclusters()>0 || c3d.ncells()>0)
            this->addNewC3D( c3d );
    }
    
    return HGCC3DbuildBase<T>::getNewC3Ds();
    
}


template<class T>
void HGCpolarHisto<T>::addHitToC3D( HGCC3D *c3ds, unsigned c3dId, const T *hit ) {
    
    cout << " !!! WARNING !!! HGCpolarHisto<T>::addHitToC3D(HGCC3D *c3ds, unsigned c3dId, const T *hit) is implemented only for Classes HGCC2D and HGCTC. " << endl; 

}


template<>
void HGCpolarHisto<HGCC2D>::addHitToC3D( HGCC3D *c3ds, unsigned c3dId, const HGCC2D *hit ) {
    
    c3ds[c3dId].addC2D( hit );

}


template<>
void HGCpolarHisto<HGCTC>::addHitToC3D( HGCC3D *c3ds, unsigned c3dId, const HGCTC *hit ) {

    c3ds[c3dId].addTC( hit );

}


template<class T>
TGraph* HGCpolarHisto<T>::getGraph() {

    return _graph;

}


template<class T>
void HGCpolarHisto<T>::clear() {

    _histo->Reset();
    _histoSums->Reset();
    _histoMaxima->Reset();
    _maxima.clear();
    _hits.clear();
    _graph->Clear();

     for (unsigned iphi=0; iphi<_phiNbins; iphi++)
         for (unsigned irz=0; irz<_phiNbins; irz++)
             _grid[iphi][irz].clear();

}


template class HGCpolarHisto<HGCC2D>;
template class HGCpolarHisto<HGCTC>;
