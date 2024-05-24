/*
 * CBOSLib: Drag operations within windows
 * Copyright (C) 2021 Christopher Bazley
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
  CJB: 30-Oct-21: Created this source file.
 */

/* Acorn C/C++ library headers */
#include "kernel.h"
#include "wimp.h"
#include "swis.h"

/* Local headers */
#include "Internal/CBOSMisc.h"
#include "WimpExtra.h"

/* Constant numeric values */
enum
{
  MagicWord = 0x4b534154
};

/* ----------------------------------------------------------------------- */
/*                         Public functions                                */

_kernel_oserror *wimp_drag_box2(WimpDragBox *const block,
                               unsigned int const flags)
{
  assert(block);

  DEBUGF("WDragBox2: Calling Wimp_DragBox with wimp_window %d drag_type %d\n"
         "dragging_box {%d,%d,%d,%d}\n"
         "parent_box {%d,%d,%d,%d}\n"
         "flags 0x%x\n",
         block->wimp_window, block->drag_type,
         block->dragging_box.xmin, block->dragging_box.ymin,
         block->dragging_box.xmax, block->dragging_box.ymax,
         block->parent_box.xmin, block->parent_box.ymin,
         block->parent_box.xmax, block->parent_box.ymax,
         flags);

   _kernel_swi_regs regs;
   regs.r[1] = (int)block;
   regs.r[2] = MagicWord;
   regs.r[3] = flags;

  _kernel_oserror *const e = _kernel_swi(Wimp_DragBox, &regs, &regs);
  if (e)
  {
    DEBUGF("WDragBox2: SWI returned error 0x%x '%s'\n", e->errnum, e->errmess);
  }

  return e;
}
