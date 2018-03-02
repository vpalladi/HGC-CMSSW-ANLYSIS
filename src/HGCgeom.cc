
#include "HGCgeom.h"


HGCgeom* HGCgeom::instance(){
    if (!s_instance) s_instance = new HGCgeom;
    return s_instance;
}

HGCgeom* HGCgeom::s_instance = 0;
