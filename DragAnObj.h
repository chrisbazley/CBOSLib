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

/* DragAnObj.h declares functions to deal with dragged objects on RISC OS.

Dependencies: Acorn library kernel.
Message tokens: None.
History:
  CJB: 30-Nov-15: Created this header file.
  CJB: 11-Dec-20: Remove redundant uses of the 'extern' keyword.
  CJB: 07-May-25: Dogfooding the _Optional qualifier.
*/

#ifndef DragAnObj_h
#define DragAnObj_h

/* ISO library headers */
#include <stddef.h>

/* Acorn C/C++ library headers */
#include "kernel.h"
#include "wimp.h"

#if !defined(USE_OPTIONAL) && !defined(_Optional)
#define _Optional
#endif

/* Flags for use with the drag_an_object_start function */
#define DragAnObject_HAlign_Left   (0u << 0)
#define DragAnObject_HAlign_Centre (1u << 0)
#define DragAnObject_HAlign_Right  (2u << 0)
#define DragAnObject_VAlign_Bottom (0u << 2)
#define DragAnObject_VAlign_Centre (1u << 2)
#define DragAnObject_VAlign_Top    (2u << 2)
#define DragAnObject_BBox_Screen   (0u << 4)
#define DragAnObject_BBox_Window   (1u << 4)
#define DragAnObject_BBox_User     (2u << 4)
#define DragAnObject_BBoxPointer   (1u << 6)
#define DragAnObject_DropShadow    (1u << 7)
#define DragAnObject_NoDither      (1u << 8)
#define DragAnObject_RenderAPCS    (1u << 16)
#define DragAnObject_RenderSVC     (1u << 17)

_Optional _kernel_oserror *drag_an_object_start(unsigned int /*flags*/,
                                                int /*renderer*/,
                                                const int */*renderer_args*/,
                                                const BBox */*drag_box*/,
                                                const BBox */*parent_box*/);
   /*
    * Start dragging an object, using the given function or SWI number to
    * render a representation of the object into a masked sprite. 'renderer'
    * is a SWI number to be called to render the object unless flags bit 16
    * is set, in which case it is a function pointer cast to integer type.
    * In either case, 'renderer_args' points to an array of argument values
    * to be passed to the renderer function or SWI handler.
    * If 'renderer' is a function pointer cast to an integer then it is called
    * with the SVC stack and therefore neither it nor any of its callees may
    * call shared C library functions because the C library uses the stack
    * limit register (sl/R10) to get the base address of its static data. That
    * restriction precludes use of assertions and most CBOSLib functionality
    * including DEBUG macros. The only effect of flags bit 17 is to set the
    * stack limit for the 'renderer' function by rounding the stack pointer
    * down to a megabyte boundary instead of zeroing it.
    * 'drag_box' points to the coordinates of the bounding box of the object
    * to be dragged. 'parent_box' is unused unless flags bit 5 is set, in
    * which case it points to the coordinates of a bounding box within which
    * to restrict the drag.
    *
    * Returns: a pointer to an OS error block, or else NULL for success.
    */

_Optional _kernel_oserror *drag_an_object_stop(void);
   /*
    * Stop dragging an object. Expected to be called when a Wimp_EUserDrag
    * event is delivered because the user has released all mouse buttons at
    * the end of a drag.
    * Returns: a pointer to an OS error block, or else NULL for success.
    */

#endif
