#ifndef HGCHIT_H
#define HGCHIT_H 1

/* c/c++ */
#include <iostream>
#include <map>     // needed to correctly generate dictionary
#include <vector>
#include <utility> // pair

/* mylibs */
#include "detId.h"
#include "HGCgeom.h"

/* ROOT */
#include "TObject.h"
#include "TMath.h"
#include "TVector3.h"
#include "TLorentzVector.h"

//using namespace std;


class HGChit : public TObject {

 public:

    HGChit();
    ~HGChit();

    /* set hit parameters */
    void setId(unsigned id)               ;
    void setSubdet(int subdet)            ;
    void setEnergy(float energy)          ;
    void setEta(float eta)                ;
    void setPhi(float phi)                ;
    void setX(float x)                    ;
    void setY(float y)                    ;
    void setZ(float z)                    ;
    void setLayer(int layer)              ;    
    void setPt(float pt)                  ;

    /* get hit parameters */    
    unsigned         id()                 ;
    int              subdet()             ; // 3==EE; 4==FH; 5==BH.
    float            Energy()             ;
    float            Eta()                ;
    float            Phi()                ;
    float            x()                  ;
    float            y()                  ;
    float            z()                  ;
    float            r()                  ;
    unsigned         layer()              ;
    unsigned         getEndcapId()        ;
    unsigned         getSectionId()       ; // 0==EE; 1==FH; 2==BH.
    float            Theta()              ;
    

    /* the layer is corrected using the subdet and is progressive STARTING form 1 and not 0 */
    int correctedLayer();

    /* info*/
    TVector3 centre();

    /* get useful info */
//    bool isTCcontained( HGChit::HGChit tc );
//    void getEtaSpan( double &minEta, double &maxEta );
//    void getPhiSpan( double &minPhi, double &maxPhi );
    TLorentzVector P4();
    float          Pt();
  
    /* print to stdout some useful info */
    void print();

    /* operators overloading */
    bool operator==( const HGChit &comp );

 private:

//    double _sectionStart[3];
//    double _sectionEnd[3];

    /* direct */
    unsigned int         _id     ;
    int                  _subdet ;
    float                _pt     ;
    float                _energy ;
    float                _eta    ;
    float                _phi    ;
    float                _x      ;
    float                _y      ;
    float                _z      ;
    int                  _layer  ;

    /* derived */
    float                _theta  ;

    ClassDef(HGChit, 1)

};

#endif 
