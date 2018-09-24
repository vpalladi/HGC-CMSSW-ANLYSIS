
#ifndef HGCgeom_HH
#define HGCgeom_HH

#include <iostream>
#include <vector>
#include <map>

#include <TFile.h>
#include <TTree.h>

using namespace std;
/*
  
  units:

  - distances: [cm]


*/
class geoTC {
    
public:

    geoTC() {}

    int   id         ;
    int   zside      ;
    int   subdet     ;
    int   layer      ;
    int   wafer      ;
    int   triggercell;
    float x          ;
    float y          ;
    float z          ;

    int absLayer() {
        int absLayer = -1;
        if( subdet == 3 )
            absLayer = layer;
        else if( subdet == 4 )
            absLayer = layer+12;
        else if( subdet == 5 )
            absLayer = layer+24;
        return absLayer;
    }
    
    double r() {
        double r = sqrt( pow(this->x, 2) + pow(this->y, 2) );
        return r;
    }

    double phi() {
        double phi = atan2( this->y, this->x );
        return phi;
    }
    
    uint32_t r(int nbits, double offset, double min, double max) {
        uint32_t mask = 0;
        for(int i=0; i<32; i++)
            mask = mask | (1<<i);
        double delta = abs(max-min)/pow(2,nbits);
        uint32_t r = (this->r()+offset)/delta;
        return (r & mask);
    }

    uint32_t rz(int nbits, double offset, double min, double max) {
        uint32_t mask = 0;
        for(int i=0; i<32; i++)
            mask = mask | (1<<i);
        double delta = abs(max-min)/pow(2,nbits);
        uint32_t rz = (this->r()/abs(z)+offset)/delta;
        return (rz & mask);
    }

    uint32_t phi(int nbits, double offset, double min, double max) {
        uint32_t mask = 0;
        for(int i=0; i<32; i++)
            if(i<nbits)
                mask = mask | (1<<i);
        double delta = abs(max-min)/pow(2,nbits);
        uint32_t phi = (this->phi()+offset)/delta;
        
        return (phi & mask);
    }

};


class geoWafer {

public:

    geoWafer() {
        _id = -1;
    }

    geoWafer(geoTC tc) {
        this->addTC(tc);
        _id = tc.id;
    }

    int id(){ 
        return _id; 
    }

    void addTC(geoTC tc) {
        _tcs.push_back(tc);
    }

    vector<geoTC> getTCs() {
        return _tcs;
    }

    double x() { 
        double x=0;
        for(auto tc : _tcs) 
            x += tc.x;
        x = x/_tcs.size(); 
        return x;
    }

    double y() { 
        double y=0;
        for(auto tc : _tcs) 
            y += tc.y;
        y = y/_tcs.size(); 
        return y;

    }

    double r() {
        double r = sqrt( pow(this->x(), 2) + pow(this->y(), 2) );
        return r;
    }

    double phi() {
        double phi = atan2( this->y(), this->x() );
        return phi;
    }

    bool isFullyContainedInPhiSector(double phiMin, double phiMax) {

        for( auto tc : _tcs )
            if( tc.phi()<phiMin || tc.phi()>phiMax )
                return false;
        return true;

    }
    
private:

    int _id;

    vector<geoTC> _tcs;

};


class HGCgeom {

private:

    static HGCgeom *s_instance;
    
    HGCgeom() {  /* constructor */
        initialise();
    }    

public:

    ~HGCgeom() {
        
        for( unsigned iendcap=0; iendcap<_nEndcaps; iendcap++ )
            delete[] _wafers[iendcap];
        delete[] _wafers;
        
    }
    
    static HGCgeom* instance();

