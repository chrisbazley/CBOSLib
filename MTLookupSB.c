/*
 * CBOSLib: Look up messages into a string buffer
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
  CJB: 09-Nov-26: Added string-buffer MessageTrans lookup veneers.
*/

#include <stddef.h>
#include <stdarg.h>

#include "kernel.h"

/* Local headers */
#include "MessTrans.h"
#include "Internal/CBOSMisc.h"

enum { MaxParameters = 4, DUMMY_ERRNO = 255 };

_Optional _kernel_oserror *messagetrans_lookup_sb(_Optional MessagesFD *mfd,
  const char *token, strb_t *output, size_t nparam, ...)
{
  va_list params;
  va_start(params, nparam);
  _Optional _kernel_oserror *const e =
    messagetrans_vlookup_sb(mfd, token, output, nparam, params);
  va_end(params);
  return e;
}

_Optional _kernel_oserror *messagetrans_vlookup_sb(_Optional MessagesFD *mfd,
  const char *token, strb_t *output, size_t nparam, va_list params)
{
  assert(token != NULL);
  assert(output != NULL);
  assert(nparam <= MaxParameters);
  va_list query;
  va_copy(query, params);
  size_t size;
  _Optional _kernel_oserror *e =
    messagetrans_vlookup(mfd, token, NULL, 0, &size, nparam, query);
  va_end(query);
  if (e == NULL)
  {
    assert(size > 0);
    _Optional char *const buffer = strb_write(output, size - 1);
    if (buffer)
    {
      e = messagetrans_vlookup(mfd, token, &*buffer, size, NULL, nparam, params);
      strb_restore(output);
    }
    else
    {
      e = messagetrans_error_lookup(NULL, DUMMY_ERRNO, "BufOFlo", 0);
    }
  }
  return e;
}
