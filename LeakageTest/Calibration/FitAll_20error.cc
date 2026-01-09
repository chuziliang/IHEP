
template<typename T, size_t N>
size_t arrayLength(T (&arr)[N]) {
    return N;
}

double Fitfunction(double *x, double *par)
{
    // return par[0]*x[0] + par[1];
    return par[0]*TMath::Exp(x[0]*par[1]);
}

double Fitfunction_2(double *x, double *par)
{
    // return par[0]*x[0] + par[1];
    return TMath::Exp(-x[0]*par[0]);
}

double Normalize(double *time, double *time_error, double *SF6, double *SF6_error, int length)
{
    TGraphErrors *gr = new TGraphErrors(length,time,SF6,time_error,SF6_error);
    TF1 *fitFcn = new TF1("fitFcn",Fitfunction,0,time[length-1],2);
    fitFcn->SetParameter(0,SF6[0]);
    fitFcn->SetParameter(1,TMath::Log(SF6[1]/SF6[0])/time[1]);
    gr->Fit("fitFcn");
    double C0 = fitFcn->GetParameter(0);
    double C0_Error = fitFcn->GetParError(0);
    for (int i = 0; i < length; i++)
    {
        SF6_error[i] = (SF6[i]/C0)*sqrt(pow(SF6_error[i]/SF6[i],2)+pow(C0_Error/C0,2));
        SF6[i] = SF6[i]/C0;
    }
    double Par = fitFcn->GetParameter(1);
    return Par;
}

void FitAll_20error()
{
    double time_1[] = {0,208,285,399,527,660,797};
    double time_error_1[] = {1,1,1,1,1,1,1};
    double SF6_1[] = {6.445,4.1625,2.925,2.075,1.1775,0.76375,0.54625};
    double SF6_error_1[] = {0.05258921,0.192816299,0.189761363,0.078302299,0.062873882,0.041926275,0.019485572};

    double time_2[] = {0,93,206,658,758};
    double time_error_2[] = {1,1,1,1,1};
    double SF6_2[] = {6.06625,5.87625,5.005,1.505,1.08375};
    double SF6_error_2[] = {0.111418412,0.133738317,0.179243201,0.079902284,0.074319832};

    double time_3[] = {0,112,231,420,502,996,1138};
    double time_error_3[] = {1,1,1,1,1,1,1};
    double SF6_3[] = {7.88,7.05625,5.8825,5.86875,5.79625,3.67625,2.2175};
    double SF6_error_3[] = {0.045034709,0.096363569,0.0781225,0.017984368,0.035509682,0.139188496,0.053062463};

    double time_4[] = {0,893,1276};
    double time_error_4[] = {1,1,1};
    double SF6_4[] = {100,50,28};
    double SF6_error_4[] = {33,21.6,7};

    double time_5[] = {0,205,336};
    double time_error_5[] = {1,1,1};
    double SF6_5[] = {6.7,6.6,5.9};
    double SF6_error_5[] = {0.6,0.63,0.41};

    for (int i = 0; i < sizeof(time_1)/sizeof(time_1[0]); i++)
    {
        time_1[i] = time_1[i]*60.;
        time_error_1[i] = time_error_1[i] * 60.;
        SF6_error_1[i] = SF6_1[i]*0.1;
    }

    for (int i = 0; i < sizeof(time_2)/sizeof(time_2[0]); i++)
    {
        time_2[i] = time_2[i]*60.;
        time_error_2[i] = time_error_2[i] * 60.;
        SF6_error_2[i] = SF6_2[i]*0.1;
    }

    for (int i = 0; i < sizeof(time_3)/sizeof(time_3[0]); i++)
    {
        time_3[i] = time_3[i]*60.;
        time_error_3[i] = time_error_3[i] * 60.;
        SF6_error_3[i] = SF6_3[i]*0.1;
    }

    for (int i = 0; i < sizeof(time_4)/sizeof(time_4[0]); i++)
    {
        time_4[i] = time_4[i]*60.;
        time_error_4[i] = time_error_4[i] * 60.;
        SF6_error_4[i] = SF6_4[i]*0.1;
    }

    for (int i = 0; i < sizeof(time_5)/sizeof(time_5[0]); i++)
    {
        time_5[i] = time_5[i]*60.;
        time_error_5[i] = time_error_5[i] * 60.;
        SF6_error_5[i] = SF6_5[i]*0.1;
    }

    double par_1,par_2,par_3,par_4,par_5;

    par_1 = Normalize(time_1,time_error_1,SF6_1,SF6_error_1,sizeof(time_1)/sizeof(time_1[0]));
    par_2 = Normalize(time_2,time_error_2,SF6_2,SF6_error_2,sizeof(time_2)/sizeof(time_2[0]));
    par_3 = Normalize(time_3,time_error_3,SF6_3,SF6_error_3,sizeof(time_3)/sizeof(time_3[0]));
    par_4 = Normalize(time_4,time_error_4,SF6_4,SF6_error_4,sizeof(time_4)/sizeof(time_4[0]));
    par_5 = Normalize(time_5,time_error_5,SF6_5,SF6_error_5,sizeof(time_5)/sizeof(time_5[0]));

    TGraphErrors *gr1 = new TGraphErrors(sizeof(time_1)/sizeof(time_1[0]),time_1,SF6_1,time_error_1,SF6_error_1);
    TGraphErrors *gr2 = new TGraphErrors(sizeof(time_2)/sizeof(time_2[0]),time_2,SF6_2,time_error_2,SF6_error_2);
    TGraphErrors *gr3 = new TGraphErrors(sizeof(time_3)/sizeof(time_3[0]),time_3,SF6_3,time_error_3,SF6_error_3);
    TGraphErrors *gr4 = new TGraphErrors(sizeof(time_4)/sizeof(time_4[0]),time_4,SF6_4,time_error_4,SF6_error_4);
    TGraphErrors *gr5 = new TGraphErrors(sizeof(time_5)/sizeof(time_5[0]),time_5,SF6_5,time_error_5,SF6_error_5);

    TF1 *fitFcn1 = new TF1("fitFcn1",Fitfunction_2,0,time_1[sizeof(time_1)/sizeof(time_1[0])-1],1);//time_1[sizeof(time_1)/sizeof(time_1[0])-1]
    TF1 *fitFcn2 = new TF1("fitFcn2",Fitfunction_2,0,time_2[sizeof(time_2)/sizeof(time_2[0])-1],1);//time_2[sizeof(time_2)/sizeof(time_2[0])-1]
    TF1 *fitFcn3 = new TF1("fitFcn3",Fitfunction_2,0,time_3[sizeof(time_3)/sizeof(time_3[0])-1],1);//time_3[sizeof(time_3)/sizeof(time_3[0])-1]
    TF1 *fitFcn4 = new TF1("fitFcn4",Fitfunction_2,0,time_4[sizeof(time_4)/sizeof(time_4[0])-1],1);//time_4[sizeof(time_4)/sizeof(time_4[0])-1]
    TF1 *fitFcn5 = new TF1("fitFcn5",Fitfunction_2,0,time_5[sizeof(time_5)/sizeof(time_5[0])-1],1);//time_5[sizeof(time_5)/sizeof(time_5[0])-1]

    // fitFcn->SetLineColor(kRed);
    fitFcn1->SetParNames("L/V");
    fitFcn2->SetParNames("L/V");
    fitFcn3->SetParNames("L/V");
    fitFcn4->SetParNames("L/V");
    fitFcn5->SetParNames("L/V");

    gr1->SetTitle("");
    gr1->GetXaxis()->SetTitle("Time [s]");
    gr1->GetYaxis()->SetTitle("C(t)/C(0)");
    gr1->GetXaxis()->CenterTitle(true);
    gr1->GetYaxis()->CenterTitle(true);
    gr1->GetXaxis()->SetTitleSize(0.05);  // 新增
    gr1->GetYaxis()->SetTitleSize(0.05);  // 新增
    // 调整标题偏移量保持显示效果
    gr1->GetXaxis()->SetTitleOffset(0.9); // 新增
    gr1->GetYaxis()->SetTitleOffset(0.9); // 新增
    gr1->GetXaxis()->SetLimits(-1000,90000);

    fitFcn1->SetParameter(0,-par_1);
    gr1->Fit("fitFcn1","0");
    fitFcn2->SetParameter(0,-par_2);
    gr2->Fit("fitFcn2","0");
    fitFcn3->SetParameter(0,-par_3);
    gr3->Fit("fitFcn3","0");
    fitFcn4->SetParameter(0,-par_4);
    gr4->Fit("fitFcn4","0");
    fitFcn5->SetParameter(0,-par_5);
    gr5->Fit("fitFcn5","0");

    gr1->SetMarkerStyle(20);
    gr2->SetMarkerStyle(20);
    gr3->SetMarkerStyle(20);
    gr4->SetMarkerStyle(20);
    gr5->SetMarkerStyle(20);

    auto col1 = TColor::GetColor(87, 144, 252); // R,G,B
    auto col2 = TColor::GetColor(248, 156, 32); // R,G,B
    auto col3 = TColor::GetColor(228, 37, 54); // R,G,B
    auto col4 = TColor::GetColor(150, 74, 139); // R,G,B
    auto col5 = TColor::GetColor(156, 156, 161); // R,G,B


    gr1->SetMarkerColor(col1);
    gr2->SetMarkerColor(col2);
    gr3->SetMarkerColor(col3);
    gr4->SetMarkerColor(col4);
    gr5->SetMarkerColor(col5);

    gr1->SetLineColor(col1);
    gr2->SetLineColor(col2);
    gr3->SetLineColor(col3);
    gr4->SetLineColor(col4);
    gr5->SetLineColor(col5);

    fitFcn1->SetLineColor(col1);
    fitFcn2->SetLineColor(col2);
    fitFcn3->SetLineColor(col3);
    fitFcn4->SetLineColor(col4);
    fitFcn5->SetLineColor(col5);

    TCanvas* C1 = new TCanvas();
    C1->SetRightMargin(0.2);

    // 设置x坐标标题位于中央  
    gr1->Draw("AP");
    gPad->SetLogy();
    gStyle->SetOptFit(1111);
    fitFcn1->Draw("same");
    C1->Update();
    TPaveStats* stats1 = (TPaveStats*)C1->GetPrimitive("stats");
    stats1->SetX1NDC(0.7);
    stats1->SetX2NDC(0.99);
    stats1->SetY1NDC(0.135);
    stats1->SetY2NDC(0.295);
    stats1->SetName("stats1");
    stats1->SetTextColor(col1);
	C1->Update();

    
    
    C1->Update();
    gr2->Draw("P");
    fitFcn2->Draw("same");
    C1->Update();
    TPaveStats* stats2 = (TPaveStats*)C1->GetPrimitive("stats");
    stats2->SetName("stats2");
    stats2->SetX1NDC(0.7);
    stats2->SetX2NDC(0.99);
	stats2->SetY1NDC(0.295);    //set new y start position 
	stats2->SetY2NDC(0.455);    //set new y end position 
    stats2->SetTextColor(col2);
	C1->Update();

    gr3->Draw("P");
    fitFcn3->Draw("same");
    C1->Update();
    TPaveStats* stats3 = (TPaveStats*)C1->GetPrimitive("stats");
    stats3->SetName("stats3");
    stats3->SetX1NDC(0.7);
    stats3->SetX2NDC(0.99);
	stats3->SetY1NDC(0.615);    //set new y start position
	stats3->SetY2NDC(0.775);    //set new y end position
    stats3->SetTextColor(col3);
	C1->Update();

    gr4->Draw("P");
    fitFcn4->Draw("same");
    C1->Update();
    TPaveStats* stats4 = (TPaveStats*)C1->GetPrimitive("stats");
    stats4->SetName("stats4");
    stats4->SetX1NDC(0.7);
    stats4->SetX2NDC(0.99);
	stats4->SetY1NDC(0.455);    //set new y start position
	stats4->SetY2NDC(0.615);    //set new y end position
    stats4->SetTextColor(col4);
	C1->Update();

    gr5->Draw("P");
    fitFcn5->Draw("same");
    C1->Update();
    TPaveStats* stats5 = (TPaveStats*)C1->GetPrimitive("stats");
    stats5->SetName("stats5");
    stats5->SetX1NDC(0.7);
    stats5->SetX2NDC(0.99);
	// stats5->SetY1NDC(0.135);    //set new y start position
	// stats5->SetY2NDC(0.295);    //set new y end position
    stats5->SetTextColor(col5);
	C1->Update();

    gr1->Draw("P");

    // 将C1保存为PDF文件
    C1->SaveAs("FitAll_20error.pdf");

    // cout<<"L="<<fitFcn->GetParameter(1)*0.011375<<"  "<<"Error="<<fitFcn->GetParError(1)*0.011375<<endl;
}