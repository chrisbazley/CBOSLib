/*
 * CBOSLib: Set the mouse pointer bounding box
 * Copyright (C) 2019 Christopher Bazley
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
  CJB: 07-May-25: Dogfooding the _Optional qualifier.
 */

/* ISO library headers */
#include <stddef.h>
#include <limits.h>

/* Acorn C/C++ library headers */
#include "kernel.h"
#include "wimp.h"

/* Local headers */
#include "Internal/CBOSMisc.h"
#include "OSWord.h"

/* Constant numeric values */
enum
{
  OS_Word_PointerOp = 21, /* _kernel_osword reason code */
  SetPointerBBox = 1      /* sub-reason code in parameter block */
};

/* ----------------------------------------------------------------------- */
/*                         Public functions                                */

_Optional _kernel_oserror *os_word_set_pointer_bbox(const BBox *const bbox)
{
  _Optional _kernel_oserror *e = NULL;

  assert(bbox != NULL);
  assert(bbox->xmin <= bbox->xmax);
  assert(bbox->ymin <= bbox->ymax);

  unsigned char bounding_box[9] = {
    SetPointerBBox,
    bbox->xmin,
    bbox->xmin >> CHAR_BIT,
    bbox->ymin,
    bbox->ymin >> CHAR_BIT,
    bbox->xmax,
    bbox->xmax >> CHAR_BIT,
    bbox->ymax,
    bbox->ymax >> CHAR_BIT
  };

  if (_kernel_osword(OS_Word_PointerOp, (int *)bounding_box) == _kernel_ERROR)
  {
    e = _kernel_last_oserror();
    assert(e != NULL);
    DEBUGF("SetPtrBBox: SWI returned error 0x%x '%s'\n", e->errnum, e->errmess);
  }

  return e;
}
