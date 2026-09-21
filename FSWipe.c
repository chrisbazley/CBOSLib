/*
 * CBOSLib: Veneer for SWI OS_FSControl 27
 * Copyright (C) 2026 Christopher Bazley
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
  CJB: 21-Sep-26: Created this source file.
*/

/* ISO library headers */
#include <stddef.h>
#include <stdint.h>

/* Acorn C/C++ library headers */
#include "kernel.h"
#include "swis.h"

/* Local headers */
#include "Internal/CBOSMisc.h"
#include "OSFSCntrl.h"

/* Constant numeric values */
enum
{
  FSControl_WipeObjects = 27
};

_Optional _kernel_oserror *os_fscontrol_wipe(const char *path,
  unsigned int flags)
{
  assert(path != NULL);

  DEBUGF("FSWipe: about to wipe '%s' with flags 0x%x\n", path, flags);

  _kernel_swi_regs regs = {
    .r = {
      FSControl_WipeObjects,
      (intptr_t)(void *)path,
      0,
      (intptr_t)flags,
    }
  };
  _Optional _kernel_oserror *const e = _kernel_swi(OS_FSControl, &regs, &regs);

  if (e != NULL)
  {
    DEBUGF("FSWipe: SWI returned error 0x%x '%s'\n",
           e->errnum, e->errmess);
  }
  return e;
}
