/*
 * CBOSLib: Veneer for SWI OS_SetVarVal
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
#include <stdint.h>
#include <stddef.h>
#include <string.h>

/* Acorn C/C++ library headers */
#include "kernel.h"
#include "swis.h"

/* Local headers */
#include "Internal/CBOSMisc.h"
#include "OSVarVal.h"

enum
{
  OS_SetVarVal_FirstCall = 0,
  OS_SetVarVal_String = 0
};

_Optional _kernel_oserror *os_set_var_val(const char *name,
  _Optional const char *value)
{
  assert(name != NULL);

  DEBUGF("SetVarVal: about to %s variable '%s'\n",
         value ? "set" : "delete", name);

  intptr_t length = -1;
  if (value != NULL)
  {
    size_t const size = strlen(value);
    assert(size <= INTPTR_MAX);
    length = (intptr_t)size;
  }

  _kernel_swi_regs regs = {
    .r = {
      (intptr_t)(void *)name,
      value ? (intptr_t)(void *)value : 0,
      length,
      OS_SetVarVal_FirstCall,
      OS_SetVarVal_String,
    }
  };
  _Optional _kernel_oserror *const e = _kernel_swi(OS_SetVarVal, &regs, &regs);

  if (e != NULL)
  {
    DEBUGF("SetVarVal: SWI returned error 0x%x '%s'\n",
           e->errnum, e->errmess);
  }
  return e;
}
