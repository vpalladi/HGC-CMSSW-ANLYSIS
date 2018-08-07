
/* mylibs */
#include "HGCsubdet.h"
#include "HGCgen.h"

HGCsubdet::HGCsubdet( unsigned endcapId, unsigned sectionId, int verboselevel ) {
    
    /* init */
    _endcapId = endcapId;
    _sectionId  = sectionId;
    _verboselevel = verboselevel;
    
    /* Layers Z */
    for(unsigned ilayer; ilayer<HGCgeom::instance()->nLayers(); ilayer++){
        _layerZ.push_back( HGCgeom::instance()->layerZ(endcapId, ilayer) );
    }

    /* allocate the containers for the TC C2D and ROC */
    _TC_layer  = new vector<const HGCTC*>[this->nLayers()];
    _C2D_layer = new vector<const HGCC2D*>[this->nLayers()];
    _TD        = new vector<HGCROC>*[this->nLayers()];
    for(unsigned i=0; i<this->nLayers(); i++) {
        _TD[i]  = new vector<HGCROC> ;
        _TD[i] ->reserve(5000);
    }

    /* save the new c3d into the tree */
//    const char* treeName= Form("subdet_endcap%d_section%d",_endcapId, _sectionId);
//    _tree = new TTree( treeName, treeName );
//    _tree->Branch( "c3d_forest"   , &_vC3Dforest, 64000, 1 );
//    _tree->Branch( "c3d"     , &_vC3D  , 64000, 1 );

}


HGCsubdet::~HGCsubdet() {
    
    if( _TC_layer  != NULL ) delete[] _TC_layer  ;
    if( _C2D_layer != NULL ) delete[] _C2D_layer ;
    if( _TD        != NULL ) delete[] _TD        ;
    
    for(unsigned i=0; i<this->nLayers(); i++) {
        if( _TD[i] != NULL ) delete _TD[i];
    }
    
//    if( _tree      != NULL ) delete _tree        ;

};

template<>
void HGCsubdet::add( HGCTC tc ) { 

    /* add tc to the map*/
    _TCs[tc.id()] = tc;
    
    /* keep the pointer for easy access */
    _TCvec.push_back( &_TCs[tc.id()] );
        
}


void HGCsubdet::addGen(HGCgen gen){

    _gen[gen.id()] = gen;
    _genVec.push_back( &_gen[gen.id()] );

}


template<>
void HGCsubdet::add( HGCC2D c2d ) { 

    /* add c2d to the map */
    _C2Ds[c2d.id()] = c2d;
    
    /* keep the pointer for easy access */
    _C2Dvec.push_back( &_C2Ds[c2d.id()] );

}


template<>
void HGCsubdet::add(HGCC3D c3d)      {   

    /* add c2d to the map */
    _C3Ds[c3d.id()] = c3d;

    /* keep the pointer for easy access */
    _C3Dvec.push_back( &_C3Ds[c3d.id()] );

}


/* get objects by id */
template<> int HGCsubdet::get(unsigned ID, HGCTC &tc) { 
    if( _TCs.count(ID) == 0 ) {
        std::cout << " HGCsubdet >>> !!! WARNING !!! endcap " << _endcapId << ", subdet ID " << _sectionId << ", for TC  (" << ID << ") not found in map." << std::endl;
        return -1;
    }
    tc = _TCs[ID];
    return  0; 
}


template<> int HGCsubdet::get(unsigned ID, HGCC2D &c2d) { 
    if( _C2Ds.count(ID) == 0 ) {
        std::cout << " HGCsubdet >>> !!! WARNING !!! endcap " << _endcapId << ", subdet ID " << _sectionId << ", for C2D (" << ID << ") not found in map." << std::endl;
        return -1;
    }
    c2d = _C2Ds[ID];
    return  0; 
}

template<> int HGCsubdet::get(unsigned ID, HGCC3D &c3d) { 
    if( _C3Ds.count(ID) == 0 ) {
        std::cout << " HGCsubdet >>> !!! WARNING !!! endcap " << _endcapId << ", subdet ID " << _sectionId << ", for C3D (" << ID << ") not found in map." << std::endl;
        return -1;
    }
    c3d = _C3Ds[ID];
    return  0; 
}

