/**********************************************************
 * Copyright 2008-2009 VMware, Inc.  All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 **********************************************************/

#include "util/u_inlines.h"
#include "pipe/p_defines.h"
#include "util/u_math.h"
#include "util/u_format.h"

#include "svga_context.h"
#include "svga_state.h"
#include "svga_cmd.h"
#include "svga_debug.h"
#include "svga_screen.h"
#include "svga_surface.h"
#include "svga_resource_texture.h"


/*
 * flush our command buffer after the 8th distinct render target
 *
 * This helps improve the surface cache behaviour in the face of the
 * large number of single-use render targets generated by EXA and the xorg
 * state tracker.  Without this we can reference hundreds of individual
 * render targets from a command buffer, which leaves little scope for
 * sharing or reuse of those targets.
 */
#define MAX_RT_PER_BATCH 8



static enum pipe_error
emit_fb_vgpu9(struct svga_context *svga)
{
   struct svga_screen *svgascreen = svga_screen(svga->pipe.screen);
   const struct pipe_framebuffer_state *curr = &svga->curr.framebuffer;
   struct pipe_framebuffer_state *hw = &svga->state.hw_clear.framebuffer;
   boolean reemit = svga->rebind.flags.rendertargets;
   unsigned i;
   enum pipe_error ret;

   assert(!svga_have_vgpu10(svga));

   /*
    * We need to reemit non-null surface bindings, even when they are not
    * dirty, to ensure that the resources are paged in.
    */

   for (i = 0; i < svgascreen->max_color_buffers; i++) {
      if ((curr->cbufs[i] != hw->cbufs[i]) || (reemit && hw->cbufs[i])) {
         if (svga->curr.nr_fbs++ > MAX_RT_PER_BATCH)
            return PIPE_ERROR_OUT_OF_MEMORY;

         /* Check to see if we need to propagate the render target surface */
         if (hw->cbufs[i] && svga_surface_needs_propagation(hw->cbufs[i]))
            svga_propagate_surface(svga, hw->cbufs[i], TRUE);

         ret = SVGA3D_SetRenderTarget(svga->swc, SVGA3D_RT_COLOR0 + i,
                                      curr->cbufs[i]);
         if (ret != PIPE_OK)
            return ret;

         pipe_surface_reference(&hw->cbufs[i], curr->cbufs[i]);
      }

      /* Set the rendered-to flag */
      struct pipe_surface *s = curr->cbufs[i];
      if (s) {
         svga_set_texture_rendered_to(svga_texture(s->texture),
                                      s->u.tex.first_layer, s->u.tex.level);
      }
   }

   if ((curr->zsbuf != hw->zsbuf) || (reemit && hw->zsbuf)) {
      ret = SVGA3D_SetRenderTarget(svga->swc, SVGA3D_RT_DEPTH, curr->zsbuf);
      if (ret != PIPE_OK)
         return ret;

      /* Check to see if we need to propagate the depth stencil surface */
      if (hw->zsbuf && svga_surface_needs_propagation(hw->zsbuf))
         svga_propagate_surface(svga, hw->zsbuf, TRUE);

      if (curr->zsbuf &&
          util_format_is_depth_and_stencil(curr->zsbuf->format)) {
         ret = SVGA3D_SetRenderTarget(svga->swc, SVGA3D_RT_STENCIL,
                                      curr->zsbuf);
         if (ret != PIPE_OK)
            return ret;
      }
      else {
         ret = SVGA3D_SetRenderTarget(svga->swc, SVGA3D_RT_STENCIL, NULL);
         if (ret != PIPE_OK)
            return ret;
      }

      pipe_surface_reference(&hw->zsbuf, curr->zsbuf);

      /* Set the rendered-to flag */
      struct pipe_surface *s = curr->zsbuf;
      if (s) {
         svga_set_texture_rendered_to(svga_texture(s->texture),
                                      s->u.tex.first_layer, s->u.tex.level);
      }
   }

   return PIPE_OK;
}


/*
 * Rebind rendertargets.
 *
 * Similar to emit_framebuffer, but without any state checking/update.
 *
 * Called at the beginning of every new command buffer to ensure that
 * non-dirty rendertargets are properly paged-in.
 */
static enum pipe_error
svga_reemit_framebuffer_bindings_vgpu9(struct svga_context *svga)
{
   struct svga_screen *svgascreen = svga_screen(svga->pipe.screen);
   struct pipe_framebuffer_state *hw = &svga->state.hw_clear.framebuffer;
   unsigned i;
   enum pipe_error ret;

   assert(!svga_have_vgpu10(svga));

   for (i = 0; i < svgascreen->max_color_buffers; i++) {
      if (hw->cbufs[i]) {
         ret = SVGA3D_SetRenderTarget(svga->swc, SVGA3D_RT_COLOR0 + i,
                                      hw->cbufs[i]);
         if (ret != PIPE_OK) {
            return ret;
         }
      }
   }

   if (hw->zsbuf) {
      ret = SVGA3D_SetRenderTarget(svga->swc, SVGA3D_RT_DEPTH, hw->zsbuf);
      if (ret != PIPE_OK) {
         return ret;
      }

      if (hw->zsbuf &&
          util_format_is_depth_and_stencil(hw->zsbuf->format)) {
         ret = SVGA3D_SetRenderTarget(svga->swc, SVGA3D_RT_STENCIL, hw->zsbuf);
         if (ret != PIPE_OK) {
            return ret;
         }
      }
      else {
         ret = SVGA3D_SetRenderTarget(svga->swc, SVGA3D_RT_STENCIL, NULL);
         if (ret != PIPE_OK) {
            return ret;
         }
      }
   }

   return PIPE_OK;
}



static enum pipe_error
emit_fb_vgpu10(struct svga_context *svga)
{
   const struct svga_screen *ss = svga_screen(svga->pipe.screen);
   struct pipe_surface *rtv[SVGA3D_MAX_RENDER_TARGETS];
   struct pipe_surface *dsv;
   struct pipe_framebuffer_state *curr = &svga->curr.framebuffer;
   struct pipe_framebuffer_state *hw = &svga->state.hw_clear.framebuffer;
   const unsigned num_color = MAX2(curr->nr_cbufs, hw->nr_cbufs);
   int last_rtv = -1;
   unsigned i;
   enum pipe_error ret = PIPE_OK;

   assert(svga_have_vgpu10(svga));

   /* Reset the has_backed_views flag.
    * The flag is set in svga_validate_surface_view() if
    * a backed surface view is used.
    */
   svga->state.hw_draw.has_backed_views = FALSE;

   /* Setup render targets array.  Note that we loop over the max of the
    * number of previously bound buffers and the new buffers to unbind
    * any previously bound buffers when the new number of buffers is less
    * than the old number of buffers.
    */
   for (i = 0; i < num_color; i++) {
      if (curr->cbufs[i]) {
         struct pipe_surface *s = curr->cbufs[i];

         rtv[i] = svga_validate_surface_view(svga, svga_surface(s));
         if (rtv[i] == NULL) {
            return PIPE_ERROR_OUT_OF_MEMORY;
         }

         assert(svga_surface(rtv[i])->view_id != SVGA3D_INVALID_ID);
         last_rtv = i;

         /* Set the rendered-to flag */
         svga_set_texture_rendered_to(svga_texture(s->texture),
                                      s->u.tex.first_layer, s->u.tex.level);
      }
      else {
         rtv[i] = NULL;
      }
   }

   /* Setup depth stencil view */
   if (curr->zsbuf) {
      struct pipe_surface *s = curr->zsbuf;

      dsv = svga_validate_surface_view(svga, svga_surface(curr->zsbuf));
      if (!dsv) {
         return PIPE_ERROR_OUT_OF_MEMORY;
      }

      /* Set the rendered-to flag */
      svga_set_texture_rendered_to(svga_texture(s->texture),
                                      s->u.tex.first_layer, s->u.tex.level);
   }
   else {
      dsv = NULL;
   }

   /* avoid emitting redundant SetRenderTargets command */
   if ((num_color != svga->state.hw_clear.num_rendertargets) ||
       (dsv != svga->state.hw_clear.dsv) ||
       memcmp(rtv, svga->state.hw_clear.rtv, num_color * sizeof(rtv[0]))) {

      ret = SVGA3D_vgpu10_SetRenderTargets(svga->swc, num_color, rtv, dsv);
      if (ret != PIPE_OK)
         return ret;

      /* number of render targets sent to the device, not including trailing
       * unbound render targets.
       */
      svga->state.hw_clear.num_rendertargets = last_rtv + 1;
      svga->state.hw_clear.dsv = dsv;
      memcpy(svga->state.hw_clear.rtv, rtv, num_color * sizeof(rtv[0]));
    
      for (i = 0; i < ss->max_color_buffers; i++) {
         if (hw->cbufs[i] != curr->cbufs[i]) {
            /* propagate the backed view surface before unbinding it */
            if (hw->cbufs[i] && svga_surface(hw->cbufs[i])->backed) {
               svga_propagate_surface(svga,
                                      &svga_surface(hw->cbufs[i])->backed->base,
                                      TRUE);
            }
            pipe_surface_reference(&hw->cbufs[i], curr->cbufs[i]);
         }
      }
      hw->nr_cbufs = curr->nr_cbufs;

      if (hw->zsbuf != curr->zsbuf) {
         /* propagate the backed view surface before unbinding it */
         if (hw->zsbuf && svga_surface(hw->zsbuf)->backed) {
            svga_propagate_surface(svga, &svga_surface(hw->zsbuf)->backed->base,
                                   TRUE);
         }
         pipe_surface_reference(&hw->zsbuf, curr->zsbuf);
      }
   }

   return ret;
}


static enum pipe_error
emit_framebuffer(struct svga_context *svga, unsigned dirty)
{
   if (svga_have_vgpu10(svga)) {
      return emit_fb_vgpu10(svga);
   }
   else {
      return emit_fb_vgpu9(svga);
   }
}


/*
 * Rebind rendertargets.
 *
 * Similar to emit_framebuffer, but without any state checking/update.
 *
 * Called at the beginning of every new command buffer to ensure that
 * non-dirty rendertargets are properly paged-in.
 */
enum pipe_error
svga_reemit_framebuffer_bindings(struct svga_context *svga)
{
   enum pipe_error ret;

   assert(svga->rebind.flags.rendertargets);

   if (svga_have_vgpu10(svga)) {
      ret = emit_fb_vgpu10(svga);
   }
   else {
      ret = svga_reemit_framebuffer_bindings_vgpu9(svga);
   }

   svga->rebind.flags.rendertargets = FALSE;

   return ret;
}


/*
 * Send a private allocation command to page in rendertargets resource.
 */
enum pipe_error
svga_rebind_framebuffer_bindings(struct svga_context *svga)
{
   struct svga_hw_clear_state *hw = &svga->state.hw_clear;
   unsigned i;
   enum pipe_error ret;

   assert(svga_have_vgpu10(svga));

   if (!svga->rebind.flags.rendertargets)
      return PIPE_OK;

   for (i = 0; i < hw->num_rendertargets; i++) {
      if (hw->rtv[i]) {
         ret = svga->swc->resource_rebind(svga->swc,
                                          svga_surface(hw->rtv[i])->handle,
                                          NULL,
                                          SVGA_RELOC_WRITE);
         if (ret != PIPE_OK)
            return ret;
      }
   }

   if (hw->dsv) {
      ret = svga->swc->resource_rebind(svga->swc,
                                       svga_surface(hw->dsv)->handle,
                                       NULL,
                                       SVGA_RELOC_WRITE);
      if (ret != PIPE_OK)
         return ret;
   }

   svga->rebind.flags.rendertargets = 0;

   return PIPE_OK;
}


struct svga_tracked_state svga_hw_framebuffer =
{
   "hw framebuffer state",
   SVGA_NEW_FRAME_BUFFER,
   emit_framebuffer
};




/***********************************************************************
 */

static enum pipe_error
emit_viewport( struct svga_context *svga,
               unsigned dirty )
{
   const struct pipe_viewport_state *viewport = &svga->curr.viewport;
   struct svga_prescale prescale;
   SVGA3dRect rect;
   /* Not sure if this state is relevant with POSITIONT.  Probably
    * not, but setting to 0,1 avoids some state pingponging.
    */
   float range_min = 0.0;
   float range_max = 1.0;
   float flip = -1.0;
   boolean degenerate = FALSE;
   boolean invertY = FALSE;
   enum pipe_error ret;

   float fb_width = (float) svga->curr.framebuffer.width;
   float fb_height = (float) svga->curr.framebuffer.height;

   float fx =        viewport->scale[0] * -1.0f + viewport->translate[0];
   float fy = flip * viewport->scale[1] * -1.0f + viewport->translate[1];
   float fw =        viewport->scale[0] * 2.0f;
   float fh = flip * viewport->scale[1] * 2.0f;
   boolean emit_vgpu10_viewport = FALSE;

   memset( &prescale, 0, sizeof(prescale) );

   /* Examine gallium viewport transformation and produce a screen
    * rectangle and possibly vertex shader pre-transformation to
    * get the same results.
    */

   SVGA_DBG(DEBUG_VIEWPORT,
            "\ninitial %f,%f %fx%f\n",
            fx,
            fy,
            fw,
            fh);

   prescale.scale[0] = 1.0;
   prescale.scale[1] = 1.0;
   prescale.scale[2] = 1.0;
   prescale.scale[3] = 1.0;
   prescale.translate[0] = 0;
   prescale.translate[1] = 0;
   prescale.translate[2] = 0;
   prescale.translate[3] = 0;

   /* Enable prescale to adjust vertex positions to match
      VGPU10 convention only if rasterization is enabled.
    */
   if (svga->curr.rast && svga->curr.rast->templ.rasterizer_discard) {
      degenerate = TRUE;
      goto out;
   } else {
      prescale.enabled = TRUE;
   }

   if (fw < 0) {
      prescale.scale[0] *= -1.0f;
      prescale.translate[0] += -fw;
      fw = -fw;
      fx = viewport->scale[0] * 1.0f + viewport->translate[0];
   }

   if (fh < 0.0) {
      if (svga_have_vgpu10(svga)) {
         /* floating point viewport params below */
         prescale.translate[1] = fh + fy * 2.0f;
      }
      else {
         /* integer viewport params below */
         prescale.translate[1] = fh - 1.0f + fy * 2.0f;
      }
      fh = -fh;
      fy -= fh;
      prescale.scale[1] = -1.0f;
      invertY = TRUE;
   }

   if (fx < 0) {
      prescale.translate[0] += fx;
      prescale.scale[0] *= fw / (fw + fx);
      fw += fx;
      fx = 0.0f;
   }

   if (fy < 0) {
      if (invertY) {
         prescale.translate[1] -= fy;
      }
      else {
         prescale.translate[1] += fy;
      }
      prescale.scale[1] *= fh / (fh + fy);
      fh += fy;
      fy = 0.0f;
   }

   if (fx + fw > fb_width) {
      prescale.scale[0] *= fw / (fb_width - fx);
      prescale.translate[0] -= fx * (fw / (fb_width - fx));
      prescale.translate[0] += fx;
      fw = fb_width - fx;
   }

   if (fy + fh > fb_height) {
      prescale.scale[1] *= fh / (fb_height - fy);
      if (invertY) {
         float in = fb_height - fy;       /* number of vp pixels inside view */
         float out = fy + fh - fb_height; /* number of vp pixels out of view */
         prescale.translate[1] += fy * out / in;
      }
      else {
         prescale.translate[1] -= fy * (fh / (fb_height - fy));
         prescale.translate[1] += fy;
      }
      fh = fb_height - fy;
   }

   if (fw < 0 || fh < 0) {
      fw = fh = fx = fy = 0;
      degenerate = TRUE;
      goto out;
   }

   /* D3D viewport is integer space.  Convert fx,fy,etc. to
    * integers.
    *
    * TODO: adjust pretranslate correct for any subpixel error
    * introduced converting to integers.
    */
   rect.x = (uint32) fx;
   rect.y = (uint32) fy;
   rect.w = (uint32) fw;
   rect.h = (uint32) fh;

   SVGA_DBG(DEBUG_VIEWPORT,
            "viewport error %f,%f %fx%f\n",
            fabs((float)rect.x - fx),
            fabs((float)rect.y - fy),
            fabs((float)rect.w - fw),
            fabs((float)rect.h - fh));

   SVGA_DBG(DEBUG_VIEWPORT,
            "viewport %d,%d %dx%d\n",
            rect.x,
            rect.y,
            rect.w,
            rect.h);

   /* Finally, to get GL rasterization rules, need to tweak the
    * screen-space coordinates slightly relative to D3D which is
    * what hardware implements natively.
    */
   if (svga->curr.rast && svga->curr.rast->templ.half_pixel_center) {
      float adjust_x = 0.0;
      float adjust_y = 0.0;

      if (svga_have_vgpu10(svga)) {
         /* Normally, we don't have to do any sub-pixel coordinate
          * adjustments for VGPU10.  But when we draw wide points with
          * a GS we need an X adjustment in order to be conformant.
          */
         if (svga->curr.reduced_prim == PIPE_PRIM_POINTS &&
             svga->curr.rast->pointsize > 1.0f) {
            adjust_x = 0.5;
         }
      }
      else {
         switch (svga->curr.reduced_prim) {
         case PIPE_PRIM_POINTS:
            adjust_x = -0.375;
            adjust_y = -0.75;
            break;
         case PIPE_PRIM_LINES:
            adjust_x = -0.5;
            adjust_y = -0.125;
            break;
         case PIPE_PRIM_TRIANGLES:
            adjust_x = -0.5;
            adjust_y = -0.5;
            break;
         default:
            /* nothing */
            break;
         }
      }

      if (invertY)
         adjust_y = -adjust_y;

      prescale.translate[0] += adjust_x;
      prescale.translate[1] += adjust_y;
      prescale.translate[2] = 0.5; /* D3D clip space */
      prescale.scale[2]     = 0.5; /* D3D clip space */
   }

   range_min = viewport->scale[2] * -1.0f + viewport->translate[2];
   range_max = viewport->scale[2] *  1.0f + viewport->translate[2];

   /* D3D (and by implication SVGA) doesn't like dealing with zmax
    * less than zmin.  Detect that case, flip the depth range and
    * invert our z-scale factor to achieve the same effect.
    */
   if (range_min > range_max) {
      float range_tmp;
      range_tmp = range_min;
      range_min = range_max;
      range_max = range_tmp;
      prescale.scale[2] = -prescale.scale[2];
   }

   /* If zmin is less than 0, clamp zmin to 0 and adjust the prescale.
    * zmin can be set to -1 when viewport->scale[2] is set to 1 and
    * viewport->translate[2] is set to 0 in the blit code.
    */
   if (range_min < 0.0f) {
      range_min = -0.5f * viewport->scale[2] + 0.5f + viewport->translate[2];
      range_max = 0.5f * viewport->scale[2] + 0.5f + viewport->translate[2];
      prescale.scale[2] *= 2.0f;
      prescale.translate[2] -= 0.5f;
   }

   if (prescale.enabled) {
      float H[2];
      float J[2];
      int i;

      SVGA_DBG(DEBUG_VIEWPORT,
               "prescale %f,%f %fx%f\n",
               prescale.translate[0],
               prescale.translate[1],
               prescale.scale[0],
               prescale.scale[1]);

      H[0] = (float)rect.w / 2.0f;
      H[1] = -(float)rect.h / 2.0f;
      J[0] = (float)rect.x + (float)rect.w / 2.0f;
      J[1] = (float)rect.y + (float)rect.h / 2.0f;

      SVGA_DBG(DEBUG_VIEWPORT,
               "H %f,%f\n"
               "J %fx%f\n",
               H[0],
               H[1],
               J[0],
               J[1]);

      /* Adjust prescale to take into account the fact that it is
       * going to be applied prior to the perspective divide and
       * viewport transformation.
       *
       * Vwin = H(Vc/Vc.w) + J
       *
       * We want to tweak Vwin with scale and translation from above,
       * as in:
       *
       * Vwin' = S Vwin + T
       *
       * But we can only modify the values at Vc.  Plugging all the
       * above together, and rearranging, eventually we get:
       *
       *   Vwin' = H(Vc'/Vc'.w) + J
       * where:
       *   Vc' = SVc + KVc.w
       *   K = (T + (S-1)J) / H
       *
       * Overwrite prescale.translate with values for K:
       */
      for (i = 0; i < 2; i++) {
         prescale.translate[i] = ((prescale.translate[i] +
                                   (prescale.scale[i] - 1.0f) * J[i]) / H[i]);
      }

      SVGA_DBG(DEBUG_VIEWPORT,
               "clipspace %f,%f %fx%f\n",
               prescale.translate[0],
               prescale.translate[1],
               prescale.scale[0],
               prescale.scale[1]);
   }

out:
   if (degenerate) {
      rect.x = 0;
      rect.y = 0;
      rect.w = 1;
      rect.h = 1;
      prescale.enabled = FALSE;
   }

   if (!svga_rects_equal(&rect, &svga->state.hw_clear.viewport)) {
      if (svga_have_vgpu10(svga)) {
         emit_vgpu10_viewport = TRUE;
      }
      else {
         ret = SVGA3D_SetViewport(svga->swc, &rect);
         if (ret != PIPE_OK)
            return ret;

         svga->state.hw_clear.viewport = rect;
      }
   }

   if (svga->state.hw_clear.depthrange.zmin != range_min ||
       svga->state.hw_clear.depthrange.zmax != range_max)
   {
      if (svga_have_vgpu10(svga)) {
         emit_vgpu10_viewport = TRUE;
      }
      else {
         ret = SVGA3D_SetZRange(svga->swc, range_min, range_max );
         if (ret != PIPE_OK)
            return ret;

         svga->state.hw_clear.depthrange.zmin = range_min;
         svga->state.hw_clear.depthrange.zmax = range_max;
      }
   }

   if (emit_vgpu10_viewport) {
      SVGA3dViewport vp;
      vp.x = (float) rect.x;
      vp.y = (float) rect.y;
      vp.width = (float) rect.w;
      vp.height = (float) rect.h;
      vp.minDepth = range_min;
      vp.maxDepth = range_max;
      ret = SVGA3D_vgpu10_SetViewports(svga->swc, 1, &vp);
      if (ret != PIPE_OK)
         return ret;

      svga->state.hw_clear.viewport = rect;

      svga->state.hw_clear.depthrange.zmin = range_min;
      svga->state.hw_clear.depthrange.zmax = range_max;
   }

   if (memcmp(&prescale, &svga->state.hw_clear.prescale, sizeof prescale) != 0) {
      svga->dirty |= SVGA_NEW_PRESCALE;
      svga->state.hw_clear.prescale = prescale;
   }

   return PIPE_OK;
}


struct svga_tracked_state svga_hw_viewport =
{
   "hw viewport state",
   ( SVGA_NEW_FRAME_BUFFER |
     SVGA_NEW_VIEWPORT |
     SVGA_NEW_RAST |
     SVGA_NEW_REDUCED_PRIMITIVE ),
   emit_viewport
};


/***********************************************************************
 * Scissor state
 */
static enum pipe_error
emit_scissor_rect( struct svga_context *svga,
                   unsigned dirty )
{
   const struct pipe_scissor_state *scissor = &svga->curr.scissor;

   if (svga_have_vgpu10(svga)) {
      SVGASignedRect rect;

      rect.left = scissor->minx;
      rect.top = scissor->miny;
      rect.right = scissor->maxx;
      rect.bottom = scissor->maxy;

      return SVGA3D_vgpu10_SetScissorRects(svga->swc, 1, &rect);
   }
   else {
      SVGA3dRect rect;

      rect.x = scissor->minx;
      rect.y = scissor->miny;
      rect.w = scissor->maxx - scissor->minx; /* + 1 ?? */
      rect.h = scissor->maxy - scissor->miny; /* + 1 ?? */

      return SVGA3D_SetScissorRect(svga->swc, &rect);
   }
}


struct svga_tracked_state svga_hw_scissor =
{
   "hw scissor state",
   SVGA_NEW_SCISSOR,
   emit_scissor_rect
};


/***********************************************************************
 * Userclip state
 */

static enum pipe_error
emit_clip_planes( struct svga_context *svga,
                  unsigned dirty )
{
   unsigned i;
   enum pipe_error ret;

   /* TODO: just emit directly from svga_set_clip_state()?
    */
   for (i = 0; i < SVGA3D_MAX_CLIP_PLANES; i++) {
      /* need to express the plane in D3D-style coordinate space.
       * GL coords get converted to D3D coords with the matrix:
       * [ 1  0  0  0 ]
       * [ 0 -1  0  0 ]
       * [ 0  0  2  0 ]
       * [ 0  0 -1  1 ]
       * Apply that matrix to our plane equation, and invert Y.
       */
      float a = svga->curr.clip.ucp[i][0];
      float b = svga->curr.clip.ucp[i][1];
      float c = svga->curr.clip.ucp[i][2];
      float d = svga->curr.clip.ucp[i][3];
      float plane[4];

      plane[0] = a;
      plane[1] = b;
      plane[2] = 2.0f * c;
      plane[3] = d - c;

      if (svga_have_vgpu10(svga)) {
         //debug_printf("XXX emit DX10 clip plane\n");
         ret = PIPE_OK;
      }
      else {
         ret = SVGA3D_SetClipPlane(svga->swc, i, plane);
         if (ret != PIPE_OK)
            return ret;
      }
   }

   return PIPE_OK;
}


struct svga_tracked_state svga_hw_clip_planes =
{
   "hw viewport state",
   SVGA_NEW_CLIP,
   emit_clip_planes
};
