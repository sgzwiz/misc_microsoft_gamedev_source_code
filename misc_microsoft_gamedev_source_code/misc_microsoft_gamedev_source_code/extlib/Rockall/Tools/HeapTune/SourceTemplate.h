

const char *szFilenameSuffixCPP = ".cpp";
const char *szFilenameSuffixHPP = ".hpp";

// Parameters: Iterations, BinCount, ExpectedUtilization, HeaderFileName, Stride1, Stride2
// Parameters: %d %d %f %s %d, %d
const char *const g_szSourceTemplateCPP01 = "\
// Code generated by MGS HeapTune 1.0\n\
// Iteration:            %d\n\
// BinCount:             %d\n\
// Expected utilization: %1.4f\n\
\n\
#include \"InterfacePCH.hpp\"\n\
#include \"RockallBackEnd.hpp\"\n\
#include \"%s\"\n\
\n\
\n\
\n\
CONST SBIT32 FindCacheSize			  = 8192;\n\
CONST SBIT32 FindCacheThreshold		  = 0;\n\
CONST SBIT32 FindSize				  = 4096;\n\
CONST SBIT32 Stride1				  = %d;\n\
CONST SBIT32 Stride2				  = %d;\n\
\n\
STATIC ROCKALL_FRONT_END::CACHE_DETAILS Caches1[] =\n\
{\n\
";

// Parameters: (none)
const char *const g_szSourceTemplateCPP02 = "\
{ 0,0,0,0 }\n\
};\n\
\n\
STATIC ROCKALL_FRONT_END::CACHE_DETAILS Caches2[] =\n\
{\n\
";

// Parameters: ClassName, ClassName, ClassName, ClassName
// Parameters: %s, %s, %s, %s
const char *const g_szSourceTemplateCPP03 = "\
{	65536,	0,	65536,	65536},\n\
{ 0,0,0,0 }\n\
};\n\
\n\
STATIC int NewPageSizes[] = { 1,4,16,64,0 };\n\
\n\
\n\
%s::%s\n\
		( \n\
		int							  MaxFreeSpace,\n\
		bool						  Recycle,\n\
		bool						  SingleImage,\n\
		bool						  ThreadSafe \n\
		) :\n\
\n\
\n\
		ROCKALL_FRONT_END\n\
			(\n\
			Caches1,\n\
			Caches2,\n\
			FindCacheSize,\n\
			FindCacheThreshold,\n\
			FindSize,\n\
			MaxFreeSpace,\n\
			NewPageSizes,\n\
			(ROCKALL_BACK_END::RockallBackEnd()),\n\
			Recycle,\n\
			SingleImage,\n\
			Stride1,\n\
			Stride2,\n\
			ThreadSafe\n\
			)\n\
	{ /* void */ }\n\
\n\
%s::~%s( VOID )\n\
 	{ /* void */ }\n\
 ";

// Parameters: DefineFlag, DefineFlag, ClassName, ClassName, ClassName, ClassName, ClassName, ClassName
// Parameters: %s %s %s %s %s %s %s %s
const char *const g_szSourceTemplateHPP01 = "\
// Code generated by MGS HeapTune 1.0\n\
\n\
#ifndef %s\n\
#define %s\n\
#include \"RockallFrontEnd.hpp\"\n\
\n\
\n\
class ROCKALL_DLL_LINKAGE %s : public ROCKALL_FRONT_END\n\
    {\n\
   public:\n\
        //\n\
        //   Public functions.\n\
        //\n\
        %s\n\
			( \n\
			int						  MaxFreeSpace = (2 * HalfMegabyte),\n\
			bool					  Recycle = true,\n\
			bool					  SingleImage = false,\n\
			bool					  ThreadSafe = true \n\
			);\n\
\n\
        ~%s( void );\n\
\n\
	private:\n\
        //\n\
        //   Disabled operations.\n\
 		//\n\
		//   All copy constructors and class assignment \n\
		//   operations are disabled.\n\
        //\n\
        %s( const %s & Copy );\n\
\n\
        void operator=( const %s & Copy );\n\
    };\n\
#endif\n\
	";