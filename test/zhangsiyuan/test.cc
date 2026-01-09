#include "Identifier.h"
#include "JunoDetectorID.h"
#include "CdID.h"

void test()
{
	int InputTrueEnergy=700;
        TChain *calibevt_spmt = new TChain("/Event/CdSpmtCalib/CdSpmtCalibEvt");
	for(int i = 0; i < 100; i++){
		TFile *f=new TFile(Form("/junofs/users/junospmt/GXU/Data/Masstest/Input_file/Other_Data/hep/calib/highE/SimOut/uniform/%dMeV/calib/calib_%dMeV_%d.root",InputTrueEnergy,InputTrueEnergy,i));
                if (f->IsZombie()) {delete f;continue;}
                delete f;
                calibevt_spmt->Add(Form("/junofs/users/junospmt/GXU/Data/Masstest/Input_file/Other_Data/hep/calib/highE/SimOut/uniform/%dMeV/calib/calib_%dMeV_%d.root",InputTrueEnergy,InputTrueEnergy,i));
	}
	JM::CdSpmtCalibEvt *calibobj_spmt=0;
        calibevt_spmt->SetBranchAddress("CdSpmtCalibEvt",&calibobj_spmt);
        Int_t TotalNumEvent = calibevt_spmt->GetEntries();
        cout << "TotalNumEvent = " << TotalNumEvent << endl;
		
	for(int i=0;i<TotalNumEvent;i++){
                if(i%100==0) cout << "event:"<<i<<endl;
                calibevt_spmt->GetEntry(i);
		const std::list<JM::CalibPmtChannel*>& pmtchannel_spmt=calibobj_spmt->calibPMTCol();
                std::list<JM::CalibPmtChannel*>::const_iterator schit=pmtchannel_spmt.begin();
                double charge[25600];
                for (int j = 0; j < 25600;j++){
                        charge[j]=0;
                }
                while(schit!=pmtchannel_spmt.end())
                {
                        const JM::CalibPmtChannel  *calib_spmt = *schit++;
                        unsigned int pmtID=calib_spmt->pmtId();
                        Identifier id = Identifier(pmtID);
                        if(CdID::is3inch(id))
                        {
                                double ch = calib_spmt->nPE();
                                unsigned int pmtid = CdID::module(id);
                                charge[pmtid-17612]=ch;
                        	cout<<ch<<endl;
			}
                }
	}
}
