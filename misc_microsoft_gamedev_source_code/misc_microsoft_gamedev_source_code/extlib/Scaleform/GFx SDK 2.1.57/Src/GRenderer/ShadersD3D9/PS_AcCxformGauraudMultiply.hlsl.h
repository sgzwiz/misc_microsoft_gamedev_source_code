#if 0
//
// Generated by 2.0.7776.0
//
//   fxc /T ps_1_1 /E PS_AcCxformGauraudMultiply scaleform.hlsl /Fh
//    PS_AcCxformGauraudMultiply.hlsl.h
//
//
// Parameters:
//
//   float4 cxadd;
//   float4 cxmul;
//
//
// Registers:
//
//   Name         Reg   Size
//   ------------ ----- ----
//   cxmul        c2       1
//   cxadd        c3       1
//

// Shader type: pixel 

xps_3_0
defconst cxadd, float, vector, [1, 4], c3
defconst cxmul, float, vector, [1, 4], c2
config AutoSerialize=false
config AutoResource=false
config PsMaxReg=2
// PsExportColorCount=1

dcl_color_centroid r0
dcl_color1_centroid r1

def c252, 0, 0, 0, 0
def c253, 0, 0, 0, 0
def c254, 0, 0, 0, 0
def c255, -1, 1, 0, 0


    alloc colors
    exec
    mad r2.w, r0.w, c2.w, c3.w
    add r2.xyz, c3.xyz, c255.x
    mad r2.xyz, r0.xyz, c2.xyz, r2.xyz
    mul r0.x, r2.w, r1.w
    subsc r2.w, -c255.y, -r0.x
    mad r1.xyz, r0.x, r2.xyz, c255.y
    exece
    mad r0.w, r0.x, r2.w, c255.y
    mul r0.xyz, r1.xyz, r0.w
    mov oC0, r0

// PDB hint 00000000-00000000-00000000

#endif

// This microcode is in native DWORD byte order.

const DWORD g_xps_PS_AcCxformGauraudMultiply[] =
{
    0x102a1100, 0x000000ec, 0x000000d0, 0x00000000, 0x00000024, 0x0000009c, 
    0x000000c4, 0x00000000, 0x00000000, 0x00000074, 0x0000001c, 0x00000069, 
    0xffff0300, 0x00000002, 0x0000001c, 0x00000000, 0x00000062, 0x00000044, 
    0x00020003, 0x00010000, 0x0000004c, 0x00000000, 0x0000005c, 0x00020002, 
    0x00010000, 0x0000004c, 0x00000000, 0x63786164, 0x6400abab, 0x00010003, 
    0x00010004, 0x00010000, 0x00000000, 0x63786d75, 0x6c007073, 0x5f335f30, 
    0x00322e30, 0x2e373737, 0x362e3000, 0x00000000, 0x00000001, 0x00000000, 
    0x00000000, 0x00000014, 0x01fc0010, 0x00000000, 0x00000000, 0x00000000, 
    0x00000000, 0x00000040, 0x00000090, 0x10000200, 0x00000000, 0x00000000, 
    0x00002042, 0x00000003, 0x00000001, 0x0000f0a0, 0x0000f1a1, 0x00000000, 
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0xbf800000, 
    0x3f800000, 0x00000000, 0x00000000, 0x00000000, 0x6002c400, 0x12000000, 
    0x00003008, 0x00002200, 0x00000000, 0xc8080002, 0x001b1b1b, 0x8b000203, 
    0xc8070002, 0x00c06c00, 0x2003ff00, 0xc8070002, 0x00c0c0c0, 0xab000202, 
    0xc8010000, 0x001b1b00, 0xe1020100, 0xb8800200, 0x01000080, 0xc20000ff, 
    0xc8070001, 0x006cc0b1, 0xcb0002ff, 0xc8080000, 0x006c1bb1, 0xcb0002ff, 
    0xc8070000, 0x00c01b00, 0xe1010000, 0xc80f8000, 0x00000000, 0xe2000000, 
    0x00000000, 0x00000000, 0x00000000
};
