void DrawPedestalcopy()
{
    ifstream FitPedestalResult("Pedestal1222_1224.txt");

    double Pedestal;
    double Sigma;
    //string nun;name;
    TH1 *Pedestalhist1 = new TH1F("after","after",128,1,128);
    TH1 *Pedestalhist2 = new TH1F("before","before",128,1,128);
    TH1 *Pedestalhist3 = new TH1F("after2","after2",128,1,128);
    TH1 *Pedestalhist4 = new TH1F("before2","before2",128,1,128);
    //while (FitPedestalResult.good())
    {

       // FitPedestalResult>>nun>>nun>>name;
        
        for (int i = 0; i<128; i++)
        {
            FitPedestalResult >> Sigma >> Sigma >> Pedestal;
            cout<<Pedestal<<endl;
            Pedestalhist1->SetBinContent(i+1,Pedestal);
            Pedestal = 0;
        }
        for (int i = 0; i < 128; i++)
        {
            FitPedestalResult >> Sigma >> Sigma >> Pedestal;
            Pedestalhist3->SetBinContent(i+1,Pedestal);
        }
        for (int i = 0; i<128; i++)
        {
            FitPedestalResult >> Sigma >> Sigma >> Pedestal;
            Pedestalhist2->SetBinContent(i+1,Pedestal);
        }
        for (int i = 0; i < 128; i++)
        {
            FitPedestalResult >> Sigma >> Sigma >> Pedestal;
            Pedestalhist4->SetBinContent(i+1,Pedestal);
        }
    }
    TCanvas *C1 = new TCanvas("c1","c1",1);
    Pedestalhist1->Draw();
	C1->Update();
    TPaveStats* stats2 = (TPaveStats*)C1->GetPrimitive("stats");
	double y1 = stats2->GetY1NDC();
	double y2 = stats2->GetY2NDC();
	double newy1 = 2 * y1 - y2;   // new (lower) y start position of stats box
	double newy2 = y1;            // new (upper) y start position of stats box
	stats2->SetName("before");
	stats2->SetY1NDC(newy1);    //set new y start position
	stats2->SetY2NDC(newy2);    //set new y end position
    C1->Update();
    Pedestalhist2->Draw("same");
    C1->Update();
    TCanvas *C2 = new TCanvas("c2","c2",1);
    Pedestalhist3->Draw();
    Pedestalhist4->Draw("same");
        // TGraph *p0Pedestal = new TGraph(128,channelNum[0],Pedestal[0]);
        // p0Pedestal->SetTitle("P0 Pedestal");
        // p0Pedestal->GetXaxis()->SetTitle("Channel Number");
        // p0Pedestal->SetMarkerStyle(8);
        // p0Pedestal->SetMarkerSize(0.7);
        // TGraph *p0Sigma = new TGraph(128,channelNum[0],Sigma[0]);
        // p0Sigma->SetTitle("P0 Sigma");
        // p0Sigma->GetXaxis()->SetTitle("Channel Number");
        // p0Sigma->SetMarkerStyle(8);
        // p0Sigma->SetMarkerSize(0.7);
        // TGraph *p1Pedestal = new TGraph(128,channelNum[1],Pedestal[1]);
        // p1Pedestal->SetTitle("P1 Pedestal");
        // p1Pedestal->GetXaxis()->SetTitle("Channel Number");
        // p1Pedestal->SetMarkerStyle(8);
        // p1Pedestal->SetMarkerSize(0.7);
        // TGraph *p1Sigma = new TGraph(128,channelNum[1],Sigma[1]);
        // p1Sigma->SetTitle("P1 Sigma");
        // p1Sigma->GetXaxis()->SetTitle("Channel Number");
        // p1Sigma->SetMarkerStyle(8);
        // p1Sigma->SetMarkerSize(0.7);
        // TCanvas *c1 = new TCanvas("c1","c1",1);
        // c1->Divide(2,2);
        // c1->cd(1);
        // p0Pedestal->Draw("AP");
        // c1->cd(2);
        // p0Sigma->Draw("AP");
        // c1->cd(3);
        // p1Pedestal->Draw("AP");
        // c1->cd(4);
        // p1Sigma->Draw("AP"); 
}