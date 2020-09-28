/**********************************************************************

Filename    :   OpenGLWin32App.h
Content     :   Win32 OpenGL based FxApp class header.
Created     :   January 10, 2008
Authors     :   Michael Antonov

Copyright   :   (c) 2005-2006 Scaleform Corp. All Rights Reserved.

This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING 
THE WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR ANY PURPOSE.

**********************************************************************/

#ifndef INC_OpenGLWin32App_H
#define INC_OpenGLWin32App_H

//#include <windows.h>
#include "FxWin32App.h"

// *** Open GL Includes
#include <gl/gl.h>
#include <gl/glu.h>
// These links can be downloaded and copied to the include directory:
// http://oss.sgi.com/projects/ogl-sample/ABI/glext.h
// http://oss.sgi.com/projects/ogl-sample/ABI/wglext.h
#include "glext.h"
#include "wglext.h"

#include "GRendererOGL.h"


// ***** Application class

class FxDeviceOpenGL;
class OpenGLWin32App :  public FxWin32App
{
public:

    // **** Public Interface

    OpenGLWin32App();
    ~OpenGLWin32App();
 
    virtual bool       CreateRenderer();

    // Presents the data (swaps the back and front buffers)
    virtual void       PresentFrame();

    // Resets the direct3D, return 1 if successful.
    // On successful reset, this function will call InitRenderer again.
    virtual bool       RecreateRenderer();

    // Sizing; by default, re-initalizes the renderer
    virtual void       ResizeWindow(int w, int h);

     // Draw a text string (specify top-left corner of characters, NOT baseline)
    virtual void       DrawText(int x, int y, const char *ptext, unsigned int color);
    // Compute text size that will be generated by DrawText
    virtual void       CalcDrawTextSize(int *pw, int *ph, const char *ptext);

    virtual GRenderer* GetRenderer();

    virtual void       SwitchFSAA(bool on_off);
    virtual void       SetVSync(bool isEnabled);

    virtual DisplayStatus  CheckDisplayStatus() const;

    // *** Implementation

    PFNGLMULTITEXCOORD2FARBPROC     pglMultiTexCoord2f;
    PFNGLCLIENTACTIVETEXTUREARBPROC pglActiveTexture;
    PFNGLACTIVETEXTUREARBPROC       pglClientActiveTexture;
    PFNGLPOINTPARAMETERFVEXTPROC    pglPointParameterfvEXT;
    PFNWGLSWAPINTERVALEXTPROC       pwglSwapIntervalEXT;

    mutable PFNGLBINDRENDERBUFFEREXTPROC                        p_glBindRenderbufferEXT;
    mutable PFNGLDELETERENDERBUFFERSEXTPROC                     p_glDeleteRenderBuffersEXT;
    mutable PFNGLGENRENDERBUFFERSEXTPROC                        p_glGenRenderbuffersEXT;
    mutable PFNGLBINDFRAMEBUFFEREXTPROC                         p_glBindFramebufferEXT;
    mutable PFNGLDELETEFRAMEBUFFERSEXTPROC                      p_glDeleteFramebuffersEXT;
    mutable PFNGLGENFRAMEBUFFERSEXTPROC                         p_glGenFramebuffersEXT;
    mutable PFNGLRENDERBUFFERSTORAGEEXTPROC                     p_glRenderbufferStorageEXT;
    mutable PFNGLFRAMEBUFFERTEXTURE2DEXTPROC                    p_glFramebufferTexture2DEXT;
    mutable PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC                 p_glFramebufferRenderbufferEXT;
    mutable PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVEXTPROC     p_glFramebufferAttachmentParameterivEXT;
    mutable PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC                  p_glCheckFramebufferStatusEXT;
    mutable PFNGLGENERATEMIPMAPEXTPROC                          p_glGenerateMipmapEXT;

    // Handles..
    HDC         hDC;
    HGLRC       hGLRC;

    GPtr<GRendererOGL>   pRenderer;
    // Saved viewport
    GLint       ViewportSave[4];
    // OpenGL Text state
    enum {
        TextDisplayListBase         = 1000,
        TextDisplayListCharCount    = 255
    };
    // Text metric of the font.
    TEXTMETRIC TextMetric;
    // This flag is set if text lists were created successfully.
    bool        TextListsCreated;

    // Helpers
    bool        SetupPixelFormat();
//  bool InitializeExtensions();
//  bool CheckExtension(char* extensionName);

    static bool CheckExtension (const char *exts, const char *name)
    {
        const char *p = strstr(exts, name);
        return (p && (p[strlen(name)] == 0 || p[strlen(name)] == ' '));
    }

    void  glBindRenderbufferEXT (GLenum t, GLuint rb) const
    {
        p_glBindRenderbufferEXT(t,rb);
    }

    void  glDeleteRenderbuffersEXT (GLsizei n, const GLuint *b) const
    {
        p_glDeleteRenderBuffersEXT(n,b);
    }

    void  glGenRenderbuffersEXT (GLsizei n, GLuint *b) const
    {
        p_glGenRenderbuffersEXT(n,b);
    }

    void  glRenderbufferStorageEXT (GLenum t, GLenum f, GLsizei w, GLsizei h)
    {
        p_glRenderbufferStorageEXT(t,f,w,h);
    }

    void  glBindFramebufferEXT (GLenum t, GLuint b) const
    {
        p_glBindFramebufferEXT(t,b);
    }

    void  glDeleteFramebuffersEXT (GLsizei n, const GLuint *b) const
    {
        p_glDeleteFramebuffersEXT(n,b);
    }

    void  glGenFramebuffersEXT (GLsizei n, GLuint *b) const
    {
        p_glGenFramebuffersEXT(n,b);
    }

    GLenum  glCheckFramebufferStatusEXT (GLenum p)
    {
        return p_glCheckFramebufferStatusEXT(p);
    }
    void  glFramebufferTexture1DEXT (GLenum, GLenum, GLenum, GLuint, GLint);

    void  glFramebufferTexture2DEXT (GLenum t, GLenum a, GLenum tt, GLuint o, GLint l) const
    {
        p_glFramebufferTexture2DEXT(t,a,tt,o,l);
    }

    void  glFramebufferRenderbufferEXT (GLenum t, GLenum a, GLenum rt, GLuint r) const
    {
        p_glFramebufferRenderbufferEXT(t,a,rt,r);
    }

    void  glGetFramebufferAttachmentParameterivEXT (GLenum t, GLenum a, GLenum q, GLint *v) const
    {
        p_glFramebufferAttachmentParameterivEXT(t,a,q,v);
    }

    void glGenerateMipmapEXT (GLenum t) const
    {
        p_glGenerateMipmapEXT(t);
    }

    // Called from a SetupWindow to initialize specific graphic device 
    virtual     bool        SetupWindowDevice();
    // Called from a KillWindow to shutdown specific graphic device 
    virtual     void        KillWindowDevice();

};


#endif // INC_OpenGLWin32App_H