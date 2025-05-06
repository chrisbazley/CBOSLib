/*
 * CBOSLib: Veneers for DragAnObject SWIs
 * Copyright (C) 2015 Christopher Bazley
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
  CJB: 30-Nov-15: Created this source file.
  CJB: 07-May-25: Dogfooding the _Optional qualifier.
*/

/* ISO library headers */
#include <stddef.h>

/* Acorn C/C++ library headers */
#include "kernel.h"
#include "wimp.h"
#include "swis.h"

/* Local headers */
#include "Internal/CBOSMisc.h"
#include "DragAnObj.h"

/* ----------------------------------------------------------------------- */
/*                         Public functions                                */

_Optional _kernel_oserror *drag_an_object_start(unsigned int flags, int renderer, const int *renderer_args, const BBox *drag_box, const BBox *parent_box)
{
  _Optional _kernel_oserror *e = NULL;
  _kernel_swi_regs regs;

  assert(renderer_args != NULL);
  assert(drag_box != NULL);

  DEBUGF("DragAnObj: Calling DragAnObject_Start with flags 0x%x, renderer 0x%x, "
         "renderer args {0x%x,0x%x,0x%x,0x%x}, drag box {%d,%d,%d,%d}\n",
         flags, renderer,
         renderer_args[0], renderer_args[1], renderer_args[2], renderer_args[3],
         drag_box->xmin, drag_box->ymin, drag_box->xmax, drag_box->ymax);

  if (flags & DragAnObject_BBox_User)
  {
    assert(parent_box);
    DEBUGF("DragAnObj: parent box is {%d,%d,%d,%d}\n",
           parent_box->xmin, parent_box->ymin, parent_box->xmax, parent_box->ymax);
  }

  regs.r[0] = flags;
  regs.r[1] = renderer;
  regs.r[2] = (int)renderer_args;
  regs.r[3] = (int)drag_box;
  regs.r[4] = (int)parent_box;
  e = _kernel_swi(DragAnObject_Start, &regs, &regs);
  if (e != NULL)
  {
    DEBUGF("DragAnObj: SWI error 0x%x '%s'\n", e->errnum, e->errmess);
  }

  return e;
}

_Optional _kernel_oserror *drag_an_object_stop(void)
{
  _Optional _kernel_oserror *e = NULL;
  _kernel_swi_regs regs;

  DEBUGF("DragAnObj: Calling DragAnObject_Stop\n");
  e = _kernel_swi(DragAnObject_Stop, &regs, &regs);
  if (e != NULL)
  {
    DEBUGF("DragAnObj: SWI error 0x%x '%s'\n", e->errnum, e->errmess);
  }

  return e;
}
