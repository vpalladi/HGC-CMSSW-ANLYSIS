#ifndef NTUPLIZER_H
#define NTUPLIZER_H 1

/* c/c++ */
#include <iostream>

/* mylibs */
#include "HGC.h"
#include "HGCTC.h"
#include "HGCC2D.h"

/* ROOT */
#include "TFile.h"
#include "TTree.h"

using namespace std;


class Ntuplizer {

 public:
  
  Ntuplizer(HGC* detector, TString treename);
  ~Ntuplizer();
  
  void fillTree();
  TTree* getTree() {return _tree;}

  void write();

 private:

  TFile* _fileOut;

  TTree* _tree;
  HGC* _detector;

  HGCTC*  _tc;
  HGCC2D* _c2d;

  void clear();

};


#endif

