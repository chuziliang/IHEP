void CombineRootFile_NoBar()
{

    TChain chain("charge");

    // 添加20个root文件到TChain中
    for (int i = 0; i < 20; i++) {
        std::string fileName = "/junofs/users/chuziliang125/LPMTChargeCalibration/REffect/OldOpticalModel/ChargeSpectrum_DetCenter/ChargeSpectrum" + std::to_string(i) + ".root";
        chain.Add(fileName.c_str());
    }

    // 创建新的root文件
    string outputname = "/junofs/users/chuziliang125/LPMTChargeCalibration/REffect/OldOpticalModel/ChargeSpectrum_DetCenter/ChargeSpectrum.root";
    TFile outputFile((TString)outputname, "RECREATE");

    // 将TChain中的tree合并到新文件中
    TTree* mergedTree = chain.CloneTree(-1, "fast");

    // 保存并关闭新文件
    mergedTree->Write();
    outputFile.Close();

    return 0;
}