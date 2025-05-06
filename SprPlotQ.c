/*
 * CBOSLib: Veneer for SWI OS_SpriteOp 56
 * Copyright (C) 2022 Christopher Bazley
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/* History:
  CJB: 25-Jun-22: Created this source file.
  CJB: 07-May-25: Dogfooding the _Optional qualifier.
*/

/* Acorn C/C++ library headers */
#include "kernel.h"
#include "swis.h"

/* Local headers */
#include "OSSpriteOp.h"
#include "Internal/CBOSMisc.h"

_Optional _kernel_oserror *os_sprite_op_plot_trans_quad_sprite(
  SpriteAreaHeader *const area, const char *const name,
  _Optional BBox const *const src, int const action,
  TransformQuad const *const quad, _Optional void const *const colours)
{
  assert(area != NULL);
  assert(name != NULL);
  assert(quad != NULL);
  DEBUGF(
         "SprPlotScl: Plotting transformed sprite '%s' in area %p at "
         "(%d,%d),(%d,%d),(%d,%d),(%d,%d) with "
         "action %d, %s, %s\n",
         name, (void *)area, quad->coords[0].x, quad->coords[0].y,
         quad->coords[1].x, quad->coords[1].y,
         quad->coords[2].x, quad->coords[2].y,
         quad->coords[3].x, quad->coords[3].y,
         action, src ? "cropped" : "whole",
         colours ? "translated colours" : "native colours");

  if (src) {
    DEBUGF("SprPlotScl: Source rectangle is (%d,%d),(%d,%d)\n",
           src->xmin, src->ymin, src->xmax, src->ymax);
  }

  int const flags = SPRITEOP_PLOT_TRANSFORMED_DST_QUAD |
                    (src ? SPRITEOP_PLOT_TRANSFORMED_SRC_RECT : 0);

  _kernel_swi_regs regs = {.r = {
     SPRITEOP_USERAREA_SPRNAME + SPRITEOP_PLOT_TRANSFORMED_SPRITE,
     (int)area,
     (int)name,
     flags,
     src ? (int)src : 0,
     action,
     (int)quad,
     colours ? (int)colours : 0
  }};

  return _kernel_swi(OS_SpriteOp, &regs, &regs);
}