template<> map<unsigned,HGCTC>  *HGCsubdet::getMap() { return &_TCs  ; }
template<> map<unsigned,HGCC2D> *HGCsubdet::getMap() { return &_C2Ds ; }
template<> map<unsigned,HGCC3D> *HGCsubdet::getMap() { return &_C3Ds ; }

template<> vector<const HGCTC* > HGCsubdet::getAll() { return _TCvec ; }
template<> vector<const HGCC2D*> HGCsubdet::getAll() { return _C2Dvec; }
template<> vector<const HGCC3D*> HGCsubdet::getAll() { return _C3Dvec; }
vector<HGCgen*> HGCsubdet::getGenAll()               { return _genVec; }

void HGCsubdet::getTDall( vector<HGCROC> &data ) {

    for(unsigned i=0; i<this->nLayers(); i++)
        data.insert(data.end(), _TD[i]->begin(), _TD[i]->end() );

}


//in .h    template<class T>
//in .h    vector<const T*> HGCsubdet::getAllInRegion(double minR, double maxR, double minPhi, double maxPhi){
//in .h    
//in .h        vector<const T*> vecT = this->getAll<T>();
//in .h        vector<const T*> vecTselected;
//in .h    
//in .h        for(unsigned i=0; i<vecT.size(); i++){
//in .h            if( vecT.at(i)->Phi()>minPhi && vecT.at(i)->Phi()<maxPhi && 
//in .h                vecT.at(i)->r()>minR && vecT.at(i)->r()<maxR            ){
//in .h                vecTselected.push_back( vecT.at(i) );
//in .h            }
//in .h        }
//in .h    
//in .h        return vecTselected;
//in .h    
//in .h    }


bool HGCsubdet::isPertinent(float z){

    if( abs(z)>=abs(_layerZ.at(1)) && abs(z)<=abs(_layerZ.back()) ) 
        return true;
    
    return false;

}


HGCnorm HGCsubdet::getNormTransform_C2D( int nColsTanTheta, double tanThetaMin, double tanThetaMax, 
                                         int nRowsRho,      double rhoMin,      double rhoMax, 
                                         double minPhi, double maxPhi,
                                         bool weightPt  ) {

    HGCnorm transform( nColsTanTheta, tanThetaMin, tanThetaMax,
                       nRowsRho, rhoMin, rhoMax );

    vector<const HGCC2D*> c2ds = this->getAllInRegion<HGCC2D>(0, std::numeric_limits<double>::max(), 
                                                              minPhi, maxPhi );
    
    for(unsigned ic2d=0; ic2d<c2ds.size(); ic2d++){
        
        const HGCC2D* c2d = c2ds.at(ic2d);
        
        double w = weightPt ? c2d->Pt() : 1;
        transform.addPoint( c2d, w );

    }
    
    return transform;
                                         
}


HGCnorm HGCsubdet::getNormTransform_C3D( int nColsTanTheta, double tanThetaMin, double tanThetaMax, 
                                         int nRowsRho,      double rhoMin,      double rhoMax, 
                                         double minPhi, double maxPhi,
                                         bool weightPt
                                         //                                   unsigned nNeighboursSearch, float radius, vector<HGCgen*> gens
    ) {
    
    HGCnorm transform( nColsTanTheta, tanThetaMin, tanThetaMax,
                       nRowsRho, rhoMin, rhoMax );
    
    vector<const HGCC3D*> c3ds = getAllInRegion<HGCC3D>(0, std::numeric_limits<double>::max(), 
                                                        minPhi, maxPhi );
    
    for(unsigned ic3d=0; ic3d<c3ds.size(); ic3d++) {

        vector<unsigned> c2dIds = c3ds.at(ic3d)->clusters();

        for(std::vector<unsigned>::iterator c2dId=c2dIds.begin(); c2dId!=c2dIds.end(); c2dId++) {
            
            HGCC2D c2d;
            if( this->get<HGCC2D>(*c2dId, c2d) < 0 )
                continue;
            double w = weightPt ? c2d.Pt() : 1;

            transform.addPoint( &c2d, w );

        }

    }

    //  _vC3Dnorm = transform.getNewC3Ds( nNeighboursSearch, radius, gens );
    
    return transform;
                                         
}


HGCforest HGCsubdet::getNormTransform_C3Dforest( int nColsTanTheta, double tanThetaMin, double tanThetaMax, 
                                                 int nRowsRho,      double rhoMin,      double rhoMax, 
                                                 double minPhi,     double maxPhi,
                                                 bool weightPt  ) {
    
    HGCforest transform( nColsTanTheta, tanThetaMin, tanThetaMax,
                         nRowsRho, rhoMin, rhoMax );

    transform.setNormRadius(0.01);
    transform.setDACbit(13);

    vector<const HGCC2D*> c2ds = getAllInRegion<HGCC2D>(0, std::numeric_limits<double>::max(), 
                                                        minPhi, maxPhi );
    
    for(std::vector<const HGCC2D*>::iterator c2d=c2ds.begin(); c2d!=c2ds.end(); c2d++) {
        
        double w = weightPt ? (*c2d)->Pt() : 1;
        
        transform.addPoint( *(*c2d), w );
        
    }

//    _vC3Dforest = transform.getNewC3Ds(); 
    
    return transform;
                                         
}


HGCsingleC3D HGCsubdet::getSingleC3D() {

    HGCsingleC3D singleC3D;

    vector<const HGCC2D*> c2ds = getAll<HGCC2D>();
    
    for(std::vector<const HGCC2D*>::iterator c2d=c2ds.begin(); c2d!=c2ds.end(); c2d++) {
    
        singleC3D.addPoint( *(*c2d) );
        
    }
    
    return singleC3D;
                                         
}


HGCC3Dgen HGCsubdet::getGenC3D(double radius) {

    HGCC3Dgen genC3D;

    vector<const HGCC2D*> c2ds = getAll<HGCC2D>();
    vector<HGCgen*>       gens = getGenAll();
    
    for(std::vector<const HGCC2D*>::iterator c2d=c2ds.begin(); c2d!=c2ds.end(); c2d++) {

        genC3D.addPoint( *(*c2d), gens, radius );

    }
    
    return genC3D;
                                         
}


HGCht HGCsubdet::getRhoZtransform_C2D( int nColsTanTheta, double tanThetaMin, double tanThetaMax, 
                                       int nRowsRho,      double rhoMin,      double rhoMax, 
                                       double zOffset, double slopeCorrection,
                                       double minPhi, double maxPhi,
                                       bool weightPt  ) {

    HGCht transform( nColsTanTheta, tanThetaMin, tanThetaMax,
                     nRowsRho, rhoMin, rhoMax );

    vector<const HGCC2D*> c2ds = getAllInRegion<HGCC2D>(0, std::numeric_limits<double>::max(), 
                                                  minPhi, maxPhi );
    
    for(unsigned ic2d=0; ic2d<c2ds.size(); ic2d++){
        
        const HGCC2D* c2d = c2ds.at(ic2d);
        
        double w = weightPt ? c2d->Pt() : 1;
        //transform.addPoint( ( c2d->z()-zOffset )*slopeCorrection, 
        //                    c2d->r(), 
        //                    c2d->id(), 
        //                    w
        //    );

        transform.setZoffset( -zOffset );
        transform.setSlopeCorrection( slopeCorrection );        
        transform.addPoint( c2d, w );

    }

    return transform;

}


/* histos and graphs */
void HGCsubdet::getC2DallInPhiRegion( double minPhi, double maxPhi, TGraph &graph ){

    vector<const HGCC2D*> c2ds = this->getAllInRegion<HGCC2D>(0, std::numeric_limits<double>::max(), minPhi, maxPhi );

    for(unsigned iclu=0; iclu<c2ds.size(); iclu++){
        
        double xClu = c2ds.at(iclu)->x();
        double yClu = c2ds.at(iclu)->y();
        
        /* XY projection (layer1) */
        graph.SetPoint( graph.GetN(), xClu, yClu );
        
    }

}
    

/* fill tree */
//void HGCsubdet::fillTree() {
//    
//    _tree->Fill();
//
//}


/* save tree */
//void HGCsubdet::saveTree(TFile* f) {
//    
//    f->cd("/");    
//    _tree->Write();
//
//}


/* clear */
void HGCsubdet::clear() {

    for(unsigned ilayer=0; ilayer<this->nLayers(); ilayer++){
        _TC_layer[ilayer].clear();
        _C2D_layer[ilayer].clear();
    }    
    
    _TCs.clear();
    _C2Ds.clear();
    _C3Ds.clear();
    
    _TCvec.clear();
    _C2Dvec.clear();
    _C3Dvec.clear();

    _genVec.clear();

//    _vC3Dforest.clear();
//    _vC3Dnorm  .clear();
    
}