    void initialise() {

        _nEndcaps  = 2 ;
        _nLayersEE = 28;
        _nLayersFH = 12;
        _nLayersBH = 12;
        _nLayers   = _nLayersEE + _nLayersFH + _nLayersBH;

        _layerZ         = new double[_nLayers+1];
        _isTriggerLayer = new bool[_nLayers+1];
        // no layer called 0 
        _layerZ[ 0]= 0      ;    _isTriggerLayer[ 0] = false;
        //EE                    _
        _layerZ[ 1]= 320.755;    _isTriggerLayer[ 1] = true ;
        _layerZ[ 2]= 321.505;    _isTriggerLayer[ 2] = false;
        _layerZ[ 3]= 322.735;    _isTriggerLayer[ 3] = true ;
        _layerZ[ 4]= 323.485;    _isTriggerLayer[ 4] = false;
        _layerZ[ 5]= 324.715;    _isTriggerLayer[ 5] = true ;
        _layerZ[ 6]= 325.465;    _isTriggerLayer[ 6] = false;
        _layerZ[ 7]= 326.695;    _isTriggerLayer[ 7] = true ;
        _layerZ[ 8]= 327.445;    _isTriggerLayer[ 8] = false;
        _layerZ[ 9]= 328.675;    _isTriggerLayer[ 9] = true ;
        _layerZ[10]= 329.425;    _isTriggerLayer[10] = false;
        _layerZ[11]= 330.735;    _isTriggerLayer[11] = true ;
        _layerZ[12]= 331.605;    _isTriggerLayer[12] = false;
        _layerZ[13]= 332.915;    _isTriggerLayer[13] = true ;
        _layerZ[14]= 333.785;    _isTriggerLayer[14] = false;
        _layerZ[15]= 335.095;    _isTriggerLayer[15] = true ;
        _layerZ[16]= 335.965;    _isTriggerLayer[16] = false;
        _layerZ[17]= 337.275;    _isTriggerLayer[17] = true ;
        _layerZ[18]= 338.145;    _isTriggerLayer[18] = false;
        _layerZ[19]= 339.455;    _isTriggerLayer[19] = true ;
        _layerZ[20]= 340.325;    _isTriggerLayer[20] = false;
        _layerZ[21]= 341.775;    _isTriggerLayer[21] = true ;
        _layerZ[22]= 342.845;    _isTriggerLayer[22] = false;
        _layerZ[23]= 344.295;    _isTriggerLayer[23] = true ;
        _layerZ[24]= 345.365;    _isTriggerLayer[24] = false;
        _layerZ[25]= 346.815;    _isTriggerLayer[25] = true ;
        _layerZ[26]= 347.885;    _isTriggerLayer[26] = false;
        _layerZ[27]= 349.335;    _isTriggerLayer[27] = true ;
        _layerZ[28]= 350.405;    _isTriggerLayer[28] = false;
        // FH                   _                       
        _layerZ[29]= 356.335;    _isTriggerLayer[29] = true ;
        _layerZ[30]= 361.015;    _isTriggerLayer[30] = true ;
        _layerZ[31]= 365.695;    _isTriggerLayer[31] = true ;
        _layerZ[32]= 370.375;    _isTriggerLayer[32] = true ;
        _layerZ[33]= 375.055;    _isTriggerLayer[33] = true ;
        _layerZ[34]= 379.735;    _isTriggerLayer[34] = true ;
        _layerZ[35]= 384.415;    _isTriggerLayer[35] = true ;
        _layerZ[36]= 389.095;    _isTriggerLayer[36] = true ;
        _layerZ[37]= 393.775;    _isTriggerLayer[37] = true ;
        _layerZ[38]= 398.455;    _isTriggerLayer[38] = true ;
        _layerZ[39]= 403.135;    _isTriggerLayer[39] = true ;
        _layerZ[40]= 407.815;    _isTriggerLayer[40] = true ; 

        // BH
        unsigned fistBHlayer= (_nLayersEE+_nLayersFH+1);
        for(unsigned ilayer=0; ilayer<_nLayersBH; ilayer++) {
            _layerZ[ilayer+fistBHlayer] = 421.0 + 9.0*(ilayer);   // NOT EXACTLY CORRECT!!!
            std::cout << ilayer << " " << ilayer+fistBHlayer << " " << _layerZ[ilayer+fistBHlayer] << std::endl; 
            _isTriggerLayer[ilayer+fistBHlayer] = true;
        }

        /* wafer and TC */

        TFile fIn("test_triggergeom.root", "read");

        _wafers = new map< unsigned, geoWafer >*[_nEndcaps];
        for( unsigned iendcap=0; iendcap<_nEndcaps; iendcap++ )
            _wafers[iendcap] = new map< unsigned, geoWafer >[_nLayers];

        TTree* t = (TTree*)fIn.Get("hgcaltriggergeomtester/TreeTriggerCells");  
        unsigned nEnt = t->GetEntries();
        
        int   id         ;
        int   zside      ;
        int   subdet     ;
        int   layer      ;
        int   wafer      ;
        int   triggercell;
        float x          ;
        float y          ;
        float z          ;
        
        t->SetBranchAddress("id"         , &id         );
        t->SetBranchAddress("zside"      , &zside      );
        t->SetBranchAddress("subdet"     , &subdet     );
        t->SetBranchAddress("layer"      , &layer      );
        t->SetBranchAddress("wafer"      , &wafer      );
        t->SetBranchAddress("triggercell", &triggercell);
        t->SetBranchAddress("x"          , &x          );
        t->SetBranchAddress("y"          , &y          );
        t->SetBranchAddress("z"          , &z          );
     
        
        for( unsigned ient=0; ient<nEnt; ient++ ) {
            
            t->GetEntry(ient);
            
            geoTC tc;
            tc.id          = id         ;
            tc.zside       = zside      ;
            tc.subdet      = subdet     ;
            tc.layer       = layer      ;
            tc.wafer       = wafer      ;
            tc.triggercell = triggercell;
            tc.x           = x          ;
            tc.y           = y          ;
            tc.z           = z          ;
            
            int iendcap = zside > 0 ? 0 : 1;
            
            if( _wafers[iendcap][layer].find( wafer ) == _wafers[iendcap][layer].end() ) {
                
                geoWafer w(tc);
                _wafers[iendcap][tc.absLayer()][wafer] = w;
                
            }
            else {
                
                _wafers[iendcap][tc.absLayer()][wafer].addTC( tc );
                
            }
            
        }
        
    }

    
    map< unsigned, geoWafer > getWafers( unsigned endcap, unsigned layer) {

        return _wafers[endcap][layer]; 
        
    }


