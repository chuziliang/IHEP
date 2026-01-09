#include "/junofs/users/chuziliang125/CATIROCsimulation/compiable_version/shaper.cc"

void AppendSignal()
{
    TFile *ifile = TFile::Open("./Signals.root");
    
    TH1 *tmp = nullptr; 
    TH1 *tmp2 = nullptr;
    TH1 *tmpFSH = nullptr;
    TH1 *tmpSSH = nullptr;
    TH1 *tmp2FSH = nullptr;
    TH1 *tmp2SSH = nullptr;
    //ifile->cd("0");
    double charge = 0;
    tmp = (TH1*)ifile->Get(TString::Format("%1d/signal %1d",0,3));
    
    tmp2 = (TH1*)tmp->Clone();
    // tmp2->GetXaxis()->SetCanExtend(kTRUE);
    // tmp2->ExtendAxis(2000,tmp2->GetXaxis());
    tmp2->SetBins(2000,0,2000);

    shaper(tmp,tmpFSH,tmpSSH);
    shaper(tmp2,tmp2FSH,tmp2SSH);

    TCanvas *c1 = new TCanvas("c1","c1",1);
    c1->Divide(2,1);
    c1->cd(1);
    tmp->Draw();
    c1->cd(2);
    tmp2->Draw();
    TCanvas *c2 = new TCanvas("c2","c2",1);
    c2->Divide(2,1);
    c2->cd(1);
    tmpFSH->Draw("HIST L");
    c2->cd(2);
    tmp2FSH->Draw("HIST L");
    TCanvas *c3 = new TCanvas("c3","c3",1);
    c3->Divide(2,1);
    c3->cd(1);
    tmpSSH->Draw("HIST L");
    c3->cd(2);
    tmp2SSH->Draw("HIST L");
}