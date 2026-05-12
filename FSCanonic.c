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
  CJB: 07-May-25: Dogfooding the _Optional qualifier.
  CJB: 12-May-26: Make mixed-signedness buffer size calculations
                  warning-free and hopefully more robust.
 */

/* ISO library headers */
#include <stdint.h>
#include <stddef.h>
#include <limits.h>

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

_Optional _kernel_oserror *os_fscontrol_canonicalise(_Optional char *buffer, size_t buff_size, _Optional const char *pv, _Optional const char *ps, const char *f, _Optional size_t *nbytes)
{
  _Optional _kernel_oserror *e;
  _kernel_swi_regs regs;

  assert(f != NULL);

  if (!buffer)
  {
    buff_size = 0;
  }
  else if (buff_size > (uintptr_t)INTPTR_MAX)
  {
    buff_size = (uintptr_t)INTPTR_MAX;
  }

  DEBUGF("FSCanonic: about to canonicalise path '%s' with path variable '%s' "
         "or string '%s'\n", f, pv ? pv : "", ps ? ps : "");

  DEBUGF("FSCanonic: output buffer is %p of size %zu\n", (void *)buffer, buff_size);
  regs.r[0] = FSControl_CanonicalisePath;
  regs.r[1] = (intptr_t)f;
  regs.r[2] = buffer ? (intptr_t)buffer : 0;
  regs.r[3] = pv ? (intptr_t)pv : 0;
  regs.r[4] = ps ? (intptr_t)ps : 0;
  regs.r[5] = (intptr_t)buff_size;
  e = _kernel_swi(OS_FSControl, &regs, &regs);

  if (e == NULL && nbytes != NULL)
  {
    if (buffer == NULL)
    {
      /* If no buffer is supplied then the SWI returns minus the length
         of the canonicalised path (not including terminator) */
      assert(regs.r[5] >= -INT_MAX);
      intptr_t const len = -regs.r[5];
      assert(len >= 0);
      assert((uintptr_t)len <= SIZE_MAX);
      *nbytes = (size_t)len;
    }
    else
    {
      /* Otherwise the SWI returns the number of spare bytes in the buffer
         (including terminator) */
      intptr_t const spare = regs.r[5];
      assert(spare >= 0);
      assert((uintptr_t)spare <= buff_size);
      *nbytes = buff_size - (size_t)spare;
    }
    assert(*nbytes < SIZE_MAX);
    *nbytes += 1; // terminator
    DEBUGF("FSCanonic: required buffer size is %zu\n", *nbytes);
  }
  return e;
}
