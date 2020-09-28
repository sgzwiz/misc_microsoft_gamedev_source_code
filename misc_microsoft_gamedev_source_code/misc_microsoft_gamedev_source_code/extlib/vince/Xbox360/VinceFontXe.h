//-----------------------------------------------------------------------------
//  VinceFontXe.h : This is a version of 3d Fonts for use on Xenon versions
//  of VINCE using DX9. Adapted from sample code in Xenon XDK.
//
//	Created 2004/08/23 Rich Bonny <rbonny@microsoft.com>
//
// Texture-based font class. This class reads .abc font files that are generated by the
// FontMaker tool. These .abc files are used to create a texture with all the font's
// glyph, and also extract information on the dimensions of each glyph.
//
// Once created, this class is used to render text in a 3D scene with the following
// function:
//    DrawText( fScreenY, fScreenSpaceY, dwTextColor, strText,
//              dwJustificationFlags );
//
// For performance, you should batch multiple DrawText calls together between Begin() and
// End() calls, as in the following example:
//    pFont->Begin();
//    pFont->DrawText( ... );
//    pFont->DrawText( ... );
//    pFont->DrawText( ... );
//    pFont->End();
//
// The size (extent) of the text can be computed without rendering with the following
// function:
//    GetTextExtent( strText, pfReturnedWidth, pfReturnedHeight,
//                   bComputeExtentUsingFirstLineOnly );
//
// Finally, the font class can create a texture to hold rendered text, which is useful
// for static text that must be rendered for many frames, or can even be used within a
// 3D scene. (For instance, for a player's name on a jersey.) Use the following function
// for this:
//    CreateTexture( strText, d3dTextureFormat );
//
// Adapted from AtgFont class developed by Xbox Advanced Technology Group.
//--------------------------------------------------------------------------------------
//	MICROSOFT CONFIDENTIAL.  DO NOT DISTRIBUTE.
//	Copyright (c) 2004 Microsoft Corp.  All rights reserved.
//-----------------------------------------------------------------------------

#pragma once

#include <xtl.h>
#include "VinceResourceXe.h"

//--------------------------------------------------------------------------------------
// Flags for the CVinceFont::DrawText() function
//--------------------------------------------------------------------------------------
#define FONT_LEFT       0x00000000
#define FONT_RIGHT      0x00000001
#define FONT_CENTER_X   0x00000002
#define FONT_CENTER_Y   0x00000004
#define FONT_TRUNCATED  0x00000008

//--------------------------------------------------------------------------------------
// Name: VINCE_GLYPH_ATTR
// Desc: Structure to hold information about one glyph (font character image)
//--------------------------------------------------------------------------------------
struct VINCE_GLYPH_ATTR
{
    WORD  tu1, tv1, tu2, tv2; // Texture coordinates for the image
    SHORT wOffset;            // Pixel offset for glyph start
    SHORT wWidth;             // Pixel width of the glyph
    SHORT wAdvance;           // Pixels to advance after the glyph
    WORD  wMask;              // Channel mask
};

//--------------------------------------------------------------------------------------
// Defines for special characters. Note this depends on the font used, but all samples
// are coded to use the same basic font
//--------------------------------------------------------------------------------------
#define GLYPH_A_BUTTON      L"\400"
#define GLYPH_B_BUTTON      L"\401"
#define GLYPH_X_BUTTON      L"\402"
#define GLYPH_Y_BUTTON      L"\403"
#define GLYPH_WHITE_BUTTON  L"\404"
#define GLYPH_BLACK_BUTTON  L"\405"
#define GLYPH_LEFT_BUTTON   L"\406"
#define GLYPH_RIGHT_BUTTON  L"\407"
#define GLYPH_START_BUTTON  L"\410\411"
#define GLYPH_BACK_BUTTON   L"\412\413"

#define GLYPH_LEFT_HAND     L"\414"
#define GLYPH_RIGHT_HAND    L"\415"
#define GLYPH_UP_HAND       L"\416"
#define GLYPH_DOWN_HAND     L"\417"

#define GLYPH_BULLET        L"\500"
#define GLYPH_STAR_1        L"\501"
#define GLYPH_STAR_2        L"\502"
#define GLYPH_STAR_3        L"\503"
#define GLYPH_STAR_4        L"\504"
#define GLYPH_STAR_5        L"\505"

#define GLYPH_BIG_X         L"\506"
#define GLYPH_HAND          L"\507"
#define GLYPH_CHECK_MARK    L"\510"
#define GLYPH_X_MARK        L"\511"
#define GLYPH_SKULL         L"\512"

