/*
 * CBOSLib: Veneer for SWI OS_SpriteOp 24
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
  CJB: 21-Mar-19: Created this source file.
  CJB: 31-Mar-19: Corrected os_sprite_op_select output type.
*/

/* Acorn C/C++ library headers */
#include "kernel.h"
#include "swis.h"

/* Local headers */
#include "OSSpriteOp.h"
#include "Internal/CBOSMisc.h"

_kernel_oserror *os_sprite_op_select(SpriteAreaHeader *const area,
  const char *const name, SpriteHeader **const address)
{
  assert(area != NULL);
  assert(name != NULL);
  DEBUGF("SprSelect: Selecting sprite '%s' in area %p\n", name,
         (void *)area);

  _kernel_swi_regs regs;
  regs.r[0] = SPRITEOP_USERAREA_SPRNAME + SPRITEOP_SELECT_SPRITE;
  regs.r[1] = (int)area;
  regs.r[2] = (int)name;

  _kernel_oserror *const e = _kernel_swi(OS_SpriteOp, &regs, &regs);
  if (e == NULL && area) {
    *address = (SpriteHeader *)regs.r[2];
  } else {
    *address = NULL;
  }

  return e;
}
