
#include <HGCsingleC3D.h>

HGCsingleC3D::HGCsingleC3D() {

}


HGCsingleC3D::~HGCsingleC3D() { 

}

//void HGCsingleC3D::addNewC3D(HGCC3D c3d) {
//
//    _newC3Ds[c3d.id()] = c3d; 
//    _newC3DsVec.push_back( &_newC3Ds[c3d.id()] );
//
//}


void HGCsingleC3D::addPoint(const HGCC2D c2d){

    if( this->getNewC3Ds().size() == 0 ){
        HGCC3D c3d;
        c3d.addC2D( &c2d );
        this->addNewC3D( c3d );
    }
    else {
        unsigned id = this->getNewC3Ds().at(0).id();
        this->getC3D( id )->addC2D( &c2d );
    }
        
}


//HGCC3D* HGCsingleC3D::getC3D(unsigned id) {
//
//    return &_newC3Ds[id];
//
//}
//
//
//vector<HGCC3D> HGCsingleC3D::getNewC3Ds() {
//    
//    vector<HGCC3D> vec;
//
//    for( int i=0; i<_newC3DsVec.size(); i++ ) {
//        vec.push_back( *_newC3DsVec.at(i) );
//    }
//
//    return vec;
//
//}


void HGCsingleC3D::clear() {

    HGCC3DbuildBase<HGCC2D>::clear();

//    _newC3DsVec.clear();
//    _newC3Ds.clear();

}