#define GLYPH_LEFT_TICK     L"\514"
#define GLYPH_RIGHT_TICK    L"\515"
#define GLYPH_UP_TICK       L"\516"
#define GLYPH_DOWN_TICK     L"\517"

#define GLYPH_HOLLOW_CIRCLE L"\600"
#define GLYPH_FILLED_CIRCLE L"\601"

#define GLYPH_LR_ARROW      L"\612"
#define GLYPH_UD_ARROW      L"\613"
#define GLYPH_LEFT_ARROW    L"\614"
#define GLYPH_RIGHT_ARROW   L"\615"
#define GLYPH_UP_ARROW      L"\616"
#define GLYPH_DOWN_ARROW    L"\617"

namespace Vince
{
	//-----------------------------------------------------------------------------
	// Name: class CVinceFont
	// Desc: Texture-based font class for doing text in a 3D scene.
	//-----------------------------------------------------------------------------
	class CVinceFont
	{
	public:

		// Font vertical dimensions
		FLOAT         m_fFontHeight;
		FLOAT         m_fFontTopPadding;
		FLOAT         m_fFontBottomPadding;
		FLOAT         m_fFontYAdvance;

		FLOAT         m_fXScaleFactor;
		FLOAT         m_fYScaleFactor;
		FLOAT         m_fSlantFactor;

		D3DRECT       m_rcWindow;
		FLOAT         m_fCursorX;
		FLOAT         m_fCursorY;

		// Translator table for supporting unicode ranges
		WCHAR         m_cMaxGlyph;
		SHORT*        m_TranslatorTable;

		// Glyph data for the font
		DWORD               m_dwNumGlyphs;
		VINCE_GLYPH_ATTR*   m_Glyphs;

		// Reference counter; may be unnecessary. - RWB
		DWORD         m_dwNestedBeginCount;

		// D3D rendering objects
		LPDIRECT3DDEVICE9       m_pd3dDevice; // A D3DDevice used for rendering
		VincePackedResource m_xprResource;
		D3DTexture*   m_pFontTexture;

	public:
		// Access functions for debugging purposes
		IDirect3DTexture9* GetTexture() const    { return m_pFontTexture; }
		FLOAT              GetFontHeight() const { return m_fFontYAdvance; }

	public:
		CVinceFont();
		~CVinceFont();

		// Functions to create and destroy the internal objects
		HRESULT Create( const CHAR* strFontFileName );
		HRESULT Create( IDirect3DTexture9* pFontTexture, const VOID* pFontData );
		HRESULT CreateFontShaders();
		void SetDevice( LPDIRECT3DDEVICE9 pd3dDevice );
		VOID    Destroy();

		// Returns the dimensions of a text string
		VOID    GetTextExtent( const WCHAR* strText, FLOAT* pWidth, 
							FLOAT* pHeight, BOOL bFirstLineOnly=FALSE ) const;
		FLOAT   GetTextWidth( const WCHAR* strText ) const;

		VOID    SetWindow( D3DRECT rcWindow );
		VOID    SetWindow( LONG x1, LONG y1, LONG x2, LONG y2 );
		VOID    SetCursorPosition( FLOAT fCursorX, FLOAT fCursorY );
		VOID    SetScaleFactors( FLOAT fXScaleFactor, FLOAT fYScaleFactor );
		VOID    SetSlantFactor( FLOAT fSlantFactor );

		// Function to create a texture containing rendered text
		IDirect3DTexture9* CreateTexture( const WCHAR* strText, 
										D3DCOLOR dwBackgroundColor = 0x00000000,
										D3DCOLOR dwTextColor = 0xffffffff,
										D3DFORMAT d3dFormat = D3DFMT_A8R8G8B8 );

		// Public calls to render text. Callers can simply call DrawText(), but for
		// performance, they should batch multiple calls together, bracketed by calls to
		// Begin() and End().
		VOID Begin();
		VOID DrawText( DWORD dwColor, const WCHAR* strText, DWORD dwFlags=0L,
					FLOAT fMaxPixelWidth = 0.0f );
		VOID DrawText( FLOAT sx, FLOAT sy, DWORD dwColor, const WCHAR* strText,
					DWORD dwFlags=0L, FLOAT fMaxPixelWidth = 0.0f );
		VOID DrawBox(float x1, float y1, float x2, float y2, DWORD dwOutlineColor, DWORD dwFillColor);
		VOID End();

	};

}	// namespace VINCE
