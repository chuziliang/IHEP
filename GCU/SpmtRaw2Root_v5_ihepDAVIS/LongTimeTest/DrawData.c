void DrawData()
{
    ifstream FitDataResult;//定义一个输入流
    FitDataResult.open("FitDataResult.txt",ios::ate);//打开一个txt文件，ate是把光标在文件末尾

    Double_t channelNum[2][128];
    Double_t Gain[2][128];
    Double_t Resolution[2][128];
    Double_t DarkRate[2][128];
    //string nun;name;
    FitDataResult.seekg(-2,FitDataResult.cur);//从当前光标位置前移2个数据，为了避免文件末尾空格或者回车的影响
    while(FitDataResult.peek() != FitDataResult.widen('\n'))
    {
        FitDataResult.seekg(-1,FitDataResult.cur);
    }//前移光标，直到回车处停止，也就是到最后一行的开头处。
   
   // while (!FitDataResult.eof())
    {

       // FitDataResult>>nun>>nun>>name;
        
        for (int i = 0; i<128; i++)
        {
            FitDataResult>>Gain[0][i]>>Resolution[0][i]>>DarkRate[0][i];//读取数据，我的数据结构是“gain1 resolution1 DarkRate1 gain2 resolution2 DarkRate2 ...”
            channelNum[0][i] = i+1;
        }
        for (int i = 0; i < 128; i++)
        {
            FitDataResult>>Gain[1][i]>>Resolution[1][i]>>DarkRate[1][i];
            channelNum[1][i] = i+1;
        }

        //这后面可以不用看了，都是画图啥的

		double AverageGain[8] = {0,0,0,0,0,0,0,0};
		for(int i = 0; i<128;i++)
		{
			AverageGain[i/16]+= Gain[1][i];
		}
		
		for(int i = 0; i<8; i++)
		{
			AverageGain[i]=AverageGain[i]/16;
			cout<<"PMT group "<<i<<"    AverageGain = "<<AverageGain[i]<<endl;
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
        TCanvas *c1 = new TCanvas("c1","c1",1);
        c1->Divide(3,2);
        c1->cd(1);
        p0Gain->Draw("AP");
        c1->cd(2);
        p0Resolution->Draw("AP");
        c1->cd(3);
        p0DarkRate->Draw("AP");
        c1->cd(4);
        p1Gain->Draw("AP");
        c1->cd(5);
        p1Resolution->Draw("AP");
        c1->cd(6);
        p1DarkRate->Draw("AP");
    } 
}
