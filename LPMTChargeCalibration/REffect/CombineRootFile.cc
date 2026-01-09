void CombineRootFile(const double Position_ID)
{

    TChain chain("charge");

    // 添加20个root文件到TChain中
    for (int i = 0; i < 200; i++) {
        TString fileName = TString::Format("/junofs/users/chuziliang125/LPMTChargeCalibration/REffect/ChargeSpectrum/%.1f/ChargeSpectrum%1d.root",Position_ID,i);
        chain.Add(fileName);
    }

    // 创建新的root文件
    TString outputname = TString::Format("/junofs/users/chuziliang125/LPMTChargeCalibration/REffect/ChargeSpectrum/ChargeSpectrum_%.1f.root",Position_ID);
    TFile outputFile((TString)outputname, "RECREATE");

    // 将TChain中的tree合并到新文件中
    TTree* mergedTree = chain.CloneTree(-1, "fast");

    // 保存并关闭新文件
    mergedTree->Write();
    outputFile.Close();

    return 0;
}