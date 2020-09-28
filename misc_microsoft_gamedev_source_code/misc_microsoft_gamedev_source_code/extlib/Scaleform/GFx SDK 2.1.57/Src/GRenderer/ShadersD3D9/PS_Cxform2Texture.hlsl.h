#if 0
//
// Generated by 2.0.7776.0
//
//   fxc /T ps_1_1 /E PS_Cxform2Texture scaleform.hlsl /Fh
//    PS_Cxform2Texture.hlsl.h
//
//
// Parameters:
//
//   float4 cxadd;
//   float4 cxmul;
//   sampler2D tex0;
//   sampler2D tex1;
//
//
// Registers:
//
//   Name         Reg   Size
//   ------------ ----- ----
//   cxmul        c2       1
//   cxadd        c3       1
//   tex0         s0       1
//   tex1         s1       1
//

// Shader type: pixel 

xps_3_0
defconst cxadd, float, vector, [1, 4], c3
defconst cxmul, float, vector, [1, 4], c2
defconst tex0, sampler2d, object, [1, 1], s0
defconst tex1, sampler2d, object, [1, 1], s1
config AutoSerialize=false
config AutoResource=false
config PsMaxReg=3
// PsExportColorCount=1
// PsSampleControl=both

dcl_texcoord r0.xy
dcl_texcoord1 r1.xy
dcl_color1_centroid r2


    exec
    tfetch2D r3, r0.xy, tf0
    tfetch2D r0, r1.xy, tf1
    alloc colors
    exece
    add r1, r3, -r0
    mad r2, r1, r2.z, r0
    mad oC0, r2, c2, c3

// PDB hint 00000000-00000000-00000000

#endif

// This microcode is in native DWORD byte order.

const DWORD g_xps_PS_Cxform2Texture[] =
{
    0x102a1100, 0x0000010c, 0x00000060, 0x00000000, 0x00000024, 0x00000000, 
    0x000000e0, 0x00000000, 0x00000000, 0x000000b8, 0x0000001c, 0x000000ac, 
    0xffff0300, 0x00000004, 0x0000001c, 0x00000000, 0x000000a5, 0x0000006c, 
    0x00020003, 0x00010000, 0x00000074, 0x00000000, 0x00000084, 0x00020002, 
    0x00010000, 0x00000074, 0x00000000, 0x0000008a, 0x00030000, 0x00010000, 
    0x00000090, 0x00000000, 0x000000a0, 0x00030001, 0x00010000, 0x00000090, 
    0x00000000, 0x63786164, 0x6400abab, 0x00010003, 0x00010004, 0x00010000, 
    0x00000000, 0x63786d75, 0x6c007465, 0x783000ab, 0x0004000c, 0x00010001, 
    0x00010000, 0x00000000, 0x74657831, 0x0070735f, 0x335f3000, 0x322e302e, 
    0x37373736, 0x2e3000ab, 0x00000000, 0x00000060, 0x10000300, 0x00000008, 
    0x00000000, 0x00002063, 0x00030007, 0x00000001, 0x00003050, 0x00003151, 
    0x0000f2a1, 0x00052002, 0x00001200, 0xc4000000, 0x00003004, 0x00002200, 
    0x00000000, 0x10083001, 0x1f1ff688, 0x00004000, 0x10180021, 0x1f1ff688, 
    0x00004000, 0xc80f0201, 0x02000000, 0xe0030000, 0xc80f0202, 0x0000c600, 
    0xeb010200, 0xc80f8000, 0x00000000, 0x8b020203, 0x00000000, 0x00000000, 
    0x00000000
};
