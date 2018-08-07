
/* include the .h */
#include "HGCC3D.h"
#include "TMath.h"

ClassImp( HGCC3D )

HGCC3D::HGCC3D()  { 
    _firstLayer=52; 
    _maxLayer=0; 
}

HGCC3D::~HGCC3D() {}
//cmssw void updateP4AndPosition(const edm::Ptr<C>& c, bool updateCentre=true, float fraction=1.)
//cmssw      {
//cmssw        double cMipt = c->mipPt()*fraction;
//cmssw        /* update cluster positions (IF requested) */
//cmssw        if( updateCentre ){
//cmssw          Basic3DVector<float> constituentCentre( c->position() );
//cmssw          Basic3DVector<float> clusterCentre( centre_ );
//cmssw
//cmssw          clusterCentre = clusterCentre*mipPt_ + constituentCentre*cMipt;
//cmssw          if( (mipPt_ + cMipt ) > 0 )
//cmssw          {
//cmssw            clusterCentre /= ( mipPt_ +  cMipt );
//cmssw          }
//cmssw          centre_ = GlobalPoint( clusterCentre );
//cmssw
//cmssw          if( clusterCentre.z()!=0 )
//cmssw          {
//cmssw            centreProj_= GlobalPoint( clusterCentre / clusterCentre.z() );
//cmssw          }
//cmssw        }
//cmssw
//cmssw        /* update cluster energies */
//cmssw        mipPt_ += cMipt;
//cmssw
//cmssw        int updatedPt = hwPt() + (int)(c->hwPt()*fraction);
//cmssw        setHwPt( updatedPt );
//cmssw
//cmssw        math::PtEtaPhiMLorentzVector updatedP4 ( p4() );
//cmssw        updatedP4 += (c->p4()*fraction);
//cmssw        setP4( updatedP4 );
//cmssw      }

/* add */
void HGCC3D::addC2D( const HGCC2D* c2d) {

    if( _clusters.size()==0 ) {
        this->setId( c2d->id() );
        this->setEta ( c2d->Eta () );
        this->setPhi ( c2d->Phi () );
        this->setZ   ( c2d->z () ); 
        this->setXnorm( c2d->xNorm() );
        this->setYnorm( c2d->yNorm() );
    } else {
        this->setXnorm( ( this->xNorm()+c2d->xNorm() )/2. );
        this->setYnorm( ( this->yNorm()+c2d->yNorm() )/2. );
    }

    _clusters.push_back( c2d->id() );
    for(int icell=0; icell<c2d->cells().size(); icell++ )
        _cells.push_back( c2d->cells().at(icell) );

    /* centre */
    double totPt = c2d->Pt()+this->Pt();
    this->setEta ( ( c2d->Eta()*c2d->Pt()+this->Eta()*this->Pt() ) / totPt );
    this->setPhi ( ( c2d->Phi()*c2d->Pt()+this->Phi()*this->Pt() ) / totPt );
    this->setZ   ( ( c2d->z()*c2d->Pt()+this->z()*this->Pt() ) / totPt );
    
    /* energy and Pt */
    this->setEnergy( this->Energy()+c2d->Energy() );
    this->setPt    ( this->Pt()+c2d->Pt()         );

    if( c2d->layer() < this->getFirstLayer() ) this->setFirstLayer( c2d->layer() ); 
    if( c2d->layer() > this->getLastLayer()   ) {
        this->setLastLayer( c2d->layer() ); 
        this->setShowerLength( this->getLastLayer() - this->getFirstLayer() );
    }

}

