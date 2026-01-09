// #include "Geometry/RecGeomSvc.h"
// #include "Identifier/Identifier.h"
// #include "Identifier/CdID.h"
// #include "Geometry/IPMTParamSvc.h"
#include "Geometry/IPMTParamSvc.h"

#include "RecTools/IRecMuonTool.h"
#include "SniperKernel/AlgFactory.h"
#include "SniperKernel/ToolBase.h"
#include "SniperKernel/AlgBase.h"

#include "BufferMemMgr/IDataMemMgr.h"

#include "Geometry/RecGeomSvc.h"
#include "Identifier/Identifier.h"
#include "Identifier/CdID.h"
#include "TStopwatch.h"

void test()
{
    //bool RecCdMuonAlg::iniGeomSvc()
    // {
    // //Retrieve Geometry service
    // SniperPtr<RecGeomSvc> rgSvc(getParent(), "RecGeomSvc");
    // if ( rgSvc.invalid())
    // {
    //     LogError <<"Failed toget RecGeomSvc instance!"<< std::endl;
    //     return false ;
    // }
    // CdGeom m_cdGeom = rgSvc->getCdGeom();
    // return true;
    // }
    // cout<<m_cdGeom->getPmtNum<<endl;

    SniperPtr<IPMTParamSvc> pmtParamSvc(getParent(), "PMTParamSvc");
    if (pmtParamSvc.invalid())
        {
            LogError << "Can't Locate  PMTParamSvc." << std::endl;
                return false;
        }
        
    // examples
    int num_CD_LPMT = pmtParamSvc->get_NTotal_CD_LPMT();
    float pmt_x = pmtParamSvc->getPMTX(0);                   // use sim id of PMT
}