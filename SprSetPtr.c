/*
 * CBOSLib: Veneer for SWI OS_SpriteOp 36
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
  CJB: 29-Jul-22: Use initializer on _kernel_swi_regs declaration.
                  Allow 'const' scale factors and colour translation table.
  CJB: 07-May-25: Dogfooding the _Optional qualifier.
*/

/* Acorn C/C++ library headers */
#include "kernel.h"
#include "swis.h"

/* Local headers */
#include "OSSpriteOp.h"
#include "Internal/CBOSMisc.h"

_Optional _kernel_oserror *os_sprite_op_set_pointer(SpriteAreaHeader *const area,
  const char *const name, unsigned int const flags,
  int const x, int const y,
  _Optional ScaleFactors const *const scale, _Optional void const *const colours)
{
  assert(area != NULL);
  assert(name != NULL);
  DEBUGF("SprSetPtr: Setting pointer from '%s' in area %p at %d,%d with "
         "flags %u, %s, %s\n",
         name, (void *)area, x, y, flags, scale ? "scaled" : "unscaled",
         colours ? "translated colours" : "native colours");

  _kernel_swi_regs regs = {.r = {
    SPRITEOP_USERAREA_SPRNAME + SPRITEOP_SETPTRSHAPE,
    (int)area,
    (int)name,
    flags,
    x,
    y,
    scale ? (int)scale : 0,
    colours ? (int)colours : 0,
  }};

  return _kernel_swi(OS_SpriteOp, &regs, &regs);
}
