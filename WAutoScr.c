/*
 * CBOSLib: Automatic scrolling of windows
 * Copyright (C) 2021 Christopher Bazley
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
  CJB: 30-Oct-21: Created this source file.
  CJB: 01-Aug-22: Only assert that a non-null pointer was passed if
                  enabling scrolling.
  CJB: 07-May-25: Dogfooding the _Optional qualifier.
 */

/* Acorn C/C++ library headers */
#include "kernel.h"
#include "wimp.h"
#include "swis.h"

/* Local headers */
#include "Internal/CBOSMisc.h"
#include "WimpExtra.h"

/* ----------------------------------------------------------------------- */
/*                         Public functions                                */

_Optional _kernel_oserror *wimp_auto_scroll(unsigned int const flags,
                                            WimpAutoScrollBlock *const block,
                                            _Optional unsigned int *const status)
{
  if (flags & (Wimp_AutoScroll_Horizontal|Wimp_AutoScroll_Vertical))
  {
    assert(block);
  }

  DEBUGF("WAutoScroll: Calling Wimp_AutoScroll with flags 0x%x\n", flags);

   _kernel_swi_regs regs;
   regs.r[0] = flags;
   regs.r[1] = (int)block;

  _Optional _kernel_oserror *const e = _kernel_swi(Wimp_AutoScroll, &regs, &regs);
  if (e)
  {
    DEBUGF("WAutoScroll: SWI returned error 0x%x '%s'\n", e->errnum, e->errmess);
  }
  else if (status)
  {
    *status = regs.r[0];
    DEBUGF("WAutoScroll: SWI returned status 0x%x\n", *status);
  }

  return e;
}
