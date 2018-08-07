#ifndef PLOTRATIO_H
#define PLOTRATIO_H


#include <iostream>

#include <TROOT.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TPad.h>
#include <TH1D.h>
#include <TLegend.h>


int plotRatio(TH1D *h1, TH1D *h2, std::string canvasName, char operation='/', bool logY=false){

    TCanvas *c =  new TCanvas(canvasName.c_str(), canvasName.c_str(), 1);

    Int_t OptStat = gStyle->GetOptStat();
    Int_t OptTitle = gStyle->GetOptTitle();
    
    gStyle->SetOptStat(0);
    gStyle->SetOptTitle(0);

    TPad *pUpper = new TPad("upperPad", "upperPad", 
                            0, 0.3, 
                            1, 1
        );
    
    TPad *pBottom = new TPad("bottomPad", "bottomPad", 
                             0, 0, 
                             1, 0.3
        ); 
    
    pUpper->SetBottomMargin(0);
    pBottom->SetTopMargin(0);
    pBottom->SetBottomMargin(0.4);

    pUpper->Draw();
    pBottom->Draw();

    c->Update();
    
    pUpper->cd();
    h1->Draw();
    h2->Draw("same");

    if(logY)
        pUpper->SetLogy();

    TLegend leg(0.7, 0.7, 0.9, 0.9);
    leg.AddEntry(h1);
    leg.AddEntry(h2);
    
    leg.Draw();


    TH1D *hRatio = (TH1D*)h1->Clone();

    hRatio->GetYaxis()->UnZoom();    

    pBottom->cd();
    if(    operation == '/')  {   
        hRatio->Divide(h2);       
        hRatio->GetYaxis()->SetRangeUser(0, 5);    
    }
    else if(operation == '*') {   hRatio->Multiply(h2);  }
    else if(operation == '+') {   hRatio->Add(h2);       }
    else if(operation == '-') {   hRatio->Add(h2, -1);   }
    else                      {   return -1;             }

    hRatio->GetYaxis()->SetNdivisions(305);
    hRatio->GetYaxis()->SetLabelSize(0.09);
    hRatio->GetXaxis()->SetLabelSize(0.1);
    hRatio->SetLineColor(kBlack);

    hRatio->Draw();

    c->SaveAs( (canvasName+".png").c_str() );
    c->SaveAs( (canvasName+".pdf").c_str() );
    
    gStyle->SetOptStat(OptStat);
    gStyle->SetOptTitle(OptTitle);

    delete c;
    
    return 0;

}

#endif

