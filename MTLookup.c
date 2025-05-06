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
  CJB: 06-Oct-09: Created this source file.
  CJB: 05-May-12: Added support for stress-testing failure of _kernel_swi.
  CJB: 18-Apr-15: Assertions are now provided by debug.h.
  CJB: 21-Apr-16: Modified format strings to avoid GNU C compiler warnings.
  CJB: 16-Mar-19: Reduced the scope of buff_req and other variables.
  CJB: 25-Aug-20: Fixed null pointers instead of strings passed to DEBUGF.
  CJB: 20-Sep-20: messagetrans_lookup now outputs the number of bytes
                  required to store the full output (as documented) instead
                  of 'size of result before terminator' (like the SWI).
  CJB: 29-Aug-22: Use size_t rather than unsigned int for nparam.
  CJB: 07-May-25: Dogfooding the _Optional qualifier.
*/

/* ISO library headers */
#include <stddef.h>
#include <stdarg.h>

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

_Optional _kernel_oserror *messagetrans_lookup(_Optional MessagesFD   *mfd,
                                               const char             *token,
                                               _Optional char         *buffer,
                                               size_t                  buff_size,
                                               _Optional size_t       *nbytes,
                                               size_t                  nparam,
                                               ...)
{
  _Optional _kernel_oserror *e;
  va_list ap;

  va_start(ap, nparam); /* make ap point to first unnamed arg */
  e = messagetrans_vlookup(mfd, token, buffer, buff_size, nbytes, nparam, ap);
  va_end(ap);

  return e;
}

/* ----------------------------------------------------------------------- */

_Optional _kernel_oserror *messagetrans_vlookup(_Optional MessagesFD   *mfd,
                                                const char              *token,
                                                _Optional char          *buffer,
                                                size_t                   buff_size,
                                                _Optional size_t        *nbytes,
                                                size_t                   nparam,
                                                va_list                  params)
{
  _Optional _kernel_oserror *e = NULL;

  assert(token != NULL);
  assert(nparam <= MaxParameters);

  if (!buffer)
  {
    buff_size = 0;
  }

  DEBUGF("MTLookup: looking up token '%s' in message file %p "
         "with %zu parameters\n", token, (void *)mfd, nparam);

  DEBUGF("MTLookup: output buffer %p of size %zu\n", (void *)buffer,
         buff_size);

  _kernel_swi_regs regs = {
    .r = {
      mfd ? (int)mfd : 0
    }
  };

  /* Decode optional arguments (parameters to be substituted into message) */
  for (size_t p = 0; p < MaxParameters; p++)
  {
    if (nparam > p)
    {
      char *const param = va_arg(params, char *);
      DEBUGF("MTLookup: parameter %zu is '%s'\n", p, param ? param : "");
      regs.r[4 + p] = param ? (int)param : 0;
    }
  }

  if (nbytes != NULL)
  {
    /* Find the buffer size required for the output */
    char dummy[1];

    regs.r[1] = (int)token;
    regs.r[2] = (int)dummy; /* in case buffer == NULL */
    regs.r[3] = 0; /* size of buffer */
    e = _kernel_swi(MessageTrans_Lookup, &regs, &regs);
    if (e == NULL)
    {
      assert(regs.r[3] >= 0);
      size_t const buff_req = regs.r[3];

      /* Output the required buffer size (including the terminator) */
      *nbytes =  buff_req + 1;
      DEBUGF("MTLookup: Outputting buffer requirement %zu\n", buff_req);
    }
    else
    {
      DEBUGF("MTLookup: SWI error 0x%x '%s' (1)\n", e->errnum, e->errmess);
    }
  }

  /* The SWI doesn't return an error for buffer overflow and I decided this
     function shouldn't either because the buffer contents are not undefined,
     it avoids two SWI calls in the most common case and matches the
     behaviour of Toolbox methods. */
  if (e == NULL && buff_size > 0)
  {
    /* If buffer == NULL then the SWI does no parameter substitution and
       returns a pointer to a read-only string with no NUL terminator. Not
       very useful for a C program. */
    assert(buffer != NULL);

    /* Generate the output string.
       R0 and R4-R7 should have been preserved by the earlier SWI. */
    regs.r[1] = (int)token;
    regs.r[2] = buffer ? (int)buffer : 0;
    regs.r[3] = buff_size;
    e = _kernel_swi(MessageTrans_Lookup, &regs, &regs);
    if (e == NULL)
    {
      DEBUGF("MTLookup: Result is '%s'\n", buffer);
    }
    else
    {
      DEBUGF("MTLookup: SWI error 0x%x '%s' (2)\n", e->errnum, e->errmess);
    }
  }

  return e;
}
