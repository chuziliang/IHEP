void CombineRootFile()
{

    TChain chain("Data");

    // 添加20个root文件到TChain中
    for (int i = 0; i < 17612; i++) {
        std::string fileName = "/junofs/users/chuziliang125/LPMTChargeCalibration/ToyWaveformReconstruction/Alter_10Percent_100PE/npeLPMTtonpeSPMT_AllLPMT_Root/npeLPMTtonpeSPMT_" + std::to_string(i) + ".root";
        chain.Add(fileName.c_str());
    }

    // 创建新的root文件
    string outputname = "/junofs/users/chuziliang125/LPMTChargeCalibration/ToyWaveformReconstruction/Alter_10Percent_100PE/npeLPMTtonpeSPMT_AllLPMT.root";
    TFile outputFile((TString)outputname, "RECREATE");

    // 将TChain中的tree合并到新文件中
    TTree* mergedTree = chain.CloneTree(-1, "fast");

    // 保存并关闭新文件
    mergedTree->Write();
    outputFile.Close();

    return 0;
}