/*
 * CBOSLib: Veneer for SWI OS_ReadModeVariable
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
#include <stdbool.h>

/* Acorn C/C++ library headers */
#include "kernel.h"
#include "swis.h"

/* Local headers */
#include "Internal/CBOSMisc.h"
#include "OSVDU.h"

/* ----------------------------------------------------------------------- */
/*                         Public functions                                */

_kernel_oserror *os_read_mode_variable(int      mode,
                                             ModeVar  var,
                                             int     *value,
                                             bool    *valid)
{
  _kernel_oserror *e = NULL;
  int carry;
  _kernel_swi_regs regs;

  regs.r[0] = mode;
  regs.r[1] = var;
  e = _kernel_swi_c(OS_ReadModeVariable, &regs, &regs, &carry);

  if (e == NULL)
  {
    /* SWI sets the carry flag if the variable or mode number were invalid */
    if (carry)
    {
      DEBUGF("ReadModeV: mode 0x%x or variable %d is invalid\n", mode, var);
    }
    else
    {
      DEBUGF("ReadModeV: value of mode 0x%x variable %d is %d (0x%x)\n",
             mode, var, regs.r[2], regs.r[2]);
    }

    if (value != NULL)
      *value = regs.r[2];

    if (valid != NULL)
      *valid = !carry;
  }
  else
  {
    DEBUGF("ReadModeV: SWI returned error 0x%x '%s'\n",
           e->errnum, e->errmess);
  }
  return e;
}
