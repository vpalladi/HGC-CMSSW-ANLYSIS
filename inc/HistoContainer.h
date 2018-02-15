
#ifndef HISTOCONTAINER_H
#define HISTOCONTAINER_H 1

/* C++ */
#include "memory"

/* ROOT */
#include "TList.h"
#include "TString.h"



//enum HistoType {
//    TH1, 
//    TH1I,
//    TH1F,
//    TH1D,
//    TH2, 
//    TH2I,
//    TH2F,
//    TH2D,       
//};


class histoContainerGeneric{
    
public:

    virtual void Draw() = 0;

};


template <class HistoType>
class histoContainer : public histoContainerGeneric {

public:


    histoContainer( HistoType* histo, TString Xlabel, TString Ylabel, TString opt){
        _obj = histo;
        _Xlable = Xlabel;
        _Ylable = Ylabel;
        _opt = opt;
    }

    void Draw() {
        _obj->GetXaxis()->SetTitle(_Xlable);
        _obj->GetYaxis()->SetTitle(_Ylable);
        _obj->Draw(_opt);
    }

private:

    HistoType* _obj;
    TString _Xlable;
    TString _Ylable;
    TString _opt;

};




#endif

