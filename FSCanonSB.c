/*
 * CBOSLib: Write a canonical file path into a string buffer
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
  CJB: 09-Nov-26: Added the string-buffer canonicalisation veneer.
*/

#include <stddef.h>

/* Acorn C/C++ library headers */
#include "kernel.h"

/* CBOSLib headers */
#include "MessTrans.h"
#include "OSFSCntrl.h"

/* Local headers */
#include "Internal/CBOSMisc.h"

enum { DUMMY_ERRNO = 255 };

_Optional _kernel_oserror *os_fscontrol_canonicalise_sb(strb_t *const buffer,
  _Optional const char *pv, _Optional const char *ps, const char *f)
{
  assert(buffer != NULL);
  assert(f != NULL);

  size_t nbytes;
  _Optional _kernel_oserror *e =
    os_fscontrol_canonicalise(NULL, 0, pv, ps, f, &nbytes);
  if (e == NULL)
  {
    assert(nbytes > 0);
    size_t const old_pos = strb_tell(buffer);
    _Optional char *const output = strb_write(buffer, nbytes - 1);
    if (output == NULL)
    {
      e = messagetrans_error_lookup(NULL, DUMMY_ERRNO, "BufOFlo", 0);
    }
    else
    {
      size_t actual;
      e = os_fscontrol_canonicalise(&*output, nbytes, pv, ps, f, &actual);
      strb_restore(buffer);
      if (e == NULL)
      {
        assert(actual == nbytes);
      }
      else
      {
        strb_delto(buffer, old_pos);
      }
    }
  }
  return e;
}

