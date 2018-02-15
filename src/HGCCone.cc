
#include "HGCCone.h"

HGCCone::HGCCone(double aperture, double theta, double phi){
    
    aperture_ = aperture; 
    theta_ = theta; 
    phi_ = phi;

}

bool HGCCone::isContained(double x, double y, double z){
    
    if( cone(x, y, z) < 0 ) return true;
    
    return false;

}

double HGCCone::cone(double x, double y, double z){

    double A = ( ( x*TMath::Cos(phi_) - y*sin(phi_) ) * TMath::Cos(theta_) + z*TMath::Sin(theta_) );
    double B = x*TMath::Sin(phi_) + y*TMath::Cos(phi_);
    double C = ( -( x*TMath::Cos(phi_) - y*sin(phi_) ) * TMath::Sin(theta_) + z*TMath::Cos(theta_) );;

    double a = 1.;
    double b = 1.;
    double c = 1./TMath::Tan(aperture_);

    return (A*A)/(a*a) + (B*B)/(b*b) - (C*C)/(c*c);

}

