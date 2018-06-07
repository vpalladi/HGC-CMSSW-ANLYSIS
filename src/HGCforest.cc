
#include <HGCforest.h>

HGCforest::HGCforest() : _normRadius(0.01){

}

HGCforest::HGCforest(unsigned nX, float minX, float maxX,
                     unsigned nY, float minY, float maxY )
: HGCC3Dbuild( nX, minX, maxX, nY, minY, maxY ), _normRadius(0.01) {
    
   _aTree = new ForestSearch::Search< HGCC2D , 13 , 13 >;
   
}


HGCforest::~HGCforest() {
    
    if( _aTree != NULL ) delete _aTree;

}


void HGCforest::setDACbit(int nbits) {
    
    _nbits=nbits;

}


void HGCforest::setNormRadius(double radius) {
    
    _normRadius=radius;

}


void HGCforest::addPoint(const HGCC2D c2d, double w){
    
    float xNorm = c2d.xNorm();
    float yNorm = c2d.yNorm();
    int   id    = c2d.id(); 
    
    // place in the tree
    uint32_t radius = uint32_t ( _normRadius / ( (this->maxX()-this->minX()) / pow(2,_nbits) ) );
    bool startNewC3D = false;

    if( _aTree->_size == 0 ){
        startNewC3D=true;
    }
    else {
        const HGCC2D NN = _aTree->search( c2d );
        if( NN.distanceNormDAC( &c2d, -0.6, 0.6, 13 ) < radius ) {
            this->getC3D( NN.id() )->addC2D( &c2d );
            int icol = ( (xNorm-_minX)/_dX );
            int irow = ( (yNorm-_minY)/_dY);
            if ( w ) _grid[icol][irow].addContent( c2d.Energy(), id );
            else     _grid[icol][irow].addContent( 1, id );
        }
        else {
            startNewC3D=true;
        }
    }

    if( startNewC3D ){
        HGCC3D c3d;
        c3d.addC2D( &c2d );
        this->addNewC3D( c3d );
        _aTree->push( c2d );
    }
        
}


void HGCforest::clear() {

    HGCC3Dbuild::clear();

    if (_aTree != NULL ) delete _aTree;
    _aTree = new ForestSearch::Search< HGCC2D , 13 , 13 >;
    
}



