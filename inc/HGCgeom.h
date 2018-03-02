
#ifndef HGCgeom_HH
#define HGCgeom_HH

#include <iostream>

/*
  
  units:

  - distances: [cm]


*/

class HGCgeom {

private:

    static HGCgeom *s_instance;
    
    HGCgeom() {  /* constructor */
        initialise();
    }    

public:

    ~HGCgeom() {}
    
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
        _layerZ[ 1]= 320.755;    _isTriggerLayer[ 1] = false;
        _layerZ[ 2]= 321.505;    _isTriggerLayer[ 2] = true;
        _layerZ[ 3]= 322.735;    _isTriggerLayer[ 3] = false;
        _layerZ[ 4]= 323.485;    _isTriggerLayer[ 4] = true;
        _layerZ[ 5]= 324.715;    _isTriggerLayer[ 5] = false;
        _layerZ[ 6]= 325.465;    _isTriggerLayer[ 6] = true;
        _layerZ[ 7]= 326.695;    _isTriggerLayer[ 7] = false;
        _layerZ[ 8]= 327.445;    _isTriggerLayer[ 8] = true;
        _layerZ[ 9]= 328.675;    _isTriggerLayer[ 9] = false;
        _layerZ[10]= 329.425;    _isTriggerLayer[10] = true;
        _layerZ[11]= 330.735;    _isTriggerLayer[11] = false;
        _layerZ[12]= 331.605;    _isTriggerLayer[12] = true;
        _layerZ[13]= 332.915;    _isTriggerLayer[13] = false;
        _layerZ[14]= 333.785;    _isTriggerLayer[14] = true;
        _layerZ[15]= 335.095;    _isTriggerLayer[15] = false;
        _layerZ[16]= 335.965;    _isTriggerLayer[16] = true;
        _layerZ[17]= 337.275;    _isTriggerLayer[17] = false;
        _layerZ[18]= 338.145;    _isTriggerLayer[18] = true;
        _layerZ[19]= 339.455;    _isTriggerLayer[19] = false;
        _layerZ[20]= 340.325;    _isTriggerLayer[20] = true;
        _layerZ[21]= 341.775;    _isTriggerLayer[21] = false;
        _layerZ[22]= 342.845;    _isTriggerLayer[22] = true;
        _layerZ[23]= 344.295;    _isTriggerLayer[23] = false;
        _layerZ[24]= 345.365;    _isTriggerLayer[24] = true;
        _layerZ[25]= 346.815;    _isTriggerLayer[25] = false;
        _layerZ[26]= 347.885;    _isTriggerLayer[26] = true;
        _layerZ[27]= 349.335;    _isTriggerLayer[27] = false;
        _layerZ[28]= 350.405;    _isTriggerLayer[28] = true;
        // FH                   _                       
        _layerZ[29]= 356.335;    _isTriggerLayer[29] = true;
        _layerZ[30]= 361.015;    _isTriggerLayer[30] = true;
        _layerZ[31]= 365.695;    _isTriggerLayer[31] = true;
        _layerZ[32]= 370.375;    _isTriggerLayer[32] = true;
        _layerZ[33]= 375.055;    _isTriggerLayer[33] = true;
        _layerZ[34]= 379.735;    _isTriggerLayer[34] = true;
        _layerZ[35]= 384.415;    _isTriggerLayer[35] = true;
        _layerZ[36]= 389.095;    _isTriggerLayer[36] = true;
        _layerZ[37]= 393.775;    _isTriggerLayer[37] = true;
        _layerZ[38]= 398.455;    _isTriggerLayer[38] = true;
        _layerZ[39]= 403.135;    _isTriggerLayer[39] = true;
        _layerZ[40]= 407.815;    _isTriggerLayer[40] = true; 

        // BH
        unsigned fistBHlayer= (_nLayersEE+_nLayersFH+1);
        for(unsigned ilayer=0; ilayer<_nLayersBH; ilayer++) {
            _layerZ[ilayer+fistBHlayer] = 421.0 + 9.0*(ilayer);   // NOT EXACTLY CORRECT!!!
            std::cout << ilayer << " " << ilayer+fistBHlayer << std::endl; 
            _isTriggerLayer[ilayer] = true;
        }

        //std::cout << " *** GEOMETRY *** " << std::endl;
        //for(unsigned ilayer=1; ilayer<nLayers; ilayer++) { 
        //
        //    std::cout << " Layer " << ilayer << " is at " << layerZ[ilayer];
        //    if( isTriggerLayer[ilayer] )  std::cout << " and is a trigger layer"     << std::endl;
        //    else                           std::cout << " and is NOT a trigger layer" << std::endl;
        //      
        //}
   
    }

    double getSectionStarts(int iendcap, int isection) {
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


    double getSectionEnds(int iendcap, int isection) {

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
    
};

#endif
