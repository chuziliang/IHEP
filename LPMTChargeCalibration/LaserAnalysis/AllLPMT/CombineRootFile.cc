const int energy[] ={1000,100,10,1500,150,1,200,20,2,300,30,3,4,500,50,5,6,700,7,8,9};
const int numbers[]={100,50,50,200,50,50,50,10,50,100,20,50,50,100,50,50,50,100,50,50,50};

void CombineRootFile(const int energynum)
{
    TChain chain("charge");

    // 添加20个root文件到TChain中
    for (int i = 0; i < numbers[energynum]; i++) {
        string filename = "/junofs/users/chuziliang125/LPMTChargeCalibration/LaserAnalysis/AllLPMT/ChargeSpectrum/"+to_string(energy[energynum])+"_MeV/Charge_"+to_string(i)+".root";
        chain.Add(filename.c_str());
    }

    // 创建新的root文件
    string outputname = "/junofs/users/chuziliang125/LPMTChargeCalibration/LaserAnalysis/AllLPMT/CombineChargeSpectrum/CombineCharge_" + to_string(energy[energynum]) +"MeV.root";
    TFile outputFile((TString)outputname, "RECREATE");

    // 将TChain中的tree合并到新文件中
    TTree* mergedTree = chain.CloneTree(-1, "fast");

    // 保存并关闭新文件
    mergedTree->Write();
    outputFile.Close();

}