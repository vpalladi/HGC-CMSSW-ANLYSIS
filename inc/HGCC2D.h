#ifndef HGCC2D_H
#define HGCC2D_H 1

/* c/c++ */
#include <iostream>
#include <map>     // needed to correctly genreate dictionary
#include <vector>
#include <utility> // pair

/* mylibs */
#include "HGChit.h"
#include "detId.h"
#include "HGCgeom.h"

/* ROOT */
#include "TObject.h"
#include "TMath.h"
#include "TVector3.h"
#include "TLorentzVector.h"

using namespace std;


class HGCC2D : public HGChit {

 public:

    HGCC2D();
    ~HGCC2D();

    /* set C2D parameters */
    void setCells(vector<unsigned> cells) ;
                                          
    /* get C2D parameters */    
    unsigned         nCells() const;
    vector<unsigned> cells() const ;

    /* HGCROC info*/
    unsigned HGCROCn();

    /* get useful info */
    bool isTCcontained( HGCC2D tc );
    void getEtaSpan( double &minEta, double &maxEta );
    void getPhiSpan( double &minPhi, double &maxPhi );

 private:

    unsigned             _ncells ;
    vector<unsigned>     _cells  ; // vector of HGCDetId for all the TC 

    ClassDef(HGCC2D, 1)

};

void GetKeys( const HGCC2D &ahit , uint32_t& aKey1 , uint32_t& aKey2 );

#endif 
