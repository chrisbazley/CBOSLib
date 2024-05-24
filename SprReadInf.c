/*
 * CBOSLib: Veneer for SWI OS_SpriteOp 40
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
  CJB: 30-Mar-19: Created this source file.
*/

/* Acorn C/C++ library headers */
#include "kernel.h"
#include "swis.h"

/* Local headers */
#include "OSSpriteOp.h"
#include "Internal/CBOSMisc.h"

_kernel_oserror *os_sprite_op_read_sprite_info(SpriteAreaHeader *const area,
  const char *const name, bool *const has_mask,
  int *const width, int *const height, int *const mode)
{
  assert(area != NULL);
  assert(name != NULL);
  DEBUGF("SprReadInf: Reading info on sprite '%s' in area %p\n",
         name, (void *)area);

  _kernel_swi_regs regs;
  regs.r[0] = SPRITEOP_USERAREA_SPRNAME + SPRITEOP_READINFO;
  regs.r[1] = (int)area;
  regs.r[2] = (int)name;

  _kernel_oserror *const e = _kernel_swi(OS_SpriteOp, &regs, &regs);
  if (e == NULL)
  {
    DEBUGF("SprReadInf: size %d,%d for mode %d, %s\n",
           regs.r[3], regs.r[4], regs.r[6],
           regs.r[5] ? "mask" : "no mask");

    if (width)
      *width = regs.r[3];
    if (height)
      *height = regs.r[4];
    if (has_mask)
      *has_mask = regs.r[5];
    if (mode)
      *mode = regs.r[6];
  }
  else
  {
    DEBUGF("SprReadInf: SWI returned error 0x%x '%s'\n",
           e->errnum, e->errmess);
  }
  return e;
}
