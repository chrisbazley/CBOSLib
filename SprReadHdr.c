/*
 * CBOSLib: Veneer for SWI OS_SpriteOp 8
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
*/

/* Acorn C/C++ library headers */
#include "kernel.h"
#include "swis.h"

/* Local headers */
#include "OSSpriteOp.h"
#include "Internal/CBOSMisc.h"

_kernel_oserror *os_sprite_op_read_header(SpriteAreaHeader *const area,
  SpriteAreaHeader *const hdr)
{
  assert(area != NULL);
  assert(hdr != NULL);
  DEBUGF("SprReadHdr: Reading header of sprite area %p to %p\n",
         (void *)area, (void *)hdr);

  _kernel_swi_regs regs;
  regs.r[0] = SPRITEOP_USERAREA_SPRNAME + SPRITEOP_READCTRLBLOCK;
  regs.r[1] = (int)area;

  _kernel_oserror *const e = _kernel_swi(OS_SpriteOp, &regs, &regs);
  if (e == NULL)
  {
    hdr->size = regs.r[2];
    hdr->sprite_count = regs.r[3];
    hdr->first = regs.r[4];
    hdr->used = regs.r[5];

    DEBUGF("SprReadHdr: size %d, count %d, first %d, used %d\n",
           hdr->size, hdr->sprite_count, hdr->first, hdr->used);
  }
  else
  {
    DEBUGF("SprReadHdr: SWI returned error 0x%x '%s'\n",
           e->errnum, e->errmess);
  }
  return e;
}
