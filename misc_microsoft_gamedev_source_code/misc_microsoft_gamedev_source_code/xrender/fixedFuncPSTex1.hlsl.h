#if 0
//
// Generated by 2.0.7776.0
//
//   fxc /T ps_2_0 /E PSFixedFunc /D UV0 fixedFuncShaders.hlsl /Fh
//    fixedFuncPSTex1.hlsl.h /Vn g_xps_Tex1PS
//
//
// Parameters:
//
//   sampler2D gSampler0;
//
//
// Registers:
//
//   Name         Reg   Size
//   ------------ ----- ----
//   gSampler0    s0       1
//

// Shader type: pixel 

xps_3_0
defconst gSampler0, sampler2d, object, [1, 1], s0
config AutoSerialize=false
config AutoResource=false
// PsExportColorCount=1
// PsSampleControl=centers

dcl_texcoord r0.xy


    exec
    tfetch2D r0, r0.xy, tf0
    alloc colors
    exece
    mov oC0, r0

// PDB hint 00000000-00000000-00000000

#endif

// This microcode is in native DWORD byte order.

const DWORD g_xps_Tex1PS[] =
{
    0x102a1100, 0x000000ac, 0x0000003c, 0x00000000, 0x00000024, 0x00000000, 
    0x00000088, 0x00000000, 0x00000000, 0x00000060, 0x0000001c, 0x00000053, 
    0xffff0300, 0x00000001, 0x0000001c, 0x00000000, 0x0000004c, 0x00000030, 
    0x00030000, 0x00010000, 0x0000003c, 0x00000000, 0x6753616d, 0x706c6572, 
    0x3000abab, 0x0004000c, 0x00010001, 0x00010000, 0x00000000, 0x70735f33, 
    0x5f300032, 0x2e302e37, 0x3737362e, 0x3000abab, 0x00000000, 0x0000003c, 
    0x10000000, 0x00000004, 0x00000000, 0x00000821, 0x00010001, 0x00000001, 
    0x00003050, 0x00011002, 0x00001200, 0xc4000000, 0x00001003, 0x00002200, 
    0x00000000, 0x10080001, 0x1f1ff688, 0x00004000, 0xc80f8000, 0x00000000, 
    0xe2000000, 0x00000000, 0x00000000, 0x00000000
};
