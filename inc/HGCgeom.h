#ifndef HGCgeom_HH
#define HGCgeom_HH

#include <iostream>

using namespace std;

/*

  units:

  - distances: [cm]


*/


class HGCgeom {
    
public:
    
    enum {
        kNendcaps=2,
        kNlayersEE=28,
        kNlayersFH=12,
        kNlayersBH=12,
        kNlayers=kNlayersEE + kNlayersFH + kNlayersBH
    };  
    
    HGCgeom() {  /* constructor */
        initialise();
    }
    
    ~HGCgeom() {  /* destructor */
    }
  
    static void initialise() {
      
        // no layer called 0 
        _layerZ[ 0]= 0      ;    _isTriggerLayer[ 0] = false;
        //EE
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
        // FH                                          
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
        unsigned fistBHlayer= (kNlayersEE+kNlayersFH+1);
        for(unsigned ilayer=fistBHlayer; ilayer<=kNlayers; ilayer++) {
            _layerZ[ilayer] = 421.0 + 9.0*ilayer;   // NOT EXACTLY CORRECT!!!
            _isTriggerLayer[ilayer] = true;
        }

        cout << " *** GEOMETRY *** " << endl;
        for(unsigned ilayer=1; ilayer<kNlayers; ilayer++) { 
       
            cout << " Layer " << ilayer << " is at " << _layerZ[ilayer];
            if( _isTriggerLayer[ilayer] )  cout << " and is a trigger layer" << endl;
            else                           cout << " and is NOT a trigger layer" << endl;
              
        }
   
    }


    static bool getLayerZ(unsigned layerId) {
      
        return _layerZ[layerId];
  
    }

  
    static bool isTriggerLayer(unsigned layerId) {
      
        return _isTriggerLayer[layerId];
  
    }


private:

    static double _layerZ[kNlayers+1];
    static bool   _isTriggerLayer[kNlayers+1];

};

double HGCgeom::_layerZ[kNlayers+1];
bool   HGCgeom::_isTriggerLayer[kNlayers+1];



#endif
