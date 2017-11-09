
/* include the .h */
#include "HGCC3D.h"


HGCC3D::HGCC3D() {};



TLorentzVector HGCC3D::P4(){

  TLorentzVector p4;
  p4.SetPtEtaPhiE(_pt,_eta,_phi,_energy);
  return p4;

}


void HGCC3D::print() {

    cout << "-------------" << endl;
    cout << " pt : " << _pt << endl
         << " ene: " << _energy << endl
         << " eta:" << _eta << endl
         << " phi:" << _phi << endl;        
    cout << "=============" << endl;

}
