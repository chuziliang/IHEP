#ifndef JUNO_DETECTOR_CD_ID_H
#define JUNO_DETECTOR_CD_ID_H

//
//  This class provide hierarchy and functions for Central Detector Identifier
//
//  Author: Zhengyun You  2013-11-20
//

#include "Identifier/JunoDetectorID.h"
#include <string>
#include <assert.h>

class CdID : public JunoDetectorID
{
    public:

        typedef Identifier::size_type  size_type; 
        typedef Identifier::value_type value_type; 

        /// constructor
        CdID();

        /// destructor 
        ~CdID();
 
        /// For a single pmt
        static Identifier id ( unsigned int module,
                               unsigned int pmt
                             );
        static Identifier id ( unsigned int value );
        static value_type getIntID ( unsigned int module,
                                     unsigned int pmt
                                   );
        //static value_type getModuleMax();
        //static value_type getModuleMin();
        //static value_type getPmtMax();
        //static value_type getPmtMin();
 
        static bool valuesOk ( const unsigned int module,  
                               const unsigned int pmt 
                             ) ;

        /// Values of different levels (failure returns 0)
        static int module (const Identifier& id);
        static int pmt    (const Identifier& id); 
        static bool is20inch(const Identifier& id);
        static bool is3inch(const Identifier& id); 
 
        /// Max/Min values for each field (error returns -999)
        static int moduleMin();
        static int moduleMax();
        static int modulePmtMin();
        static int modulePmtMax();

        static int module20inchMin();
        static int module20inchMax();
        static int module20inchNumber();
        static int module3inchMin();
        static int module3inchMax();
        static int module3inchNumber();

        /// Set Module Max (when geometry not fixed)
        static void setModuleMax(unsigned int value) { MODULE_MAX = value; }
        static void setModule20inchMin(unsigned int value) { MODULE_20INCH_MIN = value; }
        static void setModule20inchMax(unsigned int value) { MODULE_20INCH_MAX = value; }
        static void setModule3inchMin(unsigned int value) { MODULE_3INCH_MIN = value; }
        static void setModule3inchMax(unsigned int value) { MODULE_3INCH_MAX = value; }

    private:

        typedef std::vector<Identifier> idVec;
        typedef idVec::const_iterator   idVecIt;

        static const unsigned int MODULE_INDEX    = 8;
        static const unsigned int MODULE_MASK     = 0x00FFFF00;

        static const unsigned int PMT_INDEX       = 0;
        static const unsigned int PMT_MASK        = 0x000000FF;

        static const unsigned int MODULE_MIN      = 0;
        static       unsigned int MODULE_MAX;     //= 65535;  // 14518 for DetSim0, 16719 for DetSim1, 18305 DetSim1 update

        static       unsigned int MODULE_20INCH_MIN;
        static       unsigned int MODULE_20INCH_MAX;
        static       unsigned int MODULE_3INCH_MIN;
        static       unsigned int MODULE_3INCH_MAX;
        static const unsigned int MODULE_3INCH_SHIFT = 300000;

        static const unsigned int MODULE_PMT_MAX  = 0;
        static const unsigned int MODULE_PMT_MIN  = 0;  

};

#endif // JUNO_DETECTOR_CD_ID_H
