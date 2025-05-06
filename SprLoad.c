/*
 * CBOSLib: Veneer for SWI OS_SpriteOp 10
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
  CJB: 07-May-25: Dogfooding the _Optional qualifier.
*/

/* Acorn C/C++ library headers */
#include "kernel.h"
#include "swis.h"

/* Local headers */
#include "OSSpriteOp.h"
#include "Internal/CBOSMisc.h"

_Optional _kernel_oserror *os_sprite_op_load(SpriteAreaHeader *const area,
  const char *const file_name)
{
  assert(area != NULL);
  assert(file_name != NULL);
  DEBUGF("SprLoad: Loading sprite area %p from file '%s'\n",
         (void *)area, file_name);

  _kernel_swi_regs regs;
  regs.r[0] = SPRITEOP_USERAREA_SPRNAME + SPRITEOP_LOAD_AREA;
  regs.r[1] = (int)area;
  regs.r[2] = (int)file_name;

  return _kernel_swi(OS_SpriteOp, &regs, &regs);
}
