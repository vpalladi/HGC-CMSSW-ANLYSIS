
/* include the .h */
#include "HGCC3D.h"

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

    /* centre */
    double totPt = c2d->Pt()+this->Pt();
    this->setEta ( ( c2d->Eta()*c2d->Pt()+this->Eta()*this->Pt() ) / totPt );
    this->setPhi ( ( c2d->Phi()*c2d->Pt()+this->Phi()*this->Pt() ) / totPt );
    this->setZ   ( ( c2d->z()*c2d->Pt()+this->z()*this->Pt() ) / totPt );
    
    /* energy and Pt */
    this->setEnergy( this->Energy()+c2d->Energy() );
    this->setPt    ( this->Pt()+c2d->Pt()         );

    if( c2d->layer() < this->getFirstLayer() ) this->setFirstLayer( c2d->layer() ); 
    if( c2d->layer() > this->getMaxLayer()   ) this->setMaxLayer( c2d->layer() ); 

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
void HGCC3D::setFirstLayer(float firstLayer)        { _firstLayer = firstLayer; }
void HGCC3D::setMaxLayer(float maxLayer)        { _maxLayer = maxLayer; }


/* get */
unsigned         HGCC3D::nclusters() const          { return _clusters.size(); }
vector<unsigned> HGCC3D::clusters() const           { return _clusters; }

unsigned         HGCC3D::ncells() const             { return _cells.size(); }
vector<unsigned> HGCC3D::cells() const              { return _cells; }

TLorentzVector   HGCC3D::P4() const {

  TLorentzVector p4;
  p4.SetPtEtaPhiE( this->Pt(), this->Eta(), this->Phi(), this->Energy() );
  return p4;

}


unsigned HGCC3D::getFirstLayer() const { 
    return _firstLayer; 
}


unsigned HGCC3D::getMaxLayer() const { 
    return _maxLayer;  
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


void HGCC3D::print() {

    cout << "-------------" << endl;
    cout << " pt : " << this->Pt() << endl
         << " ene: " << this->Energy() << endl
         << " eta:"  << this->Eta() << endl
         << " phi:"  << this->Phi() << endl;        
    cout << "=============" << endl;

}
