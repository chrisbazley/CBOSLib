/*
 * CBOSLib: Veneer for SWI OS_SpriteOp 34
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
  CJB: 26-Jun-22: Created this source file.
  CJB: 07-May-25: Dogfooding the _Optional qualifier.
*/

/* Acorn C/C++ library headers */
#include "kernel.h"
#include "swis.h"

/* Local headers */
#include "OSSpriteOp.h"
#include "Internal/CBOSMisc.h"

_Optional _kernel_oserror *os_sprite_op_plot_sprite(
  SpriteAreaHeader *const area, const char *const name,
  int const x, int const y, int const action)
{
  assert(area != NULL);
  assert(name != NULL);
  DEBUGF(
         "SprPlot: Plotting sprite '%s' in area %p at %d,%d with "
         "action %d\n",
         name, (void *)area, x, y, action);

  _kernel_swi_regs regs = {.r = {
     SPRITEOP_USERAREA_SPRNAME + SPRITEOP_PLOTCOORDS,
     (int)area,
     (int)name,
     x,
     y,
     action,
  }};

  return _kernel_swi(OS_SpriteOp, &regs, &regs);
}