void HGCC3D::addTC( const HGCTC* tc) {

    if( _cells.size()==0 ) {
        this->setId   ( tc->id() );
        this->setEta  ( tc->Eta () );
        this->setPhi  ( tc->Phi () );
        this->setZ    ( tc->z () ); 
        this->setXnorm( tc->xNorm() );
        this->setYnorm( tc->yNorm() );
    } else {
        this->setXnorm( ( this->xNorm()+tc->xNorm() )/2. );
        this->setYnorm( ( this->yNorm()+tc->yNorm() )/2. );
    }

    _cells.push_back( tc->id() );

    /* centre */
    double totPt = tc->Pt()+this->Pt();
    this->setEta ( ( tc->Eta()*tc->Pt()+this->Eta()*this->Pt() ) / totPt );
    this->setPhi ( ( tc->Phi()*tc->Pt()+this->Phi()*this->Pt() ) / totPt );
    this->setZ   ( ( tc->z()*tc->Pt()+this->z()*this->Pt() ) / totPt );
    
    /* energy and Pt */
    this->setEnergy( this->Energy()+tc->Energy() );
    this->setPt    ( this->Pt()+tc->Pt()         );

    if( tc->layer() < this->getFirstLayer() ) this->setFirstLayer( tc->layer() ); 
    if( tc->layer() > this->getLastLayer()   ) {
        this->setLastLayer( tc->layer() ); 
        this->setShowerLength( this->getLastLayer() - this->getFirstLayer() );
    }

}


/* set */
void HGCC3D::setClusters(vector<unsigned> clusters) { _clusters = clusters; }
void HGCC3D::setCells(vector<unsigned> cells)       { _cells = cells;       }
void HGCC3D::setNearestGen(HGCgen &gen)             { _nearestGen = gen;    }
void HGCC3D::setNearestGen(vector<HGCgen*> gens)    {
    
      if( gens.size()>0 ){

            double dmin=10.; // norm. coo.
            unsigned minGenId=0;
            
            for( unsigned igen=0; igen<gens.size(); igen++ ) {
                
                if( gens.at(igen)->getEndcapId() != this->getEndcapId() ) continue;

                double XnormDiff = gens.at(igen)->xNorm() - this->xNorm();
                double YnormDiff = gens.at(igen)->yNorm() - this->yNorm();
                double d = TMath::Sqrt( XnormDiff*XnormDiff + YnormDiff*YnormDiff );

                if( d<dmin ){
                    minGenId=igen;
                    dmin=d; 
                }

            }

            /* set the nearest gen */
            this->setNearestGen( *gens.at(minGenId) );
            
        }

}

void HGCC3D::setXnorm(float xNorm)                  { _xNorm = xNorm;       }
void HGCC3D::setYnorm(float yNorm)                  { _yNorm = yNorm;       }
void HGCC3D::setFirstLayer(unsigned firstLayer)     { _firstLayer = firstLayer; }
void HGCC3D::setMaxLayer(unsigned maxLayer)         { _maxLayer  = maxLayer;  }
void HGCC3D::setLastLayer(unsigned lastLayer)       { _lastLayer  = lastLayer;  }
void HGCC3D::setShowerLength(unsigned showerLength) { _showerLength   = showerLength;   }

/* get */
unsigned         HGCC3D::nclusters() const          { return _clusters.size(); }
vector<unsigned> HGCC3D::clusters() const           { return _clusters;        }

unsigned         HGCC3D::ncells() const             { return _cells.size(); }
vector<unsigned> HGCC3D::cells() const              { return _cells;        }

TLorentzVector   HGCC3D::P4() const {

  TLorentzVector p4;
  p4.SetPtEtaPhiE( this->Pt(), this->Eta(), this->Phi(), this->Energy() );
  return p4;

}


unsigned HGCC3D::getFirstLayer() const { 
    return _firstLayer; 
}


unsigned HGCC3D::getShowerLength() const { 
    return _showerLength; 
}


unsigned HGCC3D::getMaxLayer() const { 
    return _maxLayer;  
}


unsigned HGCC3D::getLastLayer() const { 
    return ( _lastLayer );  
}


ROOT::Math::RhoEtaPhiPoint HGCC3D::getZprojection(double z){
    
    ROOT::Math::RhoEtaPhiPoint proj( z*TMath::Tan(2*TMath::ATan(TMath::Exp(-this->Eta()))), this->Eta(), this->Phi() );
    //ROOT::Math::RhoEtaPhiPoint proj( 0, 0, 0 );
    
    return proj;

}


float HGCC3D::xNorm() const { 
    return _xNorm;  
}


float HGCC3D::yNorm() const { 
    return _yNorm;  
}


HGCgen HGCC3D::getNearestGen() const { 
    return _nearestGen;  
}


