#ifndef HGC_LINE
#define HGC_LINE

// c++
#include <iostream>

// ROOT
#include <TF1.h>

class HGCline{

public:

    HGCline() {};

    HGCline(float m, float y0){ setParams(m, y0); }
        
    void setParams(float m, float y0){
        _m=m; 
        _y0=y0;
    }

    float eval(float x){
        return (_m*x+_y0);
    }

    TF1 getTF1(float min, float max){
        
        TF1 f("f", "-[0]*x+[1]", min, max);
        f.SetParameter( 0, _m  );
        f.SetParameter( 1, _y0 );

        return f;
    }


private:

    float _m;
    float _y0;

};



#endif
