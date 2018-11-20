/* simple glx driver for TinyGL */
#include <GL/glx.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <X11/extensions/XShm.h>
#include "zgl.h"

#include "c24bitmap.h"
#include "c256bitmap.h"


 

Bool glXQueryExtension( Display *dpy, int *errorb, int *event )
{
    return True;
}





GLXContext glXCreateContext( Display *dpy, XVisualInfo *vis,
                             GLXContext shareList, Bool direct )
{
  TinyGLXContext *ctx;

  if (shareList != NULL) {
    gl_fatal_error("No sharing available in TinyGL");
  }
  ctx=gl_malloc(sizeof(TinyGLXContext));
  ctx->gl_context=NULL;
  //khdev ctx->visual_info=*vis;
  return (GLXContext) ctx;
}


void glXDestroyContext( Display *dpy, GLXContext ctx1 )
{
  TinyGLXContext *ctx = (TinyGLXContext *) ctx1;
  if (ctx->gl_context != NULL) {
    glClose();
  }
  gl_free(ctx);
}


static int glxXErrorFlag=0;

static int glxHandleXError(Display *dpy,XErrorEvent *event)
{
  glxXErrorFlag=1;
  return 0;
}

static int bits_per_pixel(Display *dpy, XVisualInfo *visinfo)
{
   XImage *img;
   int bpp;
   char *data;

   data = gl_malloc(8);
   if (data == NULL) 
       return visinfo->depth;

   return 24;

   bpp = img->bits_per_pixel;
   gl_free(data);
   img->data = NULL;
   XDestroyImage(img);
   return bpp;
}

 
/* resize the glx viewport : we try to use the xsize and ysize
   given. We return the effective size which is guaranted to be smaller */

int glX_resize_viewport(GLContext *c,int *xsize_ptr,int *ysize_ptr)
{
  TinyGLXContext *ctx;
  int xsize,ysize;

  ctx=(TinyGLXContext *)c->opaque;

  xsize=*xsize_ptr;
  ysize=*ysize_ptr;

  /* we ensure that xsize and ysize are multiples of 2 for the zbuffer. 
     TODO: find a better solution */
  xsize&=~3;
  ysize&=~3;

  if (xsize == 0 || ysize == 0) return -1;

  *xsize_ptr=xsize;
  *ysize_ptr=ysize;

  //if (ctx->ximage != NULL) free_ximage(ctx);
  
  ctx->xsize=xsize;
  ctx->ysize=ysize;

  //if (create_ximage(ctx,ctx->xsize,ctx->ysize,ctx->visual_info.depth) != 0) 
  //  return -1;

  /* resize the Z buffer */
  if (ctx->do_convert) {
    ZB_resize(c->zb,NULL,xsize,ysize);
  } else {
    ZB_resize(c->zb,ctx->ximage->data,xsize,ysize);
  }
  return 0;
}


static Bool WaitForShmCompletion(Display *dpy, XEvent *event, char *arg) 
{
    TinyGLXContext *ctx=(TinyGLXContext *) arg;

    return (event->type == ctx->CompletionType) && 
        ( ((XShmCompletionEvent *)event)->drawable == (Window)ctx->drawable); 
}

void glXSwapBuffers( Display *dpy, GLXDrawable drawable )
{
  GLContext *gl_context;
  TinyGLXContext *ctx;

  /* retrieve the current GLXContext */
  gl_context=gl_get_context();
  ctx=(TinyGLXContext *)gl_context->opaque;

  C24BitMap  CPic;
  CPic.FormatF(ctx->image_w, ctx->image_h);
  ZBuffer * zb = ctx->gl_context->zb;
  for(int i=0;i< ctx->image_h ;i++)
     {
      char* ptr = (char*)zb->pbuf;
      ptr = &ptr[zb->linesize*i]; 
      for(int j=0; j<ctx->image_w;j++)
      {
        C24PixVal Pix = get_pix_color(CPic, j, i);
		//ptr++;
        *Pix.r = *ptr; ptr++;      
        *Pix.g = *ptr; ptr++;
		*Pix.b = *ptr; ptr++;  		
      }
     }
  CPic.Save("img.bmp");
  return;
  /* for non 16 bits visuals, a conversion is required */
  

  if (ctx->do_convert) {
    ZB_copyFrameBuffer(ctx->gl_context->zb,
                       ctx->ximage->data,
                       ctx->ximage->bytes_per_line);

  }

  /* draw the ximage */
  if (ctx->shm_use) {
      XEvent event;

      XShmPutImage(dpy,drawable,ctx->gc,
                   ctx->ximage,0,0,0,0,ctx->ximage->width, ctx->ximage->height,
                   True);
      XIfEvent(dpy, &event, WaitForShmCompletion, (char*)ctx);
  } else {
    XPutImage(dpy, drawable, ctx->gc, 
              ctx->ximage, 0, 0, 0, 0, ctx->ximage->width, ctx->ximage->height);
  }
  XFlush(dpy);
}


void glXWaitGL( void )
{
}

void glXWaitX( void )
{
}
