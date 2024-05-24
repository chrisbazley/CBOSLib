/*
 * CBOSLib: Veneer for SWI OS_SpriteOp 33
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
*/

/* Acorn C/C++ library headers */
#include "kernel.h"
#include "swis.h"

/* Local headers */
#include "OSSpriteOp.h"
#include "Internal/CBOSMisc.h"

_kernel_oserror *os_sprite_op_flip_x(
  SpriteAreaHeader *const area, const char *const name)
{
  assert(area != NULL);
  assert(name != NULL);
  DEBUGF(
         "SprPlot: Flip X of sprite '%s' in area %p\n",
         name, (void *)area);

  _kernel_swi_regs regs = {.r = {
     SPRITEOP_USERAREA_SPRNAME + SPRITEOP_FLIP_X,
     (int)area,
     (int)name,
  }};

  return _kernel_swi(OS_SpriteOp, &regs, &regs);
}
