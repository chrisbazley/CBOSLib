/*
 * CBOSLib: Veneer for SWI ColourTrans_SetGCOL
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
  CJB: 02-Oct-09: Created this source file.
  CJB: 31-Jan-16: Substituted _kernel_swi for _swix because it's easier to
                  intercept for stress testing.
  CJB: 07-May-25: Dogfooding the _Optional qualifier.
*/

/* ISO library headers */
#include <stddef.h>

/* Acorn C/C++ library headers */
#include "kernel.h"
#include "swis.h"

/* Local headers */
#include "Internal/CBOSMisc.h"
#include "ClrTrans.h"
#include "PalEntry.h"

/* ----------------------------------------------------------------------- */
/*                         Public functions                                */

_Optional _kernel_oserror *colourtrans_set_gcol(unsigned int flags,
                                                int          action,
                                                PaletteEntry colour)
{
  DEBUGF("ClrTrans: Calling ColourTrans_SetGCOL with 0x%x,0x%x,0x%x\n",
         colour, flags, action);

  _kernel_swi_regs regs;
  regs.r[0] = colour;
  regs.r[3] = flags;
  regs.r[4] = action;

  return _kernel_swi(ColourTrans_SetGCOL, &regs, &regs);
}
