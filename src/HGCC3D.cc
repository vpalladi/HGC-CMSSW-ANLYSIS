
/* include the .h */
#include "HGCC3D.h"

ClassImp( HGCC3D )

HGCC3D::HGCC3D()  { ; }
HGCC3D::~HGCC3D() { ; }

/* set */
void HGCC3D::setId(unsigned id)                     { _id = id; }   
void HGCC3D::setPt(float pt)                        { _pt = pt; }
void HGCC3D::setEnergy(float energy)                { _energy = energy; }
void HGCC3D::setEta(float eta)                      { _eta = eta; }
void HGCC3D::setPhi(float phi)                      { _phi = phi; }
void HGCC3D::setClusters(vector<unsigned> clusters) { _clusters = clusters; }
void HGCC3D::setCells(vector<unsigned> cells)       { _cells = cells; }

/* get */
unsigned         HGCC3D::id()                       { return _id; }   
float            HGCC3D::Pt()                       { return _pt; }
float            HGCC3D::Energy()                   { return _energy; }
float            HGCC3D::Eta()                      { return _eta; }
float            HGCC3D::Phi()                      { return _phi; }
unsigned         HGCC3D::nclusters()                { return _clusters.size(); }
vector<unsigned> HGCC3D::clusters()                 { return _clusters; }

unsigned         HGCC3D::ncells()                   { return _cells.size(); }
vector<unsigned> HGCC3D::cells()                    { return _cells; }

TLorentzVector   HGCC3D::P4(){

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
