#ifndef _VC_HEAP_TEST_HPP_
#define _VC_HEAP_TEST_HPP_
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

#include "AllHeaps.hpp"

    /********************************************************************/
    /*                                                                  */
    /*   A heap test harness                                            */
    /*                                                                  */
    /*   The heap test harness exposes a common interface for all       */
    /*   the heap that are tested.                                      */
    /*                                                                  */
    /********************************************************************/

class VC_HEAP_TEST : public ALL_HEAPS
    {
        //
        //   Private data.
        //
	    BOOLEAN						  Active;
		SBIT32						  MaxHeaps;
		SBIT32						  MaxThreads;
		BOOLEAN						  ZeroAll;

    public:
        //
        //   Public functions.
        //
        VC_HEAP_TEST( VOID );

		VIRTUAL BOOLEAN CreateHeaps
			( 
			SBIT32					  NumberOfHeaps,
			SBIT32					  NumberOfThreads,
			BOOLEAN					  ThreadSafe,
			BOOLEAN					  Zero
			);

		VIRTUAL VOID Delete
			( 
			VOID					  *Address,
			SBIT32					  Thread 
			);

		VIRTUAL CHAR *NameOfHeap( VOID );

		VIRTUAL VOID New
			( 
			VOID					  **Address,
			SBIT32					  Size, 
			SBIT32					  Thread 
			);

		VIRTUAL VOID Resize
			( 
			VOID					  **Address,
			SBIT32					  NewSize, 
			SBIT32					  Thread 
			);

		VIRTUAL VOID DeleteHeaps( VOID );

        ~VC_HEAP_TEST( VOID );

	private:
        //
        //   Disabled operations.
        //
        VC_HEAP_TEST( CONST VC_HEAP_TEST & Copy );

        VOID operator=( CONST VC_HEAP_TEST & Copy );
    };
#endif