    double getSectionStart(int iendcap, int isection) {
        if( iendcap == 0 ) {
            if( isection == 0 ) {
                return _layerZ[1]*endcapFactor(iendcap);            
            }
            else if( isection == 1 ) {
                return _layerZ[29]*endcapFactor(iendcap);
            }
            else if( isection == 2 ) {
                return _layerZ[41]*endcapFactor(iendcap);
            }
        }
        else if( iendcap == 1 ) {
            if( isection == 0 ) {
                return _layerZ[28]*endcapFactor(iendcap);            
            }
            else if( isection == 1 ) {
                return _layerZ[40]*endcapFactor(iendcap);
            }
            else if( isection == 2 ) {
                return _layerZ[52]*endcapFactor(iendcap);
            }
        }
        else {
            std::cout << " HGCgeom >>> Endcap not recognised: " << iendcap << std::endl;
        }
        
        return -1;
            
    }


    double getSectionEnd(int iendcap, int isection) {

         if( iendcap == 0 ) {
            if( isection == 0 ) {
                return _layerZ[28]*endcapFactor(iendcap);            
            }
            else if( isection == 1 ) {
                return _layerZ[40]*endcapFactor(iendcap);
            }
            else if( isection == 2 ) {
                return _layerZ[52]*endcapFactor(iendcap);
            }
        }
        else if( iendcap == 1 ) {
            if( isection == 0 ) {
                return _layerZ[1]*endcapFactor(iendcap);            
            }
            else if( isection == 1 ) {
                return _layerZ[29]*endcapFactor(iendcap);
            }
            else if( isection == 2 ) {
                return _layerZ[41]*endcapFactor(iendcap);
            }
        }
        else {
            std::cout << " HGCgeom >>> Endcap not recognised: " << iendcap << std::endl;
        }
        
        return -1;

    }
    

    double layerZ( int iendcap, int ilayer ) {
            
        return _layerZ[ilayer]*endcapFactor(iendcap);

    }

    
    bool layerZisTriggerLayer( unsigned iendcap, unsigned ilayer ){

        if( ilayer>_nLayers )
            return false;

        return _isTriggerLayer[ilayer];

    }
    
    
    // returns the zside of for the endcap 0==positive 1=negative
    int endcapFactor(int iendcap){
        
        if( iendcap == 0 )
            return 1;
        return -1;
        
    }
    
    unsigned nEndcaps () { return _nEndcaps ; }
    unsigned nLayersEE() { return _nLayersEE; }
    unsigned nLayersFH() { return _nLayersFH; }
    unsigned nLayersBH() { return _nLayersBH; }
    unsigned nLayers  () { return _nLayers  ; }  
    
private:
    
    double  *_layerZ;
    bool    *_isTriggerLayer;
    unsigned _nEndcaps;
    unsigned _nLayersEE;
    unsigned _nLayersFH;
    unsigned _nLayersBH;
    unsigned _nLayers;  

    map< unsigned, geoWafer > **_wafers;
    
};

#endif
