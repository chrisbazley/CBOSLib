/*
 * CBOSLib: Veneer for SWI OS_FSControl 26
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
  CJB: 14-Mar-19: Created this source file
 */

/* ISO library headers */
#include <stddef.h>

/* Acorn C/C++ library headers */
#include "kernel.h"
#include "swis.h"

/* Local headers */
#include "Internal/CBOSMisc.h"
#include "OSFSCntrl.h"

/* Constant numeric values */
enum
{
  FSControl_CopyObjects = 26
};

_kernel_oserror *os_fscontrol_copy(const char *src,
  const char *dst, unsigned int flags)
{
  assert(src != NULL);
  assert(dst != NULL);

  DEBUGF("FSCopy: about to copy '%s' to '%s' with flags 0x%x\n",
         src, dst, flags);

  _kernel_swi_regs regs;
  regs.r[0] = FSControl_CopyObjects;
  regs.r[1] = (int)src;
  regs.r[2] = (int)dst;
  regs.r[3] = (int)flags;
  _kernel_oserror *const e = _kernel_swi(OS_FSControl, &regs, &regs);

  if (e != NULL)
  {
    DEBUGF("FSCopy: SWI returned error 0x%x '%s'\n",
           e->errnum, e->errmess);
  }
  return e;
}
