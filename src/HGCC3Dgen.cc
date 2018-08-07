
#include <HGCC3Dgen.h>

HGCC3Dgen::HGCC3Dgen() {

}


HGCC3Dgen::~HGCC3Dgen() { 

}

void HGCC3Dgen::addPoint(const HGCC2D c2d){

}


void HGCC3Dgen::addPoint(const HGCC2D c2d, vector<HGCgen*> gens, double radius){

    for( vector<HGCgen*>::iterator gen=gens.begin(); gen!=gens.end(); gen++ ) {
        
        if( c2d.getEndcapId()!=(*gen)->getEndcapId() )
            continue;
        
        double c2dXnorm = c2d.xNorm();
        double c2dYnorm = c2d.yNorm();
        double genXnorm = (*gen)->xNorm();
        double genYnorm = (*gen)->yNorm();

        double dist =  TMath::Sqrt( (c2dXnorm-genXnorm)*(c2dXnorm-genXnorm)+(c2dYnorm-genYnorm)*(c2dYnorm-genYnorm) );

        if (dist > radius)
            continue;

        /* new c3d or already existing one */
        if( this->getNewC3Ds().size() == 0          || 
            _genC3D.find( (*gen)->id() ) == _genC3D.end() ) {

            HGCC3D c3d;
            
            c3d.addC2D( &c2d );
            
            this->addNewC3D( c3d );
            _genC3D[(*gen)->id()] = c3d.id();
            
        }
        else {
            unsigned c3dId = _genC3D[(*gen)->id()];
            this->getC3D( c3dId )->addC2D( &c2d );
        }   

    }

}


void HGCC3Dgen::clear() {

    HGCC3DbuildBase<HGCC2D>::clear();
    _genC3D.clear();

}



