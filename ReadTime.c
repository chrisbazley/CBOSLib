/*
 * CBOSLib: Veneer for the OS_ReadMonotonicTime SWI
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
  CJB: 14-Mar-19: Created this source file.
 */

/* Acorn C/C++ library headers */
#include "kernel.h"
#include "swis.h"

/* Local headers */
#include "Internal/CBOSMisc.h"
#include "OSReadTime.h"

_kernel_oserror *os_read_monotonic_time(int *const time)
{
  assert(time != NULL);

  _kernel_swi_regs regs;
  _kernel_oserror * const e = _kernel_swi(OS_ReadMonotonicTime,
    &regs, &regs);

  if (e != NULL)
  {
    DEBUGF("ReadTime: SWI returned error 0x%x '%s'\n",
           e->errnum, e->errmess);
  }
  else
  {
    *time = regs.r[0];
    DEBUGF("ReadTime: current timer value is %d\n", *time);
  }
  return e;
}
