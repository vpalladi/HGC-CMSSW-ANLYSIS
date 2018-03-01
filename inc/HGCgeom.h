
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

        nEndcaps  = 2 ;
        nLayersEE = 28;
        nLayersFH = 12;
        nLayersBH = 12;
        nLayers   = nLayersEE + nLayersFH + nLayersBH;

        layerZ         = new double[nLayers+1];
        isTriggerLayer = new bool[nLayers+1];
        // no layer called 0 
        layerZ[ 0]= 0      ;    isTriggerLayer[ 0] = false;
        //EE
        layerZ[ 1]= 320.755;    isTriggerLayer[ 1] = false;
        layerZ[ 2]= 321.505;    isTriggerLayer[ 2] = true;
        layerZ[ 3]= 322.735;    isTriggerLayer[ 3] = false;
        layerZ[ 4]= 323.485;    isTriggerLayer[ 4] = true;
        layerZ[ 5]= 324.715;    isTriggerLayer[ 5] = false;
        layerZ[ 6]= 325.465;    isTriggerLayer[ 6] = true;
        layerZ[ 7]= 326.695;    isTriggerLayer[ 7] = false;
        layerZ[ 8]= 327.445;    isTriggerLayer[ 8] = true;
        layerZ[ 9]= 328.675;    isTriggerLayer[ 9] = false;
        layerZ[10]= 329.425;    isTriggerLayer[10] = true;
        layerZ[11]= 330.735;    isTriggerLayer[11] = false;
        layerZ[12]= 331.605;    isTriggerLayer[12] = true;
        layerZ[13]= 332.915;    isTriggerLayer[13] = false;
        layerZ[14]= 333.785;    isTriggerLayer[14] = true;
        layerZ[15]= 335.095;    isTriggerLayer[15] = false;
        layerZ[16]= 335.965;    isTriggerLayer[16] = true;
        layerZ[17]= 337.275;    isTriggerLayer[17] = false;
        layerZ[18]= 338.145;    isTriggerLayer[18] = true;
        layerZ[19]= 339.455;    isTriggerLayer[19] = false;
        layerZ[20]= 340.325;    isTriggerLayer[20] = true;
        layerZ[21]= 341.775;    isTriggerLayer[21] = false;
        layerZ[22]= 342.845;    isTriggerLayer[22] = true;
        layerZ[23]= 344.295;    isTriggerLayer[23] = false;
        layerZ[24]= 345.365;    isTriggerLayer[24] = true;
        layerZ[25]= 346.815;    isTriggerLayer[25] = false;
        layerZ[26]= 347.885;    isTriggerLayer[26] = true;
        layerZ[27]= 349.335;    isTriggerLayer[27] = false;
        layerZ[28]= 350.405;    isTriggerLayer[28] = true;
        // FH                                          
        layerZ[29]= 356.335;    isTriggerLayer[29] = true;
        layerZ[30]= 361.015;    isTriggerLayer[30] = true;
        layerZ[31]= 365.695;    isTriggerLayer[31] = true;
        layerZ[32]= 370.375;    isTriggerLayer[32] = true;
        layerZ[33]= 375.055;    isTriggerLayer[33] = true;
        layerZ[34]= 379.735;    isTriggerLayer[34] = true;
        layerZ[35]= 384.415;    isTriggerLayer[35] = true;
        layerZ[36]= 389.095;    isTriggerLayer[36] = true;
        layerZ[37]= 393.775;    isTriggerLayer[37] = true;
        layerZ[38]= 398.455;    isTriggerLayer[38] = true;
        layerZ[39]= 403.135;    isTriggerLayer[39] = true;
        layerZ[40]= 407.815;    isTriggerLayer[40] = true; 

        // BH
        unsigned fistBHlayer= (nLayersEE+nLayersFH+1);
        for(unsigned ilayer=0; ilayer<nLayersBH; ilayer++) {
            layerZ[ilayer+fistBHlayer] = 421.0 + 9.0*(ilayer);   // NOT EXACTLY CORRECT!!!
            std::cout << ilayer << " " << ilayer+fistBHlayer << std::endl; 
            isTriggerLayer[ilayer] = true;
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

    double  *layerZ;
    bool    *isTriggerLayer;
    unsigned nEndcaps;
    unsigned nLayersEE;
    unsigned nLayersFH;
    unsigned nLayersBH;
    unsigned nLayers;  
    
};

#endif
