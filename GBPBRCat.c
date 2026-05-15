/*
 * CBOSLib: Veneer for SWI OS_GBPB 10
 * Copyright (C) 2014 Christopher Bazley
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
  CJB: 23-Nov-14: Created this source file
  CJB: 18-Apr-15: Assertions are now provided by debug.h.
  CJB: 21-Apr-16: Modified format strings to avoid GNU C compiler warnings.
  CJB: 07-May-25: Dogfooding the _Optional qualifier.
  CJB: 11-May-26: Use PRIdPTR to print results from _kernel_osgbpb.
  CJB: 14-May-26: Limit buffer size and explicitly convert it from size_t to
                  intptr_t.
  CJB: 15-May-26: Allow the buffer argument to be null. Use size_t for the
                  number of objects for which to read catalogue info.
                  More explicit conversions checked by assertions.
 */

/* ISO library headers */
#include <inttypes.h>
#include <limits.h>
#include <stddef.h>

/* Acorn C/C++ library headers */
#include "kernel.h"
#include "swis.h"

/* Local headers */
#include "Internal/CBOSMisc.h"
#include "OSGBPB.h"

/* Constant numeric values */
enum
{
  OS_GBPB_ReadEntriesAndFileInfoFromDir = 10
};

/* ----------------------------------------------------------------------- */
/*                         Public functions                                */

_Optional _kernel_oserror *
os_gbpb_read_cat_no_path(const char *f, _Optional void *buffer,
                         size_t buff_size, size_t *n, int *pos,
                         _Optional const char *pattern)
{
  _Optional _kernel_oserror *e;

  assert(f != NULL);
  assert(n != NULL);
  assert(pos != NULL);

  if (buffer == NULL)
  {
    buff_size = 0;
  }
  else if (buff_size > (uintptr_t)INTPTR_MAX)
  {
    buff_size = (uintptr_t)INTPTR_MAX;
  }

  DEBUGF("GBPBRCat: about to read %zu catalogue entries for directory '%s' "
         "at position %d with pattern '%s'\n",
         *n, f, *pos, pattern ? pattern : "");

  DEBUGF("GBPBRCat: output buffer is %p of size %zu\n", buffer, buff_size);

  assert(*n < INTPTR_MAX);
  assert(buff_size <= (uintptr_t)INTPTR_MAX);

  _kernel_osgbpb_block gbpb_params = {
    .dataptr = buffer,
    .nbytes = (intptr_t)*n,
    .fileptr = *pos,
    .buf_len = (intptr_t)buff_size,
    .wild_fld = pattern ? (char *)pattern : 0, /* may be null */
  };

  /* Disgusting type-cast from string pointer to integer (thanks, Acorn).
     Note that _kernel_osgbpb even updates 'gbpb_params' on error. */
  if (_kernel_osgbpb(OS_GBPB_ReadEntriesAndFileInfoFromDir, (uintptr_t)f,
                     &gbpb_params) == _kernel_ERROR)
  {
    e = _kernel_last_oserror();
    assert(e != NULL);
    DEBUGF("GBPBRCat: _kernel_osgbpb set error %d:%s\n", e->errnum, e->errmess);
  }
  else
  {
    DEBUGF("GBPBRCat: %" PRIdPTR
           " entries were read and new offset is %" PRIdPTR "\n",
           gbpb_params.nbytes, gbpb_params.fileptr);

    assert(gbpb_params.fileptr >= INT_MIN);
    assert(gbpb_params.fileptr <= INT_MAX);
    *pos = (int)gbpb_params.fileptr;

    assert(gbpb_params.nbytes >= 0);
    assert((uintptr_t)gbpb_params.nbytes <= SIZE_MAX);
    *n = (size_t)gbpb_params.nbytes;

    e = NULL;
  }
  return e;
}
