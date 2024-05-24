/*
 * CBOSLib: Veneer for SWI OS_SetColour
 * Copyright (C) 2009 Christopher Bazley
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
  CJB: 22-Mar-19: Created this source file.
*/

/* Acorn C/C++ library headers */
#include "kernel.h"
#include "swis.h"

/* Local headers */
#include "OSVDU.h"
#include "Internal/CBOSMisc.h"

_kernel_oserror *os_set_colour(unsigned int const flags, int const action,
  int const colour)
{
  DEBUG_VERBOSEF("SetColour: Calling OS_SetColour with 0x%x,0x%x,0x%x\n",
    colour, flags, action);

  _kernel_swi_regs regs;
  regs.r[0] = (flags & ~0xf) | (action & 0xf);
  regs.r[1] = colour;

  return _kernel_swi(OS_SetColour, &regs, &regs);
}
