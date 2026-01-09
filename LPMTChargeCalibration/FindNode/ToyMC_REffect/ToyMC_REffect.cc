void ToyMC_REffect()
{
    double r_SPMT = 36;
    double r_LPMT = 254;

    vector<double> ToGraph_Factor;
    vector<double> ToGraph_R;
    for (int R_num = 0; R_num<40; R_num++)
    {
        double R = R_num*1000;
        double Omega_SPMT = (sqrt(r_SPMT*r_SPMT + R*R)*sqrt(r_LPMT*r_LPMT + R*R)-R*sqrt(r_LPMT*r_LPMT + R*R));
        double Omega_LPMT = (sqrt(r_SPMT*r_SPMT + R*R)*sqrt(r_LPMT*r_LPMT + R*R)-R*sqrt(r_SPMT*r_SPMT + R*R));
        double Factor = Omega_LPMT/Omega_SPMT;

        ToGraph_Factor.push_back(Factor);
        ToGraph_R.push_back(R);
    }

    TGraph *output = new TGraph(ToGraph_R.size(),&ToGraph_R[0],&ToGraph_Factor[0]);
    output->Draw();

}