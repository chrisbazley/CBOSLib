/*
 * CBOSLib: Veneer for SWI OS_SpriteOp 52
 * Copyright (C) 2019 Christopher Bazley
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
  CJB: 16-Mar-19: Created this source file.
  CJB: 26-Jun-22: Use initializer on _kernel_swi_regs declaration.
                  Allow 'const' scale factors and colour translation table.
*/

/* Acorn C/C++ library headers */
#include "kernel.h"
#include "swis.h"

/* Local headers */
#include "OSSpriteOp.h"
#include "Internal/CBOSMisc.h"

_kernel_oserror *os_sprite_op_plot_scaled_sprite(
  SpriteAreaHeader *const area, const char *const name,
  int const x, int const y, int const action,
  ScaleFactors const *const scale, void const *const colours)
{
  assert(area != NULL);
  assert(name != NULL);
  DEBUGF(
         "SprPlotScl: Plotting sprite '%s' in area %p at %d,%d with "
         "action %d, %s, %s\n",
         name, (void *)area, x, y, action, scale ? "scaled" : "unscaled",
         colours ? "translated colours" : "native colours");

  if (scale) {
    DEBUGF("SprPlotScl: Scale factors are x %d/%d y %d/%d\n",
           scale->xmul, scale->xdiv, scale->ymul, scale->ydiv);
  }

  _kernel_swi_regs regs = {.r = {
     SPRITEOP_USERAREA_SPRNAME + SPRITEOP_PLOTSCALED,
     (int)area,
     (int)name,
     x,
     y,
     action,
     scale ? (int)scale : 0,
     colours ? (int)colours : 0,
  }};

  return _kernel_swi(OS_SpriteOp, &regs, &regs);
}
