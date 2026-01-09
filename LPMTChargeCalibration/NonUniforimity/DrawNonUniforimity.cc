// 用于画出不同位置、不同能量的事例下，LPMT和SPMT的非均匀性对比
// 拟合时用一个参数

const double R = 19.434; // m
const double position[11] = {0,0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,0.95};
const int Points = 10;
const double Energy[5]={0.5,1.16,2,5,10};

void DrawNonUniforimity()
{
    TGraphErrors *npeLPMTtonpeSPMT[6][5]; // PMTID, EnergyNum
    TGraphErrors *ratio[6][5];
    vector<double> MeanRatio[6][5];
    vector<double> ErrorRatio[6][5];
    vector<double> MeanPE[6][5];
    vector<double> ErrorPE[6][5];
    for (int num = 0; num < 5; num++)
    {
        TFile *inputfile = TFile::Open(TString::Format("/junofs/users/chuziliang125/LPMTChargeCalibration/NonUniforimity/npeLPMTtonpeSPMT%.1fMeV.root",Energy[num]),"read");
        for (int PMTID = 0; PMTID < 6; PMTID++)
        {
            npeLPMTtonpeSPMT[PMTID][num] = (TGraphErrors*)inputfile->Get(TString::Format("LPMT npe to SPMT_%1d npe",PMTID));
            npeLPMTtonpeSPMT[PMTID][num]->SetName(TString::Format("LPMT npe to SPMT_%1d npe %.1fMeV",PMTID,Energy[num]));
            npeLPMTtonpeSPMT[PMTID][num]->SetMarkerColor(num+1);
            npeLPMTtonpeSPMT[PMTID][num]->SetLineColor(num+1);
            npeLPMTtonpeSPMT[PMTID][num]->GetListOfFunctions()->ls();
            TF1 *fun = npeLPMTtonpeSPMT[PMTID][num]->GetFunction("pol1");
            npeLPMTtonpeSPMT[PMTID][num]->GetListOfFunctions()->Remove(fun);

            for (int i = 0; i < npeLPMTtonpeSPMT[PMTID][num]->GetN(); i++)
            {
                double x = npeLPMTtonpeSPMT[PMTID][num]->GetPointX(i);
                double y = npeLPMTtonpeSPMT[PMTID][num]->GetPointY(i);
                double ex = npeLPMTtonpeSPMT[PMTID][num]->GetErrorX(i);
                double ey = npeLPMTtonpeSPMT[PMTID][num]->GetErrorY(i);
                if (x>0 & y>0)
                {
                    MeanRatio[PMTID][num].push_back(y/x);
                    ErrorRatio[PMTID][num].push_back((y/x)*sqrt((ex*ex)/(x*x)+(ey*ey)/(y*y)));
                    MeanPE[PMTID][num].push_back(x);
                    ErrorPE[PMTID][num].push_back(ex);
                }
                else
                {
                    npeLPMTtonpeSPMT[PMTID][num]->RemovePoint(i);
                }
                
            }
            ratio[PMTID][num] = new TGraphErrors(MeanRatio[PMTID][num].size(),&MeanPE[PMTID][num][0],&MeanRatio[PMTID][num][0],&ErrorPE[PMTID][num][0],&ErrorRatio[PMTID][num][0]);
            ratio[PMTID][num]->SetName(TString::Format("LPMT/SPMT  SPMT_%1d %.1fMeV",PMTID,Energy[num]));
            ratio[PMTID][num]->SetMarkerColor(num+1);
            ratio[PMTID][num]->SetLineColor(num+1);
        }
    }
    for (int PMTID = 0; PMTID < 6; PMTID++)
    {
        npeLPMTtonpeSPMT[PMTID][4]->SetMarkerColor(6);
        npeLPMTtonpeSPMT[PMTID][4]->SetLineColor(6);
        ratio[PMTID][4]->SetMarkerColor(6);
        ratio[PMTID][4]->SetLineColor(6);

        npeLPMTtonpeSPMT[PMTID][1]->SetMarkerColor(7);
        npeLPMTtonpeSPMT[PMTID][1]->SetLineColor(7);
        ratio[PMTID][1]->SetMarkerColor(7);
        ratio[PMTID][1]->SetLineColor(7);
    }
    
    


    TCanvas *Canvas[6];
    TMultiGraph *mg[6];
    TMultiGraph *Ratiomg[6]; 
    TPad *pad1[6];
    TPad *pad2[6];
    TF1 *f1[6];
    TF1 *f2[6];
    for (int PMTID = 0; PMTID < 6; PMTID++)
    {
        TString mgName = TString::Format("LPMT npe to SPMT_%1d npe",PMTID);
        mg[PMTID] = new TMultiGraph(mgName,mgName);
        Canvas[PMTID] = new TCanvas(TString::Format("C%1d",PMTID));
        f1[PMTID] = new TF1(TString::Format("f1%1d",PMTID),"pol1",0,3);
        

        pad1[PMTID] = new TPad("pad1", "pad1", 0.00, 0.32, 1.00, 0.95);
        pad2[PMTID] = new TPad("pad1", "pad1", 0.00, 0, 1.00, 0.32);
        pad1[PMTID]->SetMargin(0.12, 0.03, 0., 0.0);
        pad2[PMTID]->SetMargin(0.12, 0.03, 0.3, 0.0);
        pad1[PMTID]->Draw();
        pad2[PMTID]->Draw();
        pad1[PMTID]->cd();

        TLegend *legenda =new TLegend(0.2,0.55,0.4,0.85);
        legenda->SetTextFont(72);
        legenda->SetTextSize(0.06);
        legenda->SetLineColor(0);
        // npeLPMTtonpeSPMT[PMTID][4]->Draw("A L P");
        // legenda->AddEntry(npeLPMTtonpeSPMT[PMTID][4],TString::Format("%.1fMeV",Energy[4]));
        for (int num = 0; num < 5; num++)
        {
            mg[PMTID]->Add(npeLPMTtonpeSPMT[PMTID][num]);
            // npeLPMTtonpeSPMT[PMTID][num]->Draw("same L P");
            legenda->AddEntry(npeLPMTtonpeSPMT[PMTID][num],TString::Format("%.1fMeV",Energy[num]));
        }
        mg[PMTID]->SetMinimum(0.001);
        mg[PMTID]->GetYaxis()->SetRangeUser(0.01,120);
        mg[PMTID]->GetXaxis()->SetLimits(0.0001,3);
        mg[PMTID]->GetYaxis()->SetTitle("LPMT charge(p.e.)");
        mg[PMTID]->GetYaxis()->SetLabelSize(0.06);
        mg[PMTID]->GetYaxis()->SetTitleSize(0.06);
        mg[PMTID]->GetYaxis()->SetTitleOffset(0.65);
        mg[PMTID]->GetYaxis()->CenterTitle();
        gPad->SetLogx();
        gPad->SetLogy();
        gStyle->SetTitleFontSize(0.08);
        mg[PMTID]->Draw("A P");
        mg[PMTID]->Fit(f1[PMTID]);
        gStyle->SetOptFit(1111);
        
        // gStyle->SetStatY(0.5);
        // gPad->Update();
        // TPaveStats *s1 = (TPaveStats*)Canvas[PMTID]->GetPrimitive("stats");
        // s1->SetY1NDC(0.1); // new x start position
        // s1->SetY2NDC(0.4); // new x end position
        legenda->Draw();

        pad2[PMTID]->cd();
        TString RatiomgName = TString::Format("LPMT/SPMT_%1d",PMTID);
        Ratiomg[PMTID] = new TMultiGraph(RatiomgName,"");
        f2[PMTID] = new TF1(TString::Format("f2%1d",PMTID),"pol1",0,3);

        // RatioCanvas[PMTID] = new TCanvas(TString::Format("RC%1d",PMTID));

        // TLegend *legendb =new TLegend(0.6,0.65,0.88,0.85);
        // legendb->SetTextFont(72);
        // legendb->SetTextSize(0.04);
        // npeLPMTtonpeSPMT[PMTID][4]->Draw("A L P");
        // legenda->AddEntry(npeLPMTtonpeSPMT[PMTID][4],TString::Format("%.1fMeV",Energy[4]));
        for (int num = 0; num < 5; num++)
        {
            Ratiomg[PMTID]->Add(ratio[PMTID][num]);
            // npeLPMTtonpeSPMT[PMTID][num]->Draw("same L P");
            // legendb->AddEntry(ratio[PMTID][num],TString::Format("%.1fMeV",Energy[num]));
        }
        Ratiomg[PMTID]->SetMinimum(0.001);
        // Ratiomg[PMTID]->SetTitleSize(20);
        // Ratiomg[PMTID]->GetYaxis()->SetRangeUser(0.01,120);
        Ratiomg[PMTID]->GetXaxis()->SetLimits(0.0001,3);
        Ratiomg[PMTID]->GetXaxis()->SetTitle("#mu(p.e.)");
        Ratiomg[PMTID]->GetYaxis()->SetTitle("LPMT/SPMT");
        Ratiomg[PMTID]->GetYaxis()->SetTitleSize(0.10);
        Ratiomg[PMTID]->GetYaxis()->SetTitleOffset(0.38);
        Ratiomg[PMTID]->GetYaxis()->SetLabelSize(0.12);
        Ratiomg[PMTID]->GetYaxis()->SetNdivisions(4,5,0);
        // Ratiomg[PMTID]->GetYaxis()->SetLabelSize(0.07);
        Ratiomg[PMTID]->GetXaxis()->SetTitleSize(0.13);
        Ratiomg[PMTID]->GetXaxis()->SetTitleOffset(1.);
        Ratiomg[PMTID]->GetXaxis()->SetLabelSize(0.12);
        gPad->SetLogx();
        // gPad->SetLogy();
        Ratiomg[PMTID]->Draw("A P");
        Ratiomg[PMTID]->Fit(f2[PMTID]);
        gStyle->SetOptFit(1111);
        
        // gStyle->SetStatY(0.2);
        // legendb->Draw();
    }

    // TCanvas *RatioCanvas[6];
    // TMultiGraph *Ratiomg[6]; 

    // for (int PMTID = 0; PMTID < 6; PMTID++)
    // {
    //     TString RatiomgName = TString::Format("LPMT npe to SPMT_%1d npe",PMTID);
    //     Ratiomg[PMTID] = new TMultiGraph(RatiomgName,RatiomgName);
    //     RatioCanvas[PMTID] = new TCanvas(TString::Format("RC%1d",PMTID));

    //     TLegend *legendb =new TLegend(0.6,0.65,0.88,0.85);
    //     legendb->SetTextFont(72);
    //     legendb->SetTextSize(0.04);
    //     // npeLPMTtonpeSPMT[PMTID][4]->Draw("A L P");
    //     // legenda->AddEntry(npeLPMTtonpeSPMT[PMTID][4],TString::Format("%.1fMeV",Energy[4]));
    //     for (int num = 0; num < 5; num++)
    //     {
    //         Ratiomg[PMTID]->Add(ratio[PMTID][num]);
    //         // npeLPMTtonpeSPMT[PMTID][num]->Draw("same L P");
    //         legendb->AddEntry(ratio[PMTID][num],TString::Format("%.1fMeV",Energy[num]));
    //     }
    //     Ratiomg[PMTID]->SetMinimum(0.001);
    //     // Ratiomg[PMTID]->GetYaxis()->SetRangeUser(0.01,120);
    //     Ratiomg[PMTID]->GetXaxis()->SetLimits(0.0001,3);
    //     gPad->SetLogx();
    //     // gPad->SetLogy();
    //     Ratiomg[PMTID]->Draw("A L P");
    //     Ratiomg[PMTID]->Fit("pol1");
    //     gStyle->SetOptFit(1111);
    //     gStyle->SetStatY(0.5);
    //     legendb->Draw();
    // }

}