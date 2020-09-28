#if 0
//
// Generated by 2.0.7776.0
//
//   fxc /T ps_2_0 /E PS_YCrCbAToRGBA bink.hlsl /Fh
//    ..\..\extlib\Bink\xbox\PS_YCrCbAToRGBA.hlsl.h
//
//
// Parameters:
//
//   bool bUseMaskTexture;
//   sampler2D tex0;
//   sampler2D tex1;
//   sampler2D tex2;
//   sampler2D tex3;
//   sampler2D tex4;
//
//
// Registers:
//
//   Name            Reg   Size
//   --------------- ----- ----
//   bUseMaskTexture b0       1
//   tex0            s0       1
//   tex1            s1       1
//   tex2            s2       1
//   tex3            s3       1
//   tex4            s4       1
//

// Shader type: pixel 

xps_3_0
defconst bUseMaskTexture, bool, scalar, [1, 1], b0
defconst tex0, sampler2d, object, [1, 1], s0
defconst tex1, sampler2d, object, [1, 1], s1
defconst tex2, sampler2d, object, [1, 1], s2
defconst tex3, sampler2d, object, [1, 1], s3
defconst tex4, sampler2d, object, [1, 1], s4
config AutoSerialize=false
config AutoResource=false
config PsMaxReg=2
// PsExportColorCount=1
// PsSampleControl=centers

dcl_texcoord r0.xy

def c252, 0, 0, 0, 0
def c253, 1.16412354, 1.59579468, -1.08166885, 0.529705048
def c254, -0.391448975, 1.16412354, -0.813476563, 2.01782227
def c255, -0.87065506, 0, 0, 0


    exec
    tfetch2D r1.___w, r0.xy, tf3
    tfetch2D r2.w___, r0.xy, tf0
    tfetch2D r2._w__, r0.xy, tf1
    tfetch2D r2.__w_, r0.xy, tf2
    serialize
    dp2add r1.x, r2.xy, c253.xy, c255.x
    mul r1.yz, r2.zzx, c254.wwy
    exec
    dp3 r0.w, r2.zxy, c254.xyz
  + adds r0.z, r1.zy
    add r1.yz, r0.wwz, c253.wwz
    cexec b128
    tfetch2D r0.w___, r0.xy, tf4
    alloc colors
    cexec b128
    mul r1.w, r1.w, r0.x
    exece
    mov oC0, r1

// PDB hint 00000000-00000000-00000000

#endif

// This microcode is in native DWORD byte order.

const DWORD g_xps_PS_YCrCbAToRGBA[] =
{
    0x102a1100, 0x00000168, 0x000000f4, 0x00000000, 0x00000024, 0x0000011c, 
    0x00000144, 0x00000000, 0x00000000, 0x000000f4, 0x0000001c, 0x000000e7, 
    0xffff0300, 0x00000006, 0x0000001c, 0x00000000, 0x000000e0, 0x00000094, 
    0x00000000, 0x00010000, 0x000000a4, 0x00000000, 0x000000b4, 0x00030000, 
    0x00010000, 0x000000bc, 0x00000000, 0x000000cc, 0x00030001, 0x00010000, 
    0x000000bc, 0x00000000, 0x000000d1, 0x00030002, 0x00010000, 0x000000bc, 
    0x00000000, 0x000000d6, 0x00030003, 0x00010000, 0x000000bc, 0x00000000, 
    0x000000db, 0x00030004, 0x00010000, 0x000000bc, 0x00000000, 0x62557365, 
    0x4d61736b, 0x54657874, 0x75726500, 0x00000001, 0x00010001, 0x00010000, 
    0x00000000, 0x74657830, 0x00ababab, 0x0004000c, 0x00010001, 0x00010000, 
    0x00000000, 0x74657831, 0x00746578, 0x32007465, 0x78330074, 0x65783400, 
    0x70735f33, 0x5f300032, 0x2e302e37, 0x3737362e, 0x3000abab, 0x00000000, 
    0x00000001, 0x00000000, 0x00000000, 0x00000014, 0x01fc0010, 0x00000000, 
    0x00000000, 0x00000000, 0x00000000, 0x00000040, 0x000000b4, 0x10000200, 
    0x00000004, 0x00020000, 0x00000821, 0x00010001, 0x00000001, 0x00003050, 
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x3f950200, 0x3fcc4300, 
    0xbf8a7420, 0x3f079ac0, 0xbec86c00, 0x3f950200, 0xbf504000, 0x40012400, 
    0xbf5ee340, 0x00000000, 0x00000000, 0x00000000, 0x02556003, 0x20091200, 
    0x12000000, 0x0001100b, 0x0000d600, 0xc4000000, 0x0000100c, 0x100dd600, 
    0x22000000, 0x10381001, 0x1f1ff7ff, 0x00004000, 0x10082001, 0x1f1ffffb, 
    0x00004000, 0x10182001, 0x1f1fffdf, 0x00004000, 0x10282001, 0x1f1ffeff, 
    0x00004000, 0xc8010001, 0x00b0b06c, 0x9102fdff, 0xc8060001, 0x0066bb00, 
    0xa102fe00, 0x00480000, 0x00bec0c1, 0xb002fe01, 0xc8060001, 0x00cbcb00, 
    0xa000fd00, 0x10480001, 0x1f1ffffb, 0x00004000, 0xc8080001, 0x001b6c00, 
    0xe1010000, 0xc80f8000, 0x00000000, 0xe2010100, 0x00000000, 0x00000000, 
    0x00000000
};
