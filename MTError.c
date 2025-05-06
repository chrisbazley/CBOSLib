/*
 * CBOSLib: Veneers for SWI MessageTrans_ErrorLookup
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
  CJB: 12-Oct-09: Created this source file.
  CJB: 05-May-12: Added support for stress-testing failure of _kernel_swi.
  CJB: 18-Apr-15: Assertions are now provided by debug.h.
  CJB: 18-Apr-16: Cast pointer parameters to void * to match %p.
  CJB: 28-Oct-20: Fixed null pointer instead of string passed to DEBUGF.
  CJB: 29-Aug-22: Use size_t rather than unsigned int for nparam.
  CJB: 07-May-25: Dogfooding the _Optional qualifier.
*/

/* ISO library headers */
#include <stddef.h>
#include <stdarg.h>
#include <string.h>

/* Acorn C/C++ library headers */
#include "kernel.h"
#include "swis.h"
#include "toolbox.h"

/* Local headers */
#include "Internal/CBOSMisc.h"
#include "MessTrans.h"

enum
{
  MaxParameters = 4
};

/* ----------------------------------------------------------------------- */
/*                         Public functions                                */

_kernel_oserror *messagetrans_error_lookup(
                         _Optional MessagesFD  *mfd,
                         int                    errnum,
                         const char            *token,
                         size_t                 nparam,
                         ...)
{
  _kernel_oserror *e;
  va_list ap;

  va_start(ap, nparam); /* make ap point to first unnamed arg */
  e = messagetrans_error_vlookup(mfd, errnum, token, nparam, ap);
  va_end(ap);

  return e;
}

/* ----------------------------------------------------------------------- */

/* I wrote a more flexible veneer for this SWI which allowed the caller to
   specify an output buffer and find the required size, but found that I
   didn't actually use it! */
_kernel_oserror *messagetrans_error_vlookup(
                         _Optional MessagesFD  *mfd,
                         int                    errnum,
                         const char            *token,
                         size_t                 nparam,
                         va_list                params)
{
  _Optional _kernel_oserror *e = NULL;

  assert(token != NULL);
  assert(nparam <= MaxParameters);

  DEBUGF("MTError: looking up error 0x%x '%s' in message file %p "
         "with %zu parameters\n", errnum, token, (void *)mfd, nparam);

  /* Fill out a temporary block with the error number and message token */
  _kernel_oserror temp = {.errnum = errnum};
  STRCPY_SAFE(temp.errmess, token);

  _kernel_swi_regs regs = {
    .r = {
      (int)&temp,
      mfd ? (int)mfd : 0
    }
  };

  /* Decode optional arguments (parameters to be substituted into message) */
  for (size_t p = 0; p < MaxParameters; p++)
  {
    char *param;

    if (nparam > p)
    {
      param = va_arg(params, char *);
      DEBUGF("MTError: parameter %zu is '%s'\n", p, param ? param : "");
      regs.r[4 + p] = param ? (int)param : 0;
    }
  }

  /* Look up the error message */
  e = _kernel_swi(MessageTrans_ErrorLookup, &regs, &regs);

  assert(e != NULL);
  DEBUGF("MTError: SWI error 0x%x '%s' (2)\n", e->errnum, e->errmess);

  return (_kernel_oserror *)e;
}
