
#include <HGCht.h>

HGCht::HGCht() { ; }
/*
  X, Y = transformed variables 
*/
HGCht::HGCht( unsigned nX, float minX, float maxX,
              unsigned nY, float minY, float maxY ){

    setParams( nX, minX, maxX,
               nY, minY, maxY );
        
}

void HGCht::setParams( unsigned nX, float minX, float maxX,
                       unsigned nY, float minY, float maxY ){

    _nX   = nX  ;
    _nY   = nY  ;
       
    _minX = minX; 
    _minY = minY;
       
    _maxX = maxX;
    _maxY = maxY;
        
    _dX = (_maxX-_minX)/_nX;
    _dY = (_maxY-_minY)/_nY;

    _grid = new HGChoughBin*[_nX];
    for( unsigned ix=0; ix<_nX; ix++){
        _grid[ix] = new HGChoughBin[_nY];
        for( unsigned iy=0; iy<_nY; iy++) {
            _grid[ix][iy].setCentre( _minX+_dX*ix-_dX/2, _minY+_dY*iy-_dY/2 );
            _grid[ix][iy].setCentreId( ix, iy );
        } 
    }
        
    this->clear();

}

void HGCht::addPointPhysicalSpace(float x, float y, int id, double w){

    pair<float,float> XY(x, y);
    _XY.push_back(XY);

    HGCline line(-x, y);
    _linesCollection.push_back( line );
        
    for(unsigned icol=0; icol<_nX; icol++){

        float workingPointX     = _minX + _dX*icol;
        float workingPointY     = line.eval( workingPointX );
        float workingPointYnext = line.eval( (workingPointX+_dX) );

        if( workingPointY < _minY || workingPointY > _maxY ) continue;

        int irowStart = floor( ( (workingPointY-_minY)/_dY ));
        int irowEnd   = floor( ( (workingPointYnext-_minY)/_dY ));

        /* adding the content to the correct bin */
        if( irowStart == irowEnd ) {
            _grid[icol][irowStart].addContent( w, id );
        }
        else {
            for( int irow=irowStart; irow!=irowEnd; ) {                    
                    
                if( irow >= 0 && irow < (int)_nY ) {
                    _grid[icol][irow].addContent( w, id );
                }

                if( irowStart < irowEnd ) 
                    irow++;
                else
                    irow--;

            }
                
            if( irowEnd >= 0 && irowEnd < _nY ){
                _grid[icol][irowEnd].addContent( w, id );
            }

        }

    }

}


vector<TF1> HGCht::getTF1s(float min, float max){
        
    vector<TF1> funcs;
    for(unsigned ifunc=0; ifunc<_linesCollection.size(); ifunc++){
        funcs.push_back( _linesCollection.at(ifunc).getTF1(min, max) );
    }
        
    return funcs;

}
    

vector<HGChoughBin> HGCht::getBinsAboveThr(double thr){
    
    vector<HGChoughBin> retVec;
    
    for(unsigned icol=0; icol<_nX; icol++){
        for(unsigned irow=0; irow<_nY; irow++){
            if( _grid[icol][irow].isAboveThr( thr ) ){
                retVec.push_back( _grid[icol][irow] );
            }
        }
    }
    
    return retVec;
    
}


vector<HGChoughBin> HGCht::getBinsLocalMaxima(double thr){
    
    vector<HGChoughBin> retVec;
    
//    for(unsigned icol=1; icol<_nX-1; icol++){
//        for(unsigned irow=1; irow<_nY-1; irow++){
//            bool isMaxima=false; // scan the grid!
//            if( !_grid[icol][irow].isAboveThr( thr ) ) continue;
//            if( _grid[icol][irow].getContent() > _grid[icol-1][irow-1].getContent() &&
//                _grid[icol][irow].getContent() > _grid[icol-1][irow]  .getContent() &&
//                _grid[icol][irow].getContent() > _grid[icol-1][irow+1].getContent() &&
//                _grid[icol][irow].getContent() > _grid[icol][irow-1]  .getContent() &&
//                _grid[icol][irow].getContent() > _grid[icol][irow+1]  .getContent() &&
//                _grid[icol][irow].getContent() > _grid[icol+1][irow-1].getContent() &&
//                _grid[icol][irow].getContent() > _grid[icol+1][irow]  .getContent() &&
//                _grid[icol][irow].getContent() > _grid[icol+1][irow+1].getContent()
//                ) {
//                retVec.push_back( _grid[icol][irow] );
//            }
//        }
//    }
    
    return retVec;
    
}


TH2D* HGCht::getTransformedHisto(TString name ){
        
    TH2D* histo = new TH2D(name, name, 
                           _nX, _minX, _maxX,
                           _nY, _minY, _maxY
        );

    for(unsigned icol=0; icol<_nX; icol++){
        for(unsigned irow=0; irow<_nY; irow++){
            histo->SetBinContent( _grid[icol][irow].getCentreId().first+1, 
                                  _grid[icol][irow].getCentreId().second+1, 
                                  _grid[icol][irow].getContent() 
                );
        }
    }
        
    return histo;

}


TH2D* HGCht::getTransformedHistoThr(TString name, double thr ){
        
    TH2D* histo = new TH2D(name, name, 
                           _nX, _minX, _maxX,
                           _nY, _minY, _maxY
        );

    vector<HGChoughBin> bins = this->getBinsAboveThr( thr );

    for(int ibin=0; ibin<bins.size(); ibin++){
        
        histo->SetBinContent( bins.at(ibin).getCentreId().first+1, 
                              bins.at(ibin).getCentreId().second+1, 
                              bins.at(ibin).getContent() 
            );

    }
    

    return histo;

}


TGraph HGCht::getXYgraph( TString name ) {
        
    TGraph g;
    g.SetName( name );
    for(vector<pair<float,float>>::iterator xy=_XY.begin(); xy!=_XY.end(); xy++){
        g.SetPoint( g.GetN(), xy->first, xy->second );
    }
        
    return g;

}


void HGCht::clear(){
        
    _XY.clear();
        
    for(unsigned icol=0; icol<_nX; icol++){
        for(unsigned irow=0; irow<_nY; irow++){
            _grid[icol][irow].clear();
        }
    }
        
}

