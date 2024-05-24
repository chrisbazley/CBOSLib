/*
 * CBOSLib: Veneer for SWI OS_FSControl 37
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
  CJB: 10-Oct-09: Created this source file
  CJB: 05-May-12: Added support for stress-testing failure of _kernel_swi.
  CJB: 18-Apr-15: Assertions are now provided by debug.h.
  CJB: 21-Apr-16: Modified format strings to avoid GNU C compiler warnings.
  CJB: 25-Aug-20: Fixed null pointers instead of strings passed to DEBUGF.
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
  FSControl_CanonicalisePath = 37
};

/* ----------------------------------------------------------------------- */
/*                         Public functions                                */

_kernel_oserror *os_fscontrol_canonicalise(char *buffer, size_t buff_size, const char *pv, const char *ps, const char *f, size_t *nbytes)
{
  _kernel_oserror *e;
  _kernel_swi_regs regs;

  assert(buffer != NULL || buff_size == 0);
  assert(f != NULL);

  DEBUGF("FSCanonic: about to canonicalise path '%s' with path variable '%s' "
         "or string '%s'\n", f, pv ? pv : "", ps ? ps : "");

  DEBUGF("FSCanonic: output buffer is %p of size %zu\n", buffer, buff_size);
  regs.r[0] = FSControl_CanonicalisePath;
  regs.r[1] = (int)f;
  regs.r[2] = (int)buffer;
  regs.r[3] = pv ? (int)pv : 0;
  regs.r[4] = ps ? (int)ps : 0;
  regs.r[5] = buff_size;
  e = _kernel_swi(OS_FSControl, &regs, &regs);

  if (e == NULL && nbytes != NULL)
  {
    if (buffer == NULL)
    {
      /* If no buffer is supplied then the SWI returns minus the length
         of the canonicalised path (not including terminator) */
      assert(regs.r[5] <= 0);
      *nbytes = 1 - regs.r[5];
    }
    else
    {
      /* Otherwise the SWI returns the number of spare bytes in the buffer
         (including terminator) */
      *nbytes = buff_size - regs.r[5] + 1;
    }
    DEBUGF("FSCanonic: required buffer size is %zu\n", *nbytes);
  }
  return e;
}
