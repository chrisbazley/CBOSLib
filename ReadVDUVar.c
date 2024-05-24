/*
 * CBOSLib: Veneer for SWI OS_ReadVduVariables
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
  CJB: 18-Apr-15: Assertions are now provided by debug.h.
  CJB: 31-Jan-16: Substituted _kernel_swi for _swix because it's easier to
                  intercept for stress testing.
*/

/* ISO library headers */
#include <stddef.h>

/* Acorn C/C++ library headers */
#include "kernel.h"
#include "swis.h"

/* Local headers */
#include "OSVDU.h"
#include "Internal/CBOSMisc.h"

/* ----------------------------------------------------------------------- */
/*                         Public functions                                */

_kernel_oserror *os_read_vdu_variables(const VDUVar vars[], int values[])
{
  _kernel_oserror *e = NULL;
  _kernel_swi_regs regs;

  assert(vars != NULL);
  assert(values != NULL);

  regs.r[0] = (int)vars;
  regs.r[1] = (int)values;
  e = _kernel_swi(OS_ReadVduVariables, &regs, &regs);
  if (e == NULL)
  {
#ifdef DEBUG_OUTPUT
    unsigned int i;
    for (i = 0; vars[i] != VDUVar_EndOfList; i++)
    {
      DEBUGF("ReadVDUVar: value of VDU variable %d is 0x%x\n",
             vars[i], values[i]);
    }
#endif
  }
  else
  {
    DEBUGF("ReadVDUVar: SWI returned error 0x%x '%s'\n", e->errnum, e->errmess);
  }
  return e;
}