float HGCC3D::getEnergyDensity(float radiusNorm) const {
    
//    cout << this->getEndcapId() << " " << this->getFirstLayer() << " " << (int)this->getLastLayer() << endl;
    float z1 = TMath::Cos( this->Theta() )*HGCgeom::instance()->layerZ( this->getEndcapId(), this->getFirstLayer() ) ;
    float z2 = TMath::Cos( this->Theta() )*HGCgeom::instance()->layerZ( this->getEndcapId(), this->getLastLayer() ) ;
    double volume = TMath::Abs( ( TMath::Pi()*radiusNorm*(z2*z2-z1*z1) )/3. );
    
    return this->Energy()/volume;

}


float HGCC3D::getEnergyDensity() const {
    
    double density = 0;
    if( this->ncells() > 0 ) 
        density = this->Energy()/this->ncells();
    
    return density;

}


float HGCC3D::getEnergyDensity(vector<const HGCC3D*> C3Ds, double radiusRegionNorm, double radiusNorm) const {
    
    float density = 0;
    int N = 0;

    for(unsigned ic3d=0; ic3d<C3Ds.size(); ic3d++) {
        const HGCC3D* c3d = C3Ds.at(ic3d);
        
        if( c3d->id() == this->id() )
            continue;

        float dist = TMath::Sqrt( (c3d->xNorm()-this->xNorm())*(c3d->xNorm()-this->xNorm()) + (c3d->yNorm()-this->yNorm())*(c3d->yNorm()-this->yNorm()) );
        
        if( dist > radiusRegionNorm )
            continue;

        density = density + c3d->getEnergyDensity( radiusNorm );
        N++;

    }
    
    if( N>0 )
        density = density/N;
    else
        density = 0.;

    return density;

}


float HGCC3D::getEnergyDensity(vector<HGCC3D> *C3Ds, double radiusRegionNorm, double radiusNorm) const {
    
    float density = 0;
    int N = 0;

    for(unsigned ic3d=0; ic3d<C3Ds->size(); ic3d++) {
        const HGCC3D* c3d = &( C3Ds->at(ic3d) );
        
        if( c3d->id() == this->id() )
            continue;

        float dist = TMath::Sqrt( (c3d->xNorm()-this->xNorm())*(c3d->xNorm()-this->xNorm()) + (c3d->yNorm()-this->yNorm())*(c3d->yNorm()-this->yNorm()) );
        
        if( dist > radiusRegionNorm )
            continue;

        density = density + c3d->getEnergyDensity( radiusNorm );
        N++;

    }
    
    if( N>0 )
        density = density/N;
    else
        density = 0.;

    return density;

}


//float HGCC3D::getEnergyDensity(vector<HGCC3D> *C3Ds, double radiusNorm) {    
//    
//    float totEnergy = 0;
//
//    for(unsigned ic3d=0; ic3d<C3Ds->size(); ic3d++) {
//        HGCC3D* c3d = &(C3Ds->at(ic3d));
//        
//        if( c3d->id() == this->id() )
//            continue;
//
//        float dist = TMath::Sqrt( (c3d->xNorm()-this->xNorm())*(c3d->xNorm()-this->xNorm()) + (c3d->yNorm()-this->yNorm())*(c3d->yNorm()-this->yNorm()) );
//        
//        if( dist > radiusNorm )
//            continue;
//
//        totEnergy += c3d->Energy();
//        
//    }
//    
//    float z1 = TMath::Cos( this->Theta() )*HGCgeom::instance()->layerZ( this->getEndcapId(), this->getFirstLayer() ) ;
//    float z2 = TMath::Cos( this->Theta() )*HGCgeom::instance()->layerZ( this->getEndcapId(), this->getLastLayer() ) ;
//    double volume = TMath::Abs( ( TMath::Pi()*radiusNorm*(z2*z2-z1*z1) )/3. );
//    
//    return totEnergy/volume;
//
//}

void HGCC3D::print() {

    cout << "-------------" << endl;
    cout << " pt : " << this->Pt() << endl
         << " ene: " << this->Energy() << endl
         << " eta:"  << this->Eta() << endl
         << " phi:"  << this->Phi() << endl;        
    cout << "=============" << endl;

}
