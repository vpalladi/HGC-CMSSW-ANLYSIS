#ifndef HGCC2D_H
#define HGCC2D_H 1

/* c/c++ */
#include <iostream>
#include <map>     // needed to correctly genreate dictionary
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

using namespace std;


class HGCC2D : public TObject {

 public:

    HGCC2D();
    ~HGCC2D();

    /* set C2D parameters */
    void setId(unsigned id)               ;
    void setSubdet(int subdet)            ;
    void setPt(float pt)                  ;
    void setEnergy(float energy)          ;
    void setEta(float eta)                ;
    void setPhi(float phi)                ;
    void setX(float x)                    ;
    void setY(float y)                    ;
    void setZ(float z)                    ;
    void setLayer(int layer)              ;
    void setCells(vector<unsigned> cells) ;
                                          
    /* get C2D parameters */    
    unsigned         id()                 ;
    int              subdet()             ; // 3==EE; 4==FH; 5==BH.
    float            Pt()                 ;
    float            Energy()             ;
    float            Eta()                ;
    float            Phi()                ;
    float            x()                  ;
    float            y()                  ;
    float            z()                  ;
    float            r()                  ;
    unsigned         layer()              ;
    unsigned         nCells()             ;
    vector<unsigned> cells()              ;
    unsigned         getEndcapId()        ;
    unsigned         getSectionId()       ; // 0==EE; 1==FH; 2==BH.


    /* the layer is corrected using the subdet and is progressive STARTING form 1 and not 0 */
    int correctedLayer();

    /* HGCROC info*/
    unsigned HGCROCn();
    TVector3 centre();

    /* get useful info */
    bool isTCcontained( HGCC2D tc );
    void getEtaSpan( double &minEta, double &maxEta );
    void getPhiSpan( double &minPhi, double &maxPhi );
     TLorentzVector P4();
  
    /* print to stdout some useful info */
    void print();

 private:
    double _sectionStart[3];
    double _sectionEnd[3];
    
    unsigned int         _id     ;
    int                  _subdet ;
    float                _pt     ;
    float                _energy ;
    float                _eta    ;
    float                _theta    ;
    float                _phi    ;
//    float                _x      ;
//    float                _y      ;
    float                _z      ;
    int                  _layer  ;
    unsigned             _ncells ;
    vector<unsigned>     _cells  ; // vector of HGCDetId for all the TC 

    ClassDef(HGCC2D, 1)

};

#endif 
