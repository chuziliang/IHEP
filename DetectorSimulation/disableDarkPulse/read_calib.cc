#include "Event/CalibHeader.h"
#include "Identifier/Identifier.h"
#include "Identifier/CdID.h"

void read_calib()
{

    std::vector<double> sTime;
    std::vector<double> lTime;

    string filename = "sample_calib.root";
    TFile* ff = new TFile(filename.c_str(), "read");
    TTree* tt = (TTree*)ff->Get("/Event/Calib/CalibEvent");

    JM::CalibEvent* ce = new JM::CalibEvent();
    tt->SetBranchAddress("CalibEvent", &ce);

	TH1F *totalCharge = new TH1F("Reconstructed total charge","Reconstructed total charge",60,10,70);
    for (int i=0; i<tt->GetEntries(); i++) {
        tt->GetEntry(i);
    
		Double_t tCharge = 0;
        const std::list<JM::CalibPMTChannel*>& ecpc = ce->calibPMTCol();
        std::list<JM::CalibPMTChannel*>* m_ecpc;
        m_ecpc = const_cast<std::list<JM::CalibPMTChannel*>*>(&ecpc);

        for(std::list<JM::CalibPMTChannel*>::iterator itr=m_ecpc->begin(); 
            itr!=m_ecpc->end();
            ++itr) {
            Identifier id = Identifier((*itr)->pmtId());        
            int TruePM = CdID::module(id);
            if (TruePM>=17612) {
				tCharge += (*itr)->sumCharge();
               // for(unsigned int j=0; j<(*itr)->size(); j++) {
                //    sTime.push_back((*itr)->time(j));
				//	tCharge += (*itr)->charge(j);
                //    cout << "spmt " <<(*itr)->charge(j) <<"    size = "<<(*itr)->size()<<"   pmtID = "<< TruePM << endl; 
				//}
            }

          //  if (TruePM<17612) {
			//     for(unsigned int j=0; j<(*itr)->size(); j++) {
              //      sTime.push_back((*itr)->time(j));
                //    cout << "lpmt " <<(*itr)->time(j) << endl; 
               // }
           // }
        }
		totalCharge->Fill(tCharge);
    }
	TCanvas *c3 = new TCanvas("c3","c3");
	totalCharge->GetXaxis()->SetTitle("Number of charge");
	totalCharge->Draw();
	TFile fff("histos.root","UPDATE");
	totalCharge->Write();
}
