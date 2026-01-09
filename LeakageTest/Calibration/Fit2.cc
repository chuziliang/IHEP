double Fitfunction(double *x, double *par)
{
    // return par[0]*x[0] + par[1];
    return par[0]*TMath::Exp(x[0]*par[1]);
}

void Fit2()
{
    double time[] = {0,93,206,658,758};
    double time_error[] = {1,1,1,1,1};
    double SF6[] = {6.06625,5.87625,5.005,1.505,1.08375};
    double SF6_error[] = {0.111418412,0.133738317,0.179243201,0.079902284,0.074319832};

    for (int i = 0; i < 5; i++)
    {
        time[i] = time[i]*60.;
        time_error[i] = time_error[i] * 60.;
    }

    TGraphErrors *gr = new TGraphErrors(5,time,SF6,time_error,SF6_error);
    TF1 *fitFcn = new TF1("fitFcn",Fitfunction,0,900,2);
    fitFcn->SetParameter(0,6.06625);
    fitFcn->SetParameter(1,TMath::Log(SF6[1]/SF6[0])/time[1]);
    fitFcn->SetLineColor(kRed);
    fitFcn->SetParNames("C_{SF_{6}}(t_{0})","-L/V");
    gr->SetTitle("");
    gr->GetXaxis()->SetTitle("Time/s");
    gr->GetYaxis()->SetTitle("Concentration/PPM");
    gr->Fit("fitFcn");
    gr->SetMarkerStyle(20);
    gr->Draw("AP");
    gStyle->SetOptFit(1111);
    fitFcn->Draw("same");

    cout<<"L="<<fitFcn->GetParameter(1)*0.011375<<"  "<<"Error="<<fitFcn->GetParError(1)*0.011375<<endl;
}