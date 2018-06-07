
#include <HGCnorm.h>
#include <HGCgen.h>
#include "Math/KDTree.h"
#include "TMath.h"

HGCnorm::HGCnorm() {

}

HGCnorm::HGCnorm(unsigned nX, float minX, float maxX,
                 unsigned nY, float minY, float maxY )
    : HGCC3Dbuild( nX, minX, maxX, nY, minY, maxY ) {
    
}


HGCnorm::~HGCnorm() {

}


void HGCnorm::addPoint(const HGCC2D* c2d, double w) {
    
    float xNorm = c2d->xNorm();
    float yNorm = c2d->yNorm();
    int   id    = c2d->id(); 
    
    int icol = ( (xNorm-_minX)/_dX );
    int irow = ( (yNorm-_minY)/_dY );

    if( icol>=0 && icol<_nX && irow>=0 && irow<_nY ) {
        
        //cout << "c2id " << id << endl;
        //cout << "grid " << _grid[icol][irow].getCentre().first << " " << _grid[icol][irow].getCentre().second << endl
        //     << "c2d  " << xNorm << " " << yNorm << endl;
        
        float centreC2Ddistance = TMath::Sqrt( (xNorm-_grid[icol][irow].getCentre().first)*(xNorm-_grid[icol][irow].getCentre().first) + (yNorm-_grid[icol][irow].getCentre().second)*(yNorm-_grid[icol][irow].getCentre().second) ) ;
        //cout << "dist " << centreC2Ddistance << endl << endl;
        if ( w ) _grid[icol][irow].addContent( c2d->Energy(), id );
        else     _grid[icol][irow].addContent( 1, id );
        
    }
    else {

        cout << " HGCnorm >>> !!! Warning !!! Col (" << icol << ") or row (" << irow << ") out of range" << endl;
        cout << " c2dId    " << c2d->id() << " "
             << " c2dx     " << c2d->x() << " "
             << " c2dy     " << c2d->y() << " "
             << " c2dxNorm " << c2d->xNorm() << " "
             << endl;

    }

    _C2Ds[c2d->id()] = *c2d;
    _vecC2Ds.push_back( &_C2Ds[c2d->id()] );

}


//void HGCnorm::buildNewC3Ds( unsigned nNeighboursSearch, unsigned nNeighboursSum, vector<HGCgen*> gens ) {
//
//    vector<HGCbin*> binsLocalMaxima = this->getBinsLocalMaxima(0., nNeighboursSearch, nNeighboursSum );
//
//    /* iterate all the maximum */
//    for( std::vector<HGCbin*>::iterator bin=binsLocalMaxima.begin(); bin!=binsLocalMaxima.end(); bin++ ) {
//        HGCC3D c3d;
//            
//        vector<unsigned> ids = (*bin)->getIds();
//
//        if( ids.size() == 0 ) continue;
//        /* iterate the c2d in the maximum bins */
//        for( vector<unsigned>::iterator id=ids.begin(); id!=ids.end(); id++ ){
//            
//            HGCC2D c2d = _C2Ds[*id] ;
//            c3d.addC2D( &c2d );
//            
//        }
//
//        /* search for the closest gen */
//        if( gens.size()>0 ){
//
//            double dmin=10.; // norm. coo.
//            unsigned minGenId=0;
//            
//            for( unsigned igen=0; igen<gens.size(); igen++ ) {
//                
//                ROOT::Math::RhoEtaPhiPoint genProj =  gens.at(igen)->getZprojection( HGCgeom::instance()->layerZ( gens.at(igen)->getEndcapId(), 1) );
//                double XnormDiff = genProj.X()/genProj.Z() - c3d.xNorm();
//                double YnormDiff = genProj.Y()/genProj.Z() - c3d.yNorm();
//                double d = TMath::Sqrt( XnormDiff*XnormDiff + YnormDiff*YnormDiff );
//
//                if( d<dmin ){
//                    minGenId=igen;
//                    dmin=d;
//                }
//
//            }
//
//            /* set the nearest gen */            
//            c3d.setNearestGen( *gens.at(minGenId) );
//            
//        }
//
//        this->addNewC3D( c3d );
//
//    }
//
//}

void HGCnorm::buildNewC3Ds( unsigned nNeighboursSearch, double radius, vector<HGCgen*> gens ) {

    vector<HGCbin*> binsLocalMaxima = this->getBinsLocalMaxima(0., nNeighboursSearch, 0 );
    
    
    /* iterate all the maxima */
    for( std::vector<HGCbin*>::iterator bin=binsLocalMaxima.begin(); bin!=binsLocalMaxima.end(); bin++ ) {
        
        vector<unsigned> idsM = (*bin)->getIds();
        //cout << "bins id (" << idsM.size() << ")" << endl;
        for(vector<unsigned>::iterator it=idsM.begin(); it!=idsM.end(); it++){
//            cout << *it << " ";
            HGCC2D c2d = _C2Ds[*it];
            float centreC2Ddistance = TMath::Sqrt( (c2d.xNorm()-(*bin)->getCentre().first)*(c2d.xNorm()-(*bin)->getCentre().first) + (c2d.yNorm()-(*bin)->getCentre().second)*(c2d.yNorm()-(*bin)->getCentre().second) );
            //cout << c2d.xNorm() << " " << c2d.yNorm() << endl; 
            //cout << (*bin)->getCentre().first << " - " << (*bin)->getCentre().second
            //     << " distance " << centreC2Ddistance << endl;

        }

//        cout << "c2d in c3d id" << endl;        
        HGCC3D c3d;
        
        vector<unsigned> ids = (*bin)->getIds();
        pair<float,float> binCentre = (*bin)->getCentre();
        
        /* iterate the c2d in the maximum bins */
        for( unsigned ic2d=0; ic2d<_vecC2Ds.size(); ic2d++ ){
            
            HGCC2D* c2d = _vecC2Ds[ic2d] ;
            float centreC2Ddistance = TMath::Sqrt( (c2d->xNorm()-binCentre.first)*(c2d->xNorm()-binCentre.first) + (c2d->yNorm()-binCentre.second)*(c2d->yNorm()-binCentre.second) );

            if( centreC2Ddistance<radius ){
                c3d.addC2D( c2d );
//                cout << c2d->id() << endl;
            }
            
        }

        if( c3d.nclusters() == 0 ) continue;

        /* search for the closest gen */
//        if( gens.size()>0 ){
//
//            double dmin=10.; // norm. coo.
//            unsigned minGenId=0;
//            
//            for( unsigned igen=0; igen<gens.size(); igen++ ) {
//                
//                double XnormDiff = gens.at(igen).xNorm() - c3d.xNorm();
//                double YnormDiff = gens.at(igen).yNorm() - c3d.yNorm();
//                double d = TMath::Sqrt( XnormDiff*XnormDiff + YnormDiff*YnormDiff );
//
//                if( d<dmin ){
//                    minGenId=igen;
//                    dmin=d;
//                }
//
//            }
//
//            /* set the nearest gen */            
//            c3d.setNearestGen( *gens.at(minGenId) );
//          
//        }

        this->addNewC3D( c3d );

    }

}


vector<HGCC3D>  HGCnorm::getNewC3Ds( unsigned nNeighboursSearch, double radius, vector<HGCgen*> gens ) {

    this->buildNewC3Ds( nNeighboursSearch, radius, gens );
    
    return HGCC3Dbuild::getNewC3Ds();

}


void HGCnorm::clear() {

    _C2Ds.clear();

    HGCC3Dbuild::clear();

}
