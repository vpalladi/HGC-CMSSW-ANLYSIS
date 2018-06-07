
#include <HGCht.h>


void HGCht::setZoffset(float zOffset){
    _zOffset = zOffset;
}


void HGCht::setSlopeCorrection(float SC){
    _slopeCorrection = SC;
}


void HGCht::addPoint(const HGChit* hit, double w){

    float x = ( hit->z()+_zOffset )*_slopeCorrection;
    float y = hit->r();
    int  id = hit->id();

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


void HGCht::getXYgraph( TGraph &g ) {
    
    g.Set(0);
    for(vector<pair<float,float>>::iterator xy=_XY.begin(); xy!=_XY.end(); xy++){
        g.SetPoint( g.GetN(), xy->first, xy->second );
    }

}


void HGCht::clear(){

    HGCC3Dbuild::clear();

    _XY.clear();
    _linesCollection.clear();

}

