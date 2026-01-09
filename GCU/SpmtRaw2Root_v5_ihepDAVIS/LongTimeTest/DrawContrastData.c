void DrawContrastData()
{
    ifstream FitDataResult;//定义一个输入流
    FitDataResult.open("ContrastData.txt",ios::in);//打开一个txt文件，ate是把光标在文件末尾

    Double_t channelNum[6][128];
    Double_t Gain[6][128];
    Double_t Resolution[6][128];
    Double_t DarkRate[6][128];
    //string nun;name;
    // for (int i = 0; i < 2; i++)
    // {
    //     FitDataResult.seekg(-2,FitDataResult.cur);//从当前光标位置前移2个数据，为了避免文件末尾空格或者回车的影响
    //     while(FitDataResult.peek() != FitDataResult.widen('\n'))
    //     {
    //         FitDataResult.seekg(-1,FitDataResult.cur);
    //     }//前移光标，直到回车处停止，也就是到最后一行的开头处。
    // }
    
   // while (!FitDataResult.eof())
    {

       // FitDataResult>>nun>>nun>>name;
        for (int j = 0; j<6; j++)
        {
            for (int i = 0; i<128; i++)
            {
                FitDataResult>>Gain[j][i]>>Resolution[j][i]>>DarkRate[j][i];//读取数据，我的数据结构是“gain1 resolution1 DarkRate1 gain2 resolution2 DarkRate2 ...”
                channelNum[j][i] = i+1;
            }
        }
        
        //这后面可以不用看了，都是画图啥的

		double AverageGain[3][8] = {{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0}};
		for (int j = 0; j<3; j++)
        {
            for(int i = 0; i<128;i++)
            {
                AverageGain[j][i/16] += Gain[2*j][i];
            }
        }

		for (int j = 0; j<3; j++)
        {
            for(int i = 0; i<8; i++)
            {
                AverageGain[j][i]=AverageGain[j][i]/16;
                cout<<"PMT group "<<i<<"    AverageGain = "<<AverageGain[j][i]<<endl;
            }
            cout<<"-------------------ABC "<<j<<"---------------------"<<endl;
        }
		
        TGraph *p0Gain = new TGraph(128,channelNum[0],Gain[0]);
        p0Gain->SetTitle("P0 Gain");
        p0Gain->GetXaxis()->SetTitle("Channel Number");
        p0Gain->SetMarkerStyle(8);
        p0Gain->SetMarkerSize(0.7);
        p0Gain->GetYaxis()->SetRangeUser(2,6);
        TGraph *p0Resolution = new TGraph(128,channelNum[0],Resolution[0]);
        p0Resolution->SetTitle("P0 Resolution");
        p0Resolution->GetXaxis()->SetTitle("Channel Number");
        p0Resolution->SetMarkerStyle(8);
        p0Resolution->SetMarkerSize(0.7);
        p0Resolution->GetYaxis()->SetRangeUser(15,55);
        TGraph *p0DarkRate = new TGraph(128,channelNum[0],DarkRate[0]);
        p0DarkRate->SetTitle("P0 Dark Rate");
        p0DarkRate->GetXaxis()->SetTitle("Channel Number");
        p0DarkRate->SetMarkerStyle(8);
        p0DarkRate->SetMarkerSize(0.7);
        TGraph *p1Gain = new TGraph(128,channelNum[1],Gain[1]);
        p1Gain->SetTitle("P1 Gain");
        p1Gain->GetXaxis()->SetTitle("Channel Number");
        p1Gain->SetMarkerStyle(8);
        p1Gain->SetMarkerSize(0.7);
        p1Gain->GetYaxis()->SetRangeUser(2,6);
        TGraph *p1Resolution = new TGraph(128,channelNum[1],Resolution[1]);
        p1Resolution->SetTitle("P1 Resolution");
        p1Resolution->GetXaxis()->SetTitle("Channel Number");
        p1Resolution->SetMarkerStyle(8);
        p1Resolution->SetMarkerSize(0.7);
        p1Resolution->GetYaxis()->SetRangeUser(15,55);
        TGraph *p1DarkRate = new TGraph(128,channelNum[1],DarkRate[1]);
        p1DarkRate->SetTitle("P1 Dark Rate");
        p1DarkRate->GetXaxis()->SetTitle("Channel Number");
        p1DarkRate->SetMarkerStyle(8);
        p1DarkRate->SetMarkerSize(0.7);

        TGraph *p2Gain = new TGraph(128,channelNum[2],Gain[2]);
        p2Gain->SetTitle("P2 Gain");
        p2Gain->GetXaxis()->SetTitle("Channel Number");
        p2Gain->SetMarkerStyle(8);
        p2Gain->SetMarkerSize(0.7);
        p2Gain->GetYaxis()->SetRangeUser(2,6);
        p2Gain->SetMarkerColor(kRed);
        TGraph *p2Resolution = new TGraph(128,channelNum[2],Resolution[2]);
        p2Resolution->SetTitle("P2 Resolution");
        p2Resolution->GetXaxis()->SetTitle("Channel Number");
        p2Resolution->SetMarkerStyle(8);
        p2Resolution->SetMarkerSize(0.7);
        p2Resolution->GetYaxis()->SetRangeUser(15,55);
        p2Resolution->SetMarkerColor(kRed);
        TGraph *p2DarkRate = new TGraph(128,channelNum[2],DarkRate[2]);
        p2DarkRate->SetTitle("P3 Dark Rate");
        p2DarkRate->GetXaxis()->SetTitle("Channel Number");
        p2DarkRate->SetMarkerStyle(8);
        p2DarkRate->SetMarkerSize(0.7);
        p2DarkRate->SetMarkerColor(kRed);
        TGraph *p3Gain = new TGraph(128,channelNum[3],Gain[3]);
        p3Gain->SetTitle("P3 Gain");
        p3Gain->GetXaxis()->SetTitle("Channel Number");
        p3Gain->SetMarkerStyle(8);
        p3Gain->SetMarkerSize(0.7);
        p3Gain->GetYaxis()->SetRangeUser(2,6);
        p3Gain->SetMarkerColor(kRed);
        TGraph *p3Resolution = new TGraph(128,channelNum[3],Resolution[3]);
        p3Resolution->SetTitle("P3 Resolution");
        p3Resolution->GetXaxis()->SetTitle("Channel Number");
        p3Resolution->SetMarkerStyle(8);
        p3Resolution->SetMarkerSize(0.7);
        p3Resolution->GetYaxis()->SetRangeUser(15,55);
        p3Resolution->SetMarkerColor(kRed);
        TGraph *p3DarkRate = new TGraph(128,channelNum[3],DarkRate[3]);
        p3DarkRate->SetTitle("P3 Dark Rate");
        p3DarkRate->GetXaxis()->SetTitle("Channel Number");
        p3DarkRate->SetMarkerStyle(8);
        p3DarkRate->SetMarkerSize(0.7);
        p3DarkRate->SetMarkerColor(kRed);

        TGraph *p4Gain = new TGraph(128,channelNum[4],Gain[4]);
        p4Gain->SetTitle("P4 Gain");
        p4Gain->GetXaxis()->SetTitle("Channel Number");
        p4Gain->SetMarkerStyle(8);
        p4Gain->SetMarkerSize(0.7);
        p4Gain->GetYaxis()->SetRangeUser(2,6);
        p4Gain->SetMarkerColor(kBlue);
        TGraph *p4Resolution = new TGraph(128,channelNum[4],Resolution[4]);
        p4Resolution->SetTitle("P4 Resolution");
        p4Resolution->GetXaxis()->SetTitle("Channel Number");
        p4Resolution->SetMarkerStyle(8);
        p4Resolution->SetMarkerSize(0.7);
        p4Resolution->GetYaxis()->SetRangeUser(15,55);
        p4Resolution->SetMarkerColor(kBlue);
        TGraph *p4DarkRate = new TGraph(128,channelNum[4],DarkRate[4]);
        p4DarkRate->SetTitle("P4 Dark Rate");
        p4DarkRate->GetXaxis()->SetTitle("Channel Number");
        p4DarkRate->SetMarkerStyle(8);
        p4DarkRate->SetMarkerSize(0.7);
        p4DarkRate->SetMarkerColor(kBlue);
        TGraph *p5Gain = new TGraph(128,channelNum[5],Gain[5]);
        p5Gain->SetTitle("P5 Gain");
        p5Gain->GetXaxis()->SetTitle("Channel Number");
        p5Gain->SetMarkerStyle(8);
        p5Gain->SetMarkerSize(0.7);
        p5Gain->GetYaxis()->SetRangeUser(2,6);
        p5Gain->SetMarkerColor(kBlue);
        TGraph *p5Resolution = new TGraph(128,channelNum[5],Resolution[5]);
        p5Resolution->SetTitle("P5 Resolution");
        p5Resolution->GetXaxis()->SetTitle("Channel Number");
        p5Resolution->SetMarkerStyle(8);
        p5Resolution->SetMarkerSize(0.7);
        p5Resolution->GetYaxis()->SetRangeUser(15,55);
        p5Resolution->SetMarkerColor(kBlue);
        TGraph *p5DarkRate = new TGraph(128,channelNum[5],DarkRate[5]);
        p5DarkRate->SetTitle("P5 Dark Rate");
        p5DarkRate->GetXaxis()->SetTitle("Channel Number");
        p5DarkRate->SetMarkerStyle(8);
        p5DarkRate->SetMarkerSize(0.7);
        p5DarkRate->SetMarkerColor(kBlue);

        TCanvas *c1 = new TCanvas("c1","c1",1);
        c1->Divide(3,2);
        c1->cd(1);
        p0Gain->Draw("AP");
        p2Gain->Draw("SAME P");
        p4Gain->Draw("SAME P");
        c1->cd(2);
        p0Resolution->Draw("AP");
        p2Resolution->Draw("SAME P");
        p4Resolution->Draw("SAME P");
        c1->cd(3);
        p2DarkRate->Draw("AP");
        p4DarkRate->Draw("SAME P");
        c1->cd(4);
        p1Gain->Draw("AP");
        p3Gain->Draw("SAME P");
        p5Gain->Draw("SAME P");
        c1->cd(5);
        p1Resolution->Draw("AP");
        p3Resolution->Draw("SAME P");
        p5Resolution->Draw("SAME P");
        c1->cd(6);
        p3DarkRate->Draw("AP");
        p5DarkRate->Draw("SAME P");
    } 
}
