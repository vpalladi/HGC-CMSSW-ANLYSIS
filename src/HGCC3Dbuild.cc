
#include <HGCC3Dbuild.h>
#include <HGCbin.h>

HGCC3Dbuild::HGCC3Dbuild() { }


HGCC3Dbuild::HGCC3Dbuild( unsigned nX, float minX, float maxX,
              unsigned nY, float minY, float maxY ){
    
    setParams( nX, minX, maxX,
               nY, minY, maxY 
        );
    
}


HGCC3Dbuild::~HGCC3Dbuild() { 
    
//    cout << "calling destructor" << endl;

    if(_grid!=NULL){
        for( unsigned ix=0; ix<_nX; ix++){
            
            if(_grid[ix]!=NULL)
                delete[] _grid[ix];
        
        }
        
        delete[] _grid;
    }

}


void HGCC3Dbuild::setParams( unsigned nX, float minX, float maxX,
                             unsigned nY, float minY, float maxY ){

    _nX   = nX  ;
    _nY   = nY  ;
       
    _minX = minX; 
    _minY = minY;
       
    _maxX = maxX;
    _maxY = maxY;
        
    _dX = (_maxX-_minX)/_nX;
    _dY = (_maxY-_minY)/_nY;

    _grid = new HGCbin*[_nX];
    for( unsigned ix=0; ix<_nX; ix++){
        _grid[ix] = new HGCbin[_nY];
        for( unsigned iy=0; iy<_nY; iy++) {
            _grid[ix][iy].setCentre( _minX+_dX*ix+_dX/2, _minY+_dY*iy+_dY/2 );
            _grid[ix][iy].setCentreId( ix, iy );
        } 
    }
        
    this->clear();

}


vector<HGCbin*> HGCC3Dbuild::getBinsAboveThr(double thr) const {
    
    vector<HGCbin*> retVec;
    
    for(unsigned icol=0; icol<_nX; icol++){
        for(unsigned irow=0; irow<_nY; irow++){
            if( _grid[icol][irow].isAboveThr( thr ) ){
                retVec.push_back( &_grid[icol][irow] );
            }
        }
    }
    
    return retVec;

    
}


vector<HGCbin*> HGCC3Dbuild::getBinsLocalMaxima(double thr, unsigned nNeighboursSearch, unsigned nNeighboursSum) const {
    
    unsigned nNN = nNeighboursSum;
    unsigned sNN = (nNeighboursSearch-1)/2;

    vector<HGCbin*> maximaBins;
    
    for(unsigned icol=1; icol<_nX-1; icol++){
        for(unsigned irow=1; irow<_nY-1; irow++){
            
            if( !_grid[icol][irow].isAboveThr( thr ) ) continue;

            unsigned minCol = (icol-sNN)<0       ? 0       : (icol-sNN);
            unsigned maxCol = (icol+sNN)>(_nX-1) ? (_nX-1) : (icol+sNN);
            unsigned minRow = (irow-sNN)<0       ? 0       : (irow-sNN);
            unsigned maxRow = (irow+sNN)>(_nX-1) ? (_nX-1) : (irow+sNN);

            bool isMaxima=true;
            for(unsigned icolNN=minCol; icolNN<maxCol; icolNN++){
                for(unsigned irowNN=minRow; irowNN<maxRow; irowNN++){
                    
                    if( (irowNN==irow) && (icolNN==icol) ) continue;

                    if( !isMaxima ) break;
                   
                    if( (icolNN+irowNN) >= 0 ){
                        if( !( _grid[icol][irow].getContent() > _grid[icolNN][irowNN].getContent() ) )
                            isMaxima = false;
                    }
                    else {
                        if( !( _grid[icol][irow].getContent() >= _grid[icolNN][irowNN].getContent() ) )
                            isMaxima = false;
                    }
                    
                }
                if( !isMaxima ) break;
                    
            }

            /* add to the vector if is a local maximum */
            if( isMaxima ) {
                /* add the maximum bin */
                if( _grid[icol][irow].getIds().size() == 0  ) continue;

                maximaBins.push_back( &_grid[icol][irow] );
                
                /* add NN */
                if( nNN==0 ) continue;
            
                unsigned minColNN = (icol-nNN)<0       ? 0       : (icol-nNN);
                unsigned maxColNN = (icol+nNN)>(_nX-1) ? (_nX-1) : (icol+nNN);
                unsigned minRowNN = (irow-nNN)<0       ? 0       : (irow-nNN);
                unsigned maxRowNN = (irow+nNN)>(_nX-1) ? (_nX-1) : (irow+nNN);
            
                for(unsigned icolNN=minColNN; icolNN<maxColNN; icolNN++){
                    for(unsigned irowNN=minRowNN; irowNN<maxRowNN; irowNN++){
            
                        if( (irowNN==irow) && (icolNN==icol) ) continue;
                        
                        maximaBins.push_back( &_grid[icolNN][irowNN] );
                        
                    }
                }

            }
        }
    }
    
    return maximaBins;
    
}


void HGCC3Dbuild::getHistoLocalMaxima(TH2D &histo, double thr, unsigned nNeighboursSearch, unsigned nNeighboursSum ) const {
        
    histo.SetBins(_nX, _minX, _maxX,
                  _nY, _minY, _maxY
        );
    
    /* get the maxima */
    vector<HGCbin*> bins = this->getBinsLocalMaxima( thr, nNeighboursSearch, nNeighboursSum );

    /* build the histo */
    for(unsigned ibin=0; ibin<bins.size(); ibin++){
        
        histo.SetBinContent( bins.at(ibin)->getCentreId().first+1,
                             bins.at(ibin)->getCentreId().second+1, 
                             bins.at(ibin)->getContent() 
            );

    }

}

void HGCC3Dbuild::getTransformedHisto(TH2D &histo, double thr ) const {
        
    histo.SetBins(_nX, _minX, _maxX,
                  _nY, _minY, _maxY
        );

    vector<HGCbin*> bins = this->getBinsAboveThr( thr );

    for(unsigned ibin=0; ibin<bins.size(); ibin++){
        
        histo.SetBinContent( bins.at(ibin)->getCentreId().first+1, 
                             bins.at(ibin)->getCentreId().second+1, 
                             bins.at(ibin)->getContent() 
            );

    }

}


int   HGCC3Dbuild::nX()   const { return _nX; } 
int   HGCC3Dbuild::nY()   const { return _nY; }
float HGCC3Dbuild::minX() const { return _minX; }
float HGCC3Dbuild::maxX() const { return _maxX; }
float HGCC3Dbuild::minY() const { return _minY; }
float HGCC3Dbuild::maxY() const { return _maxY; }


void HGCC3Dbuild::addNewC3D(HGCC3D c3d) {

    _newC3Ds[c3d.id()] = c3d; 
    _newC3DsVec.push_back( &_newC3Ds[c3d.id()] );

}


HGCC3D* HGCC3Dbuild::getC3D(unsigned id) {

    return &_newC3Ds[id];

}


vector<HGCC3D> HGCC3Dbuild::getNewC3Ds() {
    
    vector<HGCC3D> vec;

    for( int i=0; i<_newC3DsVec.size(); i++ ) {
        vec.push_back( *_newC3DsVec.at(i) );
    }

    return vec;

}

void HGCC3Dbuild::clear(){
        
    _newC3DsVec.clear();
    _newC3Ds.clear();

    for(unsigned icol=0; icol<_nX; icol++){
        for(unsigned irow=0; irow<_nY; irow++){
            _grid[icol][irow].clear();
        }
    }
        
}

