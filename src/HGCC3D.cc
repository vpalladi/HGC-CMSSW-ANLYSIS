
/* include the .h */
#include "HGCC3D.h"


HGCC3D::HGCC3D() {};


void HGCC3D::print() {

    cout << "-------------" << endl;
    cout << " pt : " << _pt << endl
         << " ene: " << _energy << endl
         << " eta:" << _eta << endl
         << " phi:" << _phi << endl;        
    cout << "=============" << endl;

}
