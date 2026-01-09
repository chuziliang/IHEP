#include "TH1.h"
#include "TMath.h"
#include "TF1.h"
#include "TLegend.h"
#include "TCanvas.h"
 
// Sum of background and peak function
Double_t fitFunction(Double_t *x, Double_t *par) {
  return par[0] + par[1]*x[0] + par[2]*x[0]*x[0]
  + par[3]*x[0]*x[0]*x[0]
  + par[4]*x[0]*x[0]*x[0]*x[0]
  + par[5]*x[0]*x[0]*x[0]*x[0]*x[0]
  + par[6]*x[0]*x[0]*x[0]*x[0]*x[0]*x[0]
  + par[7]*x[0]*x[0]*x[0]*x[0]*x[0]*x[0]*x[0]
  ;
}
 
void FittingDemo() {
 //Bevington Exercise by Peter Malzacher, modified by Rene Brun
 
   const int nBins = 60;
 
   Double_t data[nBins] = { 6, 1,10,12, 6,13,23,22,15,21,
                           23,26,36,25,27,35,40,44,66,81,
                           75,57,48,45,46,41,35,36,53,32,
                           40,37,38,31,36,44,42,37,32,32,
                           43,44,35,33,33,39,29,41,32,44,
                           26,39,29,35,32,21,21,15,25,15};
   TCanvas *c1 = new TCanvas("c1","Fitting Demo",10,10,700,500);
   c1->SetFillColor(33);
   c1->SetFrameFillColor(41);
   c1->SetGrid();
 
   TH1F *histo = new TH1F("histo",
      "Lorentzian Peak on Quadratic Background",60,0,3);
   histo->SetMarkerStyle(21);
   histo->SetMarkerSize(0.8);
 
   for(int i=0; i < nBins;  i++) histo->SetBinContent(i+1,data[i]-(0.5*1.38074e+01*1.72309e-01/TMath::Pi()) /
    TMath::Max( 1.e-10,(i/20.0-9.87281e-01)*(i/20.0-9.87281e-01)
   + .25*1.72309e-01*1.72309e-01));
 
   // create a TF1 with the range from 0 to 3 and 6 parameters
   TF1 *fitFcn = new TF1("fitFcn",fitFunction,0,3,8);
   fitFcn->SetNpx(500);
   fitFcn->SetLineWidth(4);
   fitFcn->SetLineColor(kMagenta);
 
   // first try without starting values for the parameters
   // This defaults to 1 for each param.
   // this results in an ok fit for the polynomial function
   // however the non-linear part (lorenzian) does not
   // respond well.
   histo->Fit("fitFcn","V+","ep");
   gStyle->SetOptFit(1100);
 
}