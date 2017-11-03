#ifndef NTUPLIZER_H
#define NTUPLIZER_H 1

/* c/c++ */
#include <iostream>

/* mylibs */
#include "detId.h"
#include "HGC.h"
#include "HGCTC.h"
#include "HGCC2D.h"
#include "HGCROC.h"

/* ROOT */
#include "TMath.h"
#include "TTree.h"
#include "TCollection.h"
#include "TList.h"


using namespace std;



class Ntuplizer {

 public:
  
  Ntuplizer(HGC* detector, TString treename);
  ~Ntuplizer();
  
  void fillTree(int nEvt);
  TTree* getTree() {return _tree;}

  
 private:

  TTree* _tree;
  HGC* _detector;

  vector<float> _tc_pt;
  vector<int> _tc_HGClayer;
  vector<int> _cl_HGClayer;

  void clear();


};


#endif

