
#include <HGCgenClu.h>


HGCgenClu::HGCgenClu() {
}

HGCgenClu::~HGCgenClu() {
}


bool HGCgenClu::addGen( HGCgen *newGen, float normDist ) {

    if(_gens.empty() ) {
        this->addGen( newGen );
        return true;
    }

    for( auto gen : _gens ) {
        if( gen->distanceNorm( newGen ) <= normDist ) {
            this->addGen( newGen );
            return true;
        }
    }

    return false;

}


void HGCgenClu::addGen( HGCgen *newGen ) {

    float layerZ = HGCgeom::instance()->layerZ( newGen->getEndcapId(), 1 );
    
    float W = this->Energy(); 
    float neww = newGen->Energy();
    float newW = W + newGen->Energy();

    float newGenEta = newGen->getZprojection( layerZ ).Eta();
    float newGenPhi = newGen->getZprojection( layerZ ).Phi();
    float newGenX   = newGen->getZprojection( layerZ ).X();  
    float newGenY   = newGen->getZprojection( layerZ ).Y();      
    float newGenZ   = newGen->getZprojection( layerZ ).Z();  

    float eta    = ( ( this->Eta() * W ) + (  newGenEta * neww ) ) / newW;
    float phi    = ( ( this->Phi() * W ) + (  newGenPhi * neww ) ) / newW;
    float x      = ( ( this->x()   * W ) + (  newGenX   * neww ) ) / newW;
    float y      = ( ( this->y()   * W ) + (  newGenY   * neww ) ) / newW;
    float z      = ( ( this->z()   * W ) + (  newGenZ   * neww ) ) / newW;
    
    float energy = newW;
    float pt     = this->Pt()     + newGen->Pt();

    this->setEnergy( energy );
    this->setEta   ( eta    );
    this->setPhi   ( phi    );
    this->setX     ( x      );
    this->setY     ( y      );
    this->setZ     ( z      );
    this->setPt    ( pt     );

    _gens.push_back( newGen );

}


bool HGCgenClu::isIncluded( HGCgen *newGen ) {

    for( auto gen : _gens ) 
        if( gen == newGen ) 
            return true;
    
    return false;

}

vector<HGCgen*> HGCgenClu::getConstituents() {

    return _gens;

}



void HGCgenClu::printConstituents() {

    int i=0;

    for( auto gen : _gens ) {
        
        cout << "component " << ++i  << " of " << _gens.size() << endl;
        cout << "ETA " << gen->Eta() << endl;
        cout << "PHI " << gen->Phi() << endl;
        cout << "ENE " << gen->Energy() << endl;
        


    }

}

