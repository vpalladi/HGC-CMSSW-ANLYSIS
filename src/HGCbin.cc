
#include "HGCsubdet.h"
#include "HGCbin.h"


HGCbin::HGCbin() { 
    _content = 0.;
}

HGCbin::HGCbin(float x, float y) { 
    this->setCentre(x, y); 
    _content = 0.;
}
HGCbin::~HGCbin() { ; }

void HGCbin::setCentre(float x, float y) { 
    _centre.first  = x; 
    _centre.second = y;
}

void HGCbin::setCentreId(int xBin, int yBin) { 
    _centreId.first = xBin; 
    _centreId.second = yBin;
}

void HGCbin::addContent( float content, unsigned id ) { 
    _content += content; 
    _ids.push_back( id ); 
}

void HGCbin::clear() {
    _content=0;
    _ids.clear();
}

pair<float,float> HGCbin::getCentre()   { return _centre;   }
pair<int,int>     HGCbin::getCentreId() { return _centreId; }
vector<unsigned>  HGCbin::getIds()      { return _ids;      }
float             HGCbin::getContent()  { return _content;  }


void HGCbin::getLongitudinalEnergyProfile(HGCsubdet *det, TGraph &g){
        
    for( std::vector<unsigned>::iterator id=_ids.begin(); id!=_ids.end(); id++ ) {          
        HGCC2D c2d;
        if( det->get<HGCC2D>( (*id), c2d ) < 0 )
            continue;
        g.SetPoint( g.GetN(), c2d.z(), c2d.Energy() );

    }

}


void HGCbin::getLongitudinalPtProfile(HGCsubdet *det, TGraph &g){
    
    for( std::vector<unsigned>::iterator id=_ids.begin(); id!=_ids.end(); id++ ) {          
        HGCC2D c2d;
        if( det->get<HGCC2D>( (*id), c2d ) < 0 )
            continue;
        g.SetPoint( g.GetN(), c2d.z(), c2d.Pt() );

    }
    
}


bool HGCbin::isAboveThr(double thr) { 
    
    if( _content < thr ) 
        return false; 

    return true; 

}
