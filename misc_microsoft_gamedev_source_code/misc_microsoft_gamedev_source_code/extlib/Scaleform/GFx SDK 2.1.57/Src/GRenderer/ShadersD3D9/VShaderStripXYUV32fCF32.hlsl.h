#if 0
//
// Generated by 2.0.7776.0
//
//   fxc /T vs_1_1 /E VShaderStripXYUV32fCF32 scaleform.hlsl /Fh
//    VShaderStripXYUV32fCF32.hlsl.h
//
// Shader type: vertex 

xvs_3_0
config AutoSerialize=false
config AutoResource=false
config VsMaxReg=3
config VsResource=1
// VsExportCount=2

dcl_index r0.x
dcl_texcoord o0.xy
dcl_color o1
dcl_color1 o2


    exec
    vfetch r3, r0.x, position
    vfetch r2.xy__, r0.x, texcoord
    vfetch r1, r0.x, color
    vfetch r0, r0.x, color1
    alloc position
    exec
    mov oPos, r3
    alloc interpolators
    exece
    mov o0.xy, r2.xy
    mov o1, r1
    mov o2, r0

// PDB hint 00000000-00000000-00000000

#endif

// This microcode is in native DWORD byte order.

const DWORD g_xvs_VShaderStripXYUV32fCF32[] =
{
    0x102a1101, 0x000000a8, 0x00000090, 0x00000000, 0x00000024, 0x00000000, 
    0x00000058, 0x00000000, 0x00000000, 0x00000030, 0x0000001c, 0x00000023, 
    0xfffe0300, 0x00000000, 0x00000000, 0x00000000, 0x0000001c, 0x76735f33, 
    0x5f300032, 0x2e302e37, 0x3737362e, 0x3000abab, 0x00000000, 0x00000090, 
    0x00210003, 0x00000000, 0x00000000, 0x00002863, 0x00000001, 0x00000004, 
    0x00000003, 0x00000290, 0x00100003, 0x00005004, 0x0000a005, 0x0031a006, 
    0x00003050, 0x0001f1a0, 0x0002f2a1, 0x00001008, 0x00001009, 0x0000100a, 
    0xf0554003, 0x00001200, 0xc2000000, 0x00001007, 0x00001200, 0xc4000000, 
    0x00003008, 0x00002200, 0x00000000, 0x05f83000, 0x00000688, 0x00000000, 
    0x05f82000, 0x00000fc8, 0x00000000, 0x05f81000, 0x00000688, 0x00000000, 
    0x05f80000, 0x00000688, 0x00000000, 0xc80f803e, 0x00000000, 0xe2030300, 
    0xc8038000, 0x00b0b000, 0xe2020200, 0xc80f8001, 0x00000000, 0xe2010100, 
    0xc80f8002, 0x00000000, 0xe2000000, 0x00000000, 0x00000000, 0x00000000
};
