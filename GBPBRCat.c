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
 */

/* ISO library headers */
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

_kernel_oserror *os_gbpb_read_cat_no_path(const char *f, void *buffer, size_t buff_size, unsigned int *n, int *pos, const char *pattern)
{
  _kernel_oserror *e;
  _kernel_osgbpb_block gbpb_params;

  assert(f != NULL);
  assert(buffer != NULL || buff_size == 0);
  assert(n != NULL);
  assert(pos != NULL);

  DEBUGF("GBPBRCat: about to read %u catalogue entries for directory '%s' "
         "at position %d with pattern '%s'\n", *n, f, *pos, pattern ? pattern : "");

  DEBUGF("GBPBRCat: output buffer is %p of size %zu\n", buffer, buff_size);

  gbpb_params.dataptr = buffer;
  gbpb_params.nbytes = *n;
  gbpb_params.fileptr = *pos;
  gbpb_params.buf_len = buff_size;
  gbpb_params.wild_fld = (char *)pattern; /* may be null */

  /* Disgusting type-cast from string pointer to integer (thanks, Acorn).
     Note that _kernel_osgbpb even updates 'gbpb_params' on error. */
  if (_kernel_osgbpb(OS_GBPB_ReadEntriesAndFileInfoFromDir,
                     (unsigned int)f,
                     &gbpb_params) == _kernel_ERROR) {
    e = _kernel_last_oserror();
    assert(e != NULL);
    DEBUGF("GBPBRCat: _kernel_osgbpb set error %d:%s\n",
           e->errnum, e->errmess);
  } else {
    DEBUGF("GBPBRCat: %d entries were read and new offset is %d\n",
           gbpb_params.nbytes, gbpb_params.fileptr);
    *pos = gbpb_params.fileptr;
    *n = gbpb_params.nbytes;
    e = NULL;
  }
  return e;
}
