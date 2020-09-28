//============================================================================
//
//  FatalExit.cpp
//
//  Copyright (c) 1999-2001, Ensemble Studios
//
//============================================================================


//============================================================================
//  INCLUDES
//============================================================================
#include "xcore.h"

//============================================================================
//  PRIVATE GLOBALS
//============================================================================
static bool sgFatalExit = false;


//============================================================================
//  PUBLIC FUNCTIONS
//============================================================================
bool isFatalExit()
{
   return sgFatalExit;
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
void doFatalExit(long code)
{
   sgFatalExit = true;
//   exit(code);
   //--Xenon Port
   //--We need to do something here.
#ifndef XBOX
   TerminateProcess(GetCurrentProcess(), code);
#endif
}
