#define DataLength 10
#define sets 5


void DataAnalysis()
{
    double Voltage[sets][DataLength];
    double Current[sets][DataLength];
    double times[DataLength];
    for (int i = 0; i < DataLength; i++)
    {
        times[i] = i+1;
    }
    
    double nunnum;
    char nunchar;
    string nunstr;

    TString name;
    name = "ABC+GCU.txt";
    ifstream inputfile;
    inputfile.open(name,ios::in);
    // while(inputfile.peek() != inputfile.widen('\n'))
    // {
    //     inputfile.seekg(1,inputfile.cur);
    // }
    for (int i = 0; i < sets; i++)
    {
        
        while(inputfile.good())
        {
            inputfile>>nunstr;
            if (nunstr == "start")
            {
                break;
            }  
        }
        
        for (int datanum = 0; datanum < DataLength; datanum++ )
        {
            if (!inputfile.good())
            {
                break;
            }

            while(inputfile.peek()>'9' or inputfile.peek()<'0')
            {
                inputfile.seekg(1,inputfile.cur);
            }

            inputfile.seekg(43,inputfile.cur);

            inputfile>>Voltage[i][datanum]>>Current[i][datanum];
            cout<<datanum<<"  "<<Voltage[i][datanum]<<"  "<<Current[i][datanum]<<endl; 
          
            

            inputfile.seekg(2,inputfile.cur);
            while(inputfile.peek() != inputfile.widen('\n'))
            {
                inputfile.seekg(1,inputfile.cur);
            }

            inputfile>>nunstr;
            while(inputfile.peek() != inputfile.widen('\n'))
            {
                inputfile.seekg(-1,inputfile.cur);
            }
            if (nunstr == "Finish")
            {
                break;
            }
            
        }

        // while(inputfile.good())
        // {
        //     inputfile>>nunstr;
        //     if (nunstr == "start")
        //     {
        //         break;
        //     }  
        // }
        
        // for (int datanum = 50; datanum < 2*DataLength; datanum++ )
        // {
        //     if (!inputfile.good())
        //     {
        //         break;
        //     }

        //     while(inputfile.peek()>'9' or inputfile.peek()<'0')
        //     {
        //         inputfile.seekg(1,inputfile.cur);
        //     }

        //     inputfile.seekg(43,inputfile.cur);

        //     inputfile>>Voltage[i][datanum]>>Current[i][datanum];
            
        //     inputfile.seekg(2,inputfile.cur);

        //     cout<<datanum<<"  "<<Voltage[i][datanum]<<"  "<<Current[i][datanum]<<endl; 

        //     inputfile.seekg(2,inputfile.cur);

        //     while(inputfile.peek() != inputfile.widen('\n'))
        //     {
        //         inputfile.seekg(1,inputfile.cur);
        //     }

        //     inputfile>>nunstr;
        //     while(inputfile.peek() != inputfile.widen('\n'))
        //     {
        //         inputfile.seekg(-1,inputfile.cur);
        //     }
        //     if (nunstr == "Finish")
        //     {
        //         break;
        //     }  
            
        //}
    }
   
    TGraph *set1Voltage = new TGraph(DataLength,times,Voltage[0]);
    set1Voltage->SetName("set1Voltage");
    set1Voltage->SetTitle("Voltage");
    set1Voltage->SetMarkerStyle(8);
    set1Voltage->SetMarkerSize(0.7);
    set1Voltage->SetMarkerColor(kBlue);
    set1Voltage->SetLineColor(kBlue);


    TGraph *set1Current = new TGraph(DataLength,times,Current[0]);
    set1Current->SetName("set1Current");
    set1Current->SetTitle("Current");
    set1Current->SetMarkerStyle(8);
    set1Current->SetMarkerSize(0.7);
    set1Current->SetMarkerColor(kBlue);
    set1Current->SetLineColor(kBlue);
   
    TGraph *set2Voltage = new TGraph(DataLength,times,Voltage[1]);
    set2Voltage->SetName("set2Voltage");
    set2Voltage->SetTitle("BL90N0057");
    set2Voltage->SetMarkerStyle(8);
    set2Voltage->SetMarkerSize(0.7);
    set2Voltage->SetMarkerColor(kRed);
    set2Voltage->SetLineColor(kRed);
    

    TGraph *set2Current = new TGraph(DataLength,times,Current[1]);
    set2Current->SetName("set2Current");
    set2Current->SetTitle("BL90N0057");
    set2Current->SetMarkerStyle(8);
    set2Current->SetMarkerSize(0.7);
    set2Current->SetMarkerColor(kRed);
    set2Current->SetLineColor(kRed);

    TGraph *set3Voltage = new TGraph(DataLength,times,Voltage[2]);
    set3Voltage->SetName("set3Voltage");
    set3Voltage->SetTitle("BL87N1445");
    set3Voltage->SetMarkerStyle(8);
    set3Voltage->SetMarkerSize(0.7);
    set3Voltage->SetMarkerColor(kYellow);
    set3Voltage->SetLineColor(kYellow);


    TGraph *set3Current = new TGraph(DataLength,times,Current[2]);
    set3Current->SetName("set3Current");
    set3Current->SetTitle("BL87N1445");
    set3Current->SetMarkerStyle(8);
    set3Current->SetMarkerSize(0.7);
    set3Current->SetMarkerColor(kYellow);
    set3Current->SetLineColor(kYellow);

    TGraph *set4Voltage = new TGraph(DataLength,times,Voltage[3]);
    set4Voltage->SetName("set4Voltage");
    set4Voltage->SetTitle("BL90N0050");
    set4Voltage->SetMarkerStyle(8);
    set4Voltage->SetMarkerSize(0.7);
    set4Voltage->SetMarkerColor(kGreen);
    set4Voltage->SetLineColor(kGreen);

    TGraph *set4Current = new TGraph(DataLength,times,Current[3]);
    set4Current->SetName("set4Current");
    set4Current->SetTitle("BL90N0050");
    set4Current->SetMarkerStyle(8);
    set4Current->SetMarkerSize(0.7);
    set4Current->SetMarkerColor(kGreen);
    set4Current->SetLineColor(kGreen);

    TGraph *set5Voltage = new TGraph(DataLength,times,Voltage[4]);
    set5Voltage->SetName("set5Voltage");
    set5Voltage->SetTitle("BL90N0050");
    set5Voltage->SetMarkerStyle(8);
    set5Voltage->SetMarkerSize(0.7);
    set5Voltage->SetMarkerColor(6);
    set5Voltage->SetLineColor(6);

    TGraph *set5Current = new TGraph(DataLength,times,Current[4]);
    set5Current->SetName("set5Current");
    set5Current->SetTitle("BL90N0050");
    set5Current->SetMarkerStyle(8);
    set5Current->SetMarkerSize(0.7);
    set5Current->SetMarkerColor(6);
    set5Current->SetLineColor(6);
    
    TCanvas *c1 = new TCanvas("c1","Voltage",1);
    c1->cd(1);
    set1Voltage->Draw("APL");
    set2Voltage->Draw("SAME PL");
    set3Voltage->Draw("SAME PL");
    set4Voltage->Draw("SAME PL");
    set5Voltage->Draw("SAME PL");
    TLegend *legenda =new TLegend(0.6,0.65,0.88,0.85);
    legenda->SetTextFont(72);
    legenda->SetTextSize(0.04);
    legenda->AddEntry(set1Voltage,"ABC0138","lpe");
    legenda->AddEntry(set2Voltage,"ABC0176","lpe");
    legenda->AddEntry(set3Voltage,"ABC0107","lpe");
    legenda->AddEntry(set4Voltage,"ABC0107 after taking PED","lpe");
    legenda->AddEntry(set5Voltage,"ABC0107 no grounding","lpe");
    legenda->Draw();
    c1->cd();

    TCanvas *c2 = new TCanvas("c2","Current",1);
    c2->cd(1);
    set1Current->Draw("APL");
    set2Current->Draw("SAME PL");
    set3Current->Draw("SAME PL");
    set4Current->Draw("SAME PL");
    set5Current->Draw("SAME PL");
    TLegend *legendb =new TLegend(0.6,0.65,0.88,0.85);
    legendb->SetTextFont(72);
    legendb->SetTextSize(0.04);
    legendb->AddEntry(set1Current,"ABC0138","lpe");
    legendb->AddEntry(set2Current,"ABC0176","lpe");
    legendb->AddEntry(set3Current,"ABC0107","lpe");
    legendb->AddEntry(set4Current,"ABC0107 after taking PED","lpe");
    legendb->AddEntry(set5Current,"ABC0107 no grounding","lpe");
    legendb->Draw();
    c2->cd();
    
}
