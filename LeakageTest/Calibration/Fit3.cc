double Fitfunction(double *x, double *par)
{
    // return par[0]*x[0] + par[1];
    return par[0]*TMath::Exp(x[0]*par[1]);
}

void Fit3()
{
    double time[] = {0,112,231,420,502,996,1138};
    double time_error[] = {1,1,1,1,1,1,1};
    double SF6[] = {7.88,7.05625,5.8825,5.86875,5.79625,3.67625,2.2175};
    double SF6_error[] = {0.045034709,0.096363569,0.0781225,0.017984368,0.035509682,0.139188496,0.053062463};

    for (int i = 0; i < 7; i++)
    {
        time[i] = time[i]*60.;
        time_error[i] = time_error[i] * 60.;
    }

    TGraphErrors *gr = new TGraphErrors(7,time,SF6,time_error,SF6_error);
    TF1 *fitFcn = new TF1("fitFcn",Fitfunction,0,1200*60,2);
    fitFcn->SetParameter(0,SF6[0]);
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