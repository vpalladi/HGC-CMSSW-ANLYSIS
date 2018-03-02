
/* mylibs */
#include "HGCsubdet.h"


HGCsubdet::HGCsubdet( unsigned endcapId, unsigned sectionId, bool triggerLayersOnly, int verboselevel ) {
    
    /* init */
    _endcapId = endcapId;
    _sectionId  = sectionId;
    _verboselevel = verboselevel;
    
    /* Layers Z */
    
    _validTriggerLayer[0] = false;
    for(unsigned ilayer=1; ilayer<53; ilayer++){
        if( triggerLayersOnly && ilayer<29 && (ilayer%2==1) )
            _validTriggerLayer[ilayer] = false;
        else
            _validTriggerLayer[ilayer] = true;
    }

    // first layer is never there
    _layerZ.push_back(0);
    // EE
    if( _sectionId==0 || _sectionId==3 ){
        if( !triggerLayersOnly ) _layerZ.push_back( 320.755 );
        _layerZ.push_back( 321.505 );
        if( !triggerLayersOnly ) _layerZ.push_back( 322.735 );
        _layerZ.push_back( 323.485 );
        if( !triggerLayersOnly ) _layerZ.push_back( 324.715 );
        _layerZ.push_back( 325.465 );
        if( !triggerLayersOnly ) _layerZ.push_back( 326.695 );
        _layerZ.push_back( 327.445 );
        if( !triggerLayersOnly ) _layerZ.push_back( 328.675 );
        _layerZ.push_back( 329.425 );
        if( !triggerLayersOnly ) _layerZ.push_back( 330.735 );
        _layerZ.push_back( 331.605 );
        if( !triggerLayersOnly ) _layerZ.push_back( 332.915 );
        _layerZ.push_back( 333.785 );
        // middle EE 334.440;
        if( !triggerLayersOnly ) _layerZ.push_back( 335.095 );
        _layerZ.push_back( 335.965 );
        if( !triggerLayersOnly ) _layerZ.push_back( 337.275 );
        _layerZ.push_back( 338.145 );
        if( !triggerLayersOnly ) _layerZ.push_back( 339.455 );
        _layerZ.push_back( 340.325 );
        if( !triggerLayersOnly ) _layerZ.push_back( 341.775 );
        _layerZ.push_back( 342.845 );
        if( !triggerLayersOnly ) _layerZ.push_back( 344.295 );
        _layerZ.push_back( 345.365 );
        if( !triggerLayersOnly ) _layerZ.push_back( 346.815 );
        _layerZ.push_back( 347.885 );
        if( !triggerLayersOnly ) _layerZ.push_back( 349.335 );
        _layerZ.push_back( 350.405 );
    }
//FH
    else if( _sectionId==1  || _sectionId==3 ) {
        _layerZ.push_back( 356.335 );
        _layerZ.push_back( 361.015 );
        _layerZ.push_back( 365.695 );
        _layerZ.push_back( 370.375 );
        _layerZ.push_back( 375.055 );
        _layerZ.push_back( 379.735 );
        _layerZ.push_back( 384.415 );
        _layerZ.push_back( 389.095 );
        _layerZ.push_back( 393.775 );
        _layerZ.push_back( 398.455 );
        _layerZ.push_back( 403.135 );
        _layerZ.push_back( 407.815 );
    }
        // BH is not exact
    else if( _sectionId==2  || _sectionId==3 ) {
        _layerZ.push_back( 421.0       );
        _layerZ.push_back( 421.0+9.0*1 );
        _layerZ.push_back( 421.0+9.0*2 );
        _layerZ.push_back( 421.0+9.0*3 );
        _layerZ.push_back( 421.0+9.0*4 );
        _layerZ.push_back( 421.0+9.0*5 );
        _layerZ.push_back( 421.0+9.0*6 );
        _layerZ.push_back( 421.0+9.0*7 );
        _layerZ.push_back( 421.0+9.0*8 );
        _layerZ.push_back( 421.0+9.0*9 );
        _layerZ.push_back( 421.0+9.0*10 );
        _layerZ.push_back( 421.0+9.0*11 );
    }
    else{
        exit(1);
    }

    // revert in case is negative z
    if( _endcapId==1 )
        for(unsigned ilayer=0; ilayer<this->nLayers(); ilayer++)
            _layerZ[ilayer] = -_layerZ[ilayer];

    /* allocate the containers for the TC C2D and ROC */
    _TC_layer  = new vector<HGCTC*>[this->nLayers()];
    _C2D_layer = new vector<HGCC2D*>[this->nLayers()];
    _TD        = new vector<HGCROC>*[this->nLayers()];
    for(unsigned i=0; i<this->nLayers(); i++) {
        _TD[i]  = new vector<HGCROC> ;
        _TD[i] ->reserve(5000);
    }
    

}


HGCsubdet::~HGCsubdet() {
    
    for(unsigned i=0; i<this->nLayers(); i++) {
        delete _TD[i];
    }

};


void HGCsubdet::addTC( HGCTC tc ) { 

    /* add tc to the map*/
    _TCs[tc.id()] = tc;
    
    /* keep the pointer for easy access */
    _TCvec.push_back( &_TCs[tc.id()] );
        
}


void HGCsubdet::addC2D( HGCC2D c2d ) { 
    
    /* add c2d to the map */
    _C2Ds[c2d.id()] = c2d;
  
    /* keep the pointer for easy access */
    _C2Dvec.push_back( &_C2Ds[c2d.id()] );

}


void HGCsubdet::addC3D(HGCC3D c3d)      {   

    /* add c2d to the map */
    _C3Ds[c3d.id()] = c3d;

    /* keep the pointer for easy access */
    _C3Dvec.push_back( &_C3Ds[c3d.id()] );

}



/* get objects by id */
HGCTC                 HGCsubdet::getTC(unsigned ID)      { return _TCs[ID]          ; }
HGCC2D                HGCsubdet::getC2D(unsigned ID)     { return _C2Ds[ID]         ; }
HGCC3D                HGCsubdet::getC3D(unsigned ID)     { return _C3Ds[ID]         ; }

map<unsigned,HGCTC>  *HGCsubdet::getTCmap()              { return &_TCs             ; }
map<unsigned,HGCC2D> *HGCsubdet::getC2Dmap()             { return &_C2Ds            ; }
map<unsigned,HGCC3D> *HGCsubdet::getC3Dmap()             { return &_C3Ds            ; }

vector<HGCTC*>        HGCsubdet::getTC_layer(unsigned layer)   { return _TC_layer[layer]  ; }
vector<HGCC2D*>       HGCsubdet::getC2D_layer(unsigned layer)  { return _C2D_layer[layer] ; }
vector<HGCROC>       *HGCsubdet::getTD(unsigned layer)         { return _TD[layer]        ; }


vector<HGCTC*>        HGCsubdet::getTCall()                    { return _TCvec            ; }
vector<HGCC2D*>       HGCsubdet::getC2Dall()                   { return _C2Dvec           ; }
vector<HGCC3D*>       HGCsubdet::getC3Dall()                   { return _C3Dvec           ; }

void HGCsubdet::getTDall( vector<HGCROC> &data ) {

    for(unsigned i=0; i<this->nLayers(); i++)
        data.insert(data.end(), _TD[i]->begin(), _TD[i]->end() );

}


vector<HGCTC*> HGCsubdet::getTCallInPhiRegion(double minPhi, double maxPhi){

    vector<HGCTC*> selectedTc;

    for(unsigned itc=0; itc<_TCvec.size(); itc++){
        if( _TCvec.at(itc)->Phi()>minPhi && _TCvec.at(itc)->Phi()<maxPhi ){
            selectedTc.push_back( _TCvec.at(itc) );
        }
    }

    return selectedTc;

}


vector<HGCC2D*> HGCsubdet::getC2DallInPhiRegion(double minPhi, double maxPhi){

    vector<HGCC2D*> selectedC2D;

    for(unsigned iC2D=0; iC2D<_C2Dvec.size(); iC2D++){
        if( _C2Dvec.at(iC2D)->Phi()>minPhi && _C2Dvec.at(iC2D)->Phi()<maxPhi ){ 
            selectedC2D.push_back( _C2Dvec.at(iC2D) );
        }
    }

    return selectedC2D;

}


vector<HGCC2D*> HGCsubdet::getC2DallInRphiRegion(double minR, double maxR, double minPhi, double maxPhi){

    vector<HGCC2D*> selectedC2D;

    for(unsigned iC2D=0; iC2D<_C2Dvec.size(); iC2D++){
        if( _C2Dvec.at(iC2D)->Phi()>minPhi && _C2Dvec.at(iC2D)->Phi()<maxPhi && 
            _C2Dvec.at(iC2D)->r()>minR && _C2Dvec.at(iC2D)->r()<maxR            ){
            selectedC2D.push_back( _C2Dvec.at(iC2D) );
        }
    }

    return selectedC2D;

}


vector<HGCTC*> HGCsubdet::getTCallInRphiRegion(double minR, double maxR, double minPhi, double maxPhi){

    vector<HGCTC*> selectedTc;

    for(unsigned itc=0; itc<_TCvec.size(); itc++){
        if( _TCvec.at(itc)->Phi()>minPhi && _TCvec.at(itc)->Phi()<maxPhi &&
            _TCvec.at(itc)->r()>minR     && _TCvec.at(itc)->r()<maxR        ){
            selectedTc.push_back( _TCvec.at(itc) );
        }
    }

    return selectedTc;

}


bool HGCsubdet::isPertinent(float z){

    if( abs(z)>=abs(_layerZ.at(1)) && abs(z)<=abs(_layerZ.back()) ) 
        return true;
    
    return false;

}


HGCht HGCsubdet::getRhoZtransform_C2D( int nColsTanTheta, double tanThetaMin, double tanThetaMax, 
                                       int nRowsRho,      double rhoMin,      double rhoMax, 
                                       double zOffset,
                                       double minPhi, double maxPhi,
                                       bool weightPt  ) {

    HGCht transform( nColsTanTheta, tanThetaMin, tanThetaMax,
                     nRowsRho, rhoMin, rhoMax );

    vector<HGCC2D*> c2ds = getC2DallInPhiRegion(minPhi, maxPhi);

    for(unsigned ic2d=0; ic2d<c2ds.size(); ic2d++){
        
        HGCC2D* c2d = c2ds.at(ic2d);
        
        double w = weightPt ? c2d->Pt() : 1;
        transform.addPointPhysicalSpace( ( c2d->z()-zOffset ), 
                                         c2d->r(), 
                                         c2d->id(), 
                                         w
            );
    
    }
    
    return transform;
                                         
}


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
    
}




