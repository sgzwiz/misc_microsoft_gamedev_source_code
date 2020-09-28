#ifndef _BENCHMARK_PCH_HPP_
#define _BENCHMARK_PCH_HPP_
//                                        Ruler
//       1         2         3         4         5         6         7         8
//345678901234567890123456789012345678901234567890123456789012345678901234567890

    /********************************************************************/
    /*                                                                  */
    /*   The standard layout.                                           */
    /*                                                                  */
    /*   The standard layout for 'hpp' files for this code is as        */
    /*   follows:                                                       */
    /*                                                                  */
    /*      1. Include files.                                           */
    /*      2. Constants exported from the class.                       */
    /*      3. Data structures exported from the class.                 */
	/*      4. Forward references to other data structures.             */
	/*      5. Class specifications (including inline functions).       */
    /*      6. Additional large inline functions.                       */
    /*                                                                  */
    /*   Any portion that is not required is simply omitted.            */
    /*                                                                  */
    /********************************************************************/

#include "Global.hpp"

#ifndef DISABLE_PRECOMPILED_HEADERS
#include "AllHeaps.hpp"
#include "BlendedHeapTest.hpp"
#include "DebugHeapTest.hpp"
#include "FastHeapTest.hpp"
#include "GlobalHeapTest.hpp"
#include "LfhHeapTest.hpp"
#include "NasdaqHeapTest.hpp"
#include "NoHeapTest.hpp"
#include "NtHeapTest.hpp"
#include "SmallHeapTest.hpp"
#include "SmpHeapTest.hpp"
#include "VcHeapTest.hpp"
#endif
#endif
