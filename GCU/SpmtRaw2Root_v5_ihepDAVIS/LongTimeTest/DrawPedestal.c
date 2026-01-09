void DrawPedestal()
{
    ifstream FitPedestalResult;
    FitPedestalResult.open("FitPedestalResult.txt",ios::ate);

    Double_t channelNum[2][128];
    Double_t Pedestal[2][128];
    Double_t Sigma[2][128];
    //string nun;name;
    FitPedestalResult.seekg(-2,FitPedestalResult.cur);
    while(FitPedestalResult.peek() != FitPedestalResult.widen('\n'))
    {
        FitPedestalResult.seekg(-1,FitPedestalResult.cur);
    }
    while (!FitPedestalResult.eof())
    {

       // FitPedestalResult>>nun>>nun>>name;
        
        for (int i = 0; i<128; i++)
        {
            FitPedestalResult>>Pedestal[0][i]>>Sigma[0][i];
            channelNum[0][i] = i+1;
        }
        for (int i = 0; i < 128; i++)
        {
            FitPedestalResult>>Pedestal[1][i]>>Sigma[1][i];
            channelNum[1][i] = i+1;
        }
        TGraph *p0Pedestal = new TGraph(128,channelNum[0],Pedestal[0]);
        p0Pedestal->SetTitle("P0 Pedestal");
        p0Pedestal->GetXaxis()->SetTitle("Channel Number");
        p0Pedestal->SetMarkerStyle(8);
        p0Pedestal->SetMarkerSize(0.7);
        TGraph *p0Sigma = new TGraph(128,channelNum[0],Sigma[0]);
        p0Sigma->SetTitle("P0 Sigma");
        p0Sigma->GetXaxis()->SetTitle("Channel Number");
        p0Sigma->SetMarkerStyle(8);
        p0Sigma->SetMarkerSize(0.7);
        TGraph *p1Pedestal = new TGraph(128,channelNum[1],Pedestal[1]);
        p1Pedestal->SetTitle("P1 Pedestal");
        p1Pedestal->GetXaxis()->SetTitle("Channel Number");
        p1Pedestal->SetMarkerStyle(8);
        p1Pedestal->SetMarkerSize(0.7);
        TGraph *p1Sigma = new TGraph(128,channelNum[1],Sigma[1]);
        p1Sigma->SetTitle("P1 Sigma");
        p1Sigma->GetXaxis()->SetTitle("Channel Number");
        p1Sigma->SetMarkerStyle(8);
        p1Sigma->SetMarkerSize(0.7);
        TCanvas *c1 = new TCanvas("c1","c1",1);
        c1->Divide(2,2);
        c1->cd(1);
        p0Pedestal->Draw("AP");
        c1->cd(2);
        p0Sigma->Draw("AP");
        c1->cd(3);
        p1Pedestal->Draw("AP");
        c1->cd(4);
        p1Sigma->Draw("AP");
    } 
}