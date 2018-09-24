
#include <iostream>
#include <cmath>

#include <TFile.h>
#include <TTree.h>
#include <TGraph.h>

#include <HGCgeom.h>

using namespace std;


int main() {

    double phiRangeMin[6] = {-  M_PI  , -2*M_PI/3, -M_PI/3,    0  ,   M_PI/3,  2*M_PI/3};
    double phiRangeMax[6] = {-2*M_PI/3, -  M_PI/3,     0  , M_PI/3, 2*M_PI/3,    M_PI  };

    int nLayers=52;
    TGraph gSector;
    int iendcap=0;
    for ( unsigned ilayer=0; ilayer<nLayers; ilayer++ ) {
        map< unsigned, geoWafer > wafers = HGCgeom::instance()->getWafers( iendcap, ilayer);
        cout << " >>> LAYER " << ilayer << endl;
        int counter3 = 0;
        int ilink    = 0;
        int tcLocalId     = 0;
        for ( map< unsigned, geoWafer >::iterator waferIt = wafers.begin(); waferIt != wafers.end(); waferIt++ ) {
            
            int isector = 0;
            double phi0 = M_PI * (3-isector) / 3;
                
            //cout << " >>> Sector " << isector << endl;
            
            if( waferIt->second.isFullyContainedInPhiSector( phiRangeMin[isector], phiRangeMax[isector] ) ) {
                //std::cout << waferIt->first  // string (key)
                //          << ':'
                //          << " link " << ilink << " r: " 
                //          << waferIt->second.r()  << ", phi: " 
                //          << waferIt->second.phi() + phi0 << ", nTcs: " 
                //          << waferIt->second.getTCs().size()
                //          << std::endl;
                
                if( (counter3 % 3) == 0 ) {
                    counter3=0;
                    ilink++;
                    tcLocalId=0;
                    
                }
                
                for(auto tc : waferIt->second.getTCs() ) {
                    gSector.SetPoint(gSector.GetN(), tc.x, tc.y );
                    cout << std::hex 
                         << " 0x" << ilink 
                         << " 0x" << tcLocalId 
                         << " 0x" << tc.rz(11, 0, 0., 0.6)
                         << " 0x" << tc.phi(10, phi0, 0, M_PI/3)
                         << std::dec 
                         << endl;
                    tcLocalId++;
                }
                
                counter3++;
            }
            
        }
        
    }

    return 0;

}
