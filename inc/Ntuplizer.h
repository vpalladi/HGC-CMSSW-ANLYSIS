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
  
  void fillTree(unsigned nEvt);
  TTree* getTree() {return _tree;}

  
 private:

  TTree* _tree;
  HGC* _detector;

  vector<float> _tc_pt;
  vector<int> _tc_HGClayer;
  vector<int> _tc_cl; //index of C2d in which tc included
  vector<float> _tc_dR_cl; //DeltaR wrt C2d in which tc included
  vector<int> _tc_c3d; //index of C2d in which tc included
  vector<float> _tc_dR_c3d; //DeltaR wrt C2d in which tc included

  vector<int> _cl_HGClayer;
  vector<int> _cl_cl3d; //index of C3d in which cl included

  void clear();


};


#endif

