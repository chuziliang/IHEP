{
  //Double_t energy = gRandom->Uniform(10.); // MeV
  //Double_t muL = 1.*energy;
  //Double_t muS = 0.02*energy;

  Double_t muL, muS;
  const Int_t n = 1000000; // total number of simulated events
  Double_t e[n], PEL[n], PES[n];
  for(Int_t i=0;i<n;i++) {
    //e[i] = gRandom->Uniform(100.); // 0-10 MeV uniformly distributed events
    e[i] = gRandom->Exp(5);
    muL = 1.*e[i];  // assuming average 1 PE on an LPMT
    muS = 0.02*e[i]; // assuming average 0.02 PE on an SPMT
    PEL[i] = gRandom->Poisson(muL);
    PES[i] = gRandom->Poisson(muS);
  }

  TFile f("npe.root","recreate");
  n1 = TNtuple("n1","n1","e:PEL:PES");
  for(Int_t i=0;i<n;i++) {
    n1.Fill(e[i],PEL[i],PES[i]);
  }
  f.Write();

  // binning with nPE
  const Int_t m=20; // 1, 2, ..., 20 PE for LPMT
  TH1F spmt[m];
  TH1F lpmt[m];
  for(Int_t j=0;j<m;j++) {
    TString strs("hs");
    strs += j;
    spmt[j] = TH1F(strs,strs,10,0,10);
    TString strl("hl");
    strl += j;
    lpmt[j] = TH1F(strl,strl,20,1,21);
  }

  for(Int_t i=0;i<n;i++) {
    for(Int_t j=0;j<m;j++) {
      if(PEL[i]>j+0.5&&PEL[i]<j+1.5) {
        spmt[j].Fill(PES[i]);
        lpmt[j].Fill(PEL[i]);
        //if(j==m-1) cout << "i=" << i << ",j=" << j << "," << PEL[i] << endl;
      }
    }
  }

  // define and fill average PE for LPMT and SPMT
  Double_t calibL[m],calibLerr[m],calibS[m],calibSerr[m];
  for(Int_t j=0;j<m;j++) {
    calibL[j]=lpmt[j].GetMean();
    calibLerr[j]=lpmt[j].GetMeanError();
    calibS[j]=spmt[j].GetMean();
    calibSerr[j]=spmt[j].GetMeanError();
    cout<<lpmt[j].GetMean()<<","<<lpmt[j].GetMeanError()<<";   " << spmt[j].GetMean()<<","<<spmt[j].GetMeanError()<<endl;
  }

  gr1 = new TGraphErrors(m,calibL,calibS,calibLerr,calibSerr);
  gr1->Draw("AP");
  gr1->GetYaxis()->SetRangeUser(0,0.5);
  gr1->GetYaxis()->SetTitle("#mu(nPE_{S})"); 
  gr1->GetXaxis()->SetTitle("nPE_{L}"); 
  gr1->SetTitle("Binning with nPE");
  gr1->SetMarkerColor(kBlue);
  gr1->SetLineColor(kBlue);
  gr1->SetMarkerStyle(20);

  auto fa1 = new TF1("fa1","0.02*x",0,20);
  fa1->Draw("same");

  // binning with energy
  const Int_t mm=10;
  TH1F spmt2[mm];
  TH1F lpmt2[mm];
  for(Int_t j=0;j<mm;j++) {
    TString strs2("hs2");
    strs2 += j;
    spmt2[j] = TH1F(strs2,strs2,10,0,10);
    TString strl2("hl2");
    strl2 += j;
    lpmt2[j] = TH1F(strl2,strl2,30,0,30);
  }

  for(Int_t i=0;i<n;i++) {
    for(Int_t j=0;j<mm;j++) {
      if(e[i]>j&&e[i]<j+1) {
        spmt2[j].Fill(PES[i]);
        lpmt2[j].Fill(PEL[i]);
        //if(j==m-1) cout << "i=" << i << ",j=" << j << "," << PEL[i] << endl;
      }
    }
  }

  Double_t calibL2[mm],calibLerr2[mm],calibS2[mm],calibSerr2[mm];
  for(Int_t j=0;j<mm;j++) {
    calibL2[j]=lpmt2[j].GetMean();
    calibLerr2[j]=lpmt2[j].GetMeanError();
    calibS2[j]=spmt2[j].GetMean();
    calibSerr2[j]=spmt2[j].GetMeanError();
    cout<<lpmt2[j].GetMean()<<","<<lpmt2[j].GetMeanError()<<";   " << spmt2[j].GetMean()<<","<<spmt2[j].GetMeanError()<<endl;
  }

  new TCanvas();
  gr2 = new TGraphErrors(mm,calibL2,calibS2,calibLerr2,calibSerr2);
  gr2->GetYaxis()->SetRangeUser(0,0.2);
  gr2->GetYaxis()->SetTitle("#mu(nPE_{S})"); 
  gr2->GetXaxis()->SetTitle("#mu(nPE_{L})"); 
  gr2->SetTitle("Binning with energy");
  gr2->SetMarkerColor(kBlue);
  gr2->SetLineColor(kBlue);
  gr2->SetMarkerStyle(20);
  gr2->Draw("AP");

  auto fa2 = new TF1("fa2","[0]*x",0,20);
  gr2->Fit("fa2","R");
  //c1 = new TCanvas();
  //n1.Draw("PEL");

  //c2 = new TCanvas();
  //n1.Draw("PES");

  //c3 = new TCanvas();
  //n1.Draw("PEL:PES");
}
