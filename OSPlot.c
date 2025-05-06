/*
 * CBOSLib: Simple veneer for VDU driver's drawing capabilities
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
  CJB: 28-Sep-09: Created this source file.
  CJB: 31-Jan-16: Substituted _kernel_swi for _swix because it's easier to
                  intercept for stress testing.
  CJB: 05-May-19: Only emit debugging output when verbose.
  CJB: 01-Jun-21: Assert that the coordinates passed to os_plot() are within
                  a sensible range.
  CJB: 07-May-25: Dogfooding the _Optional qualifier.
*/

#include <limits.h>

/* Acorn C/C++ library headers */
#include "kernel.h"
#include "swis.h"

/* Local headers */
#include "OSVDU.h"
#include "Internal/CBOSMisc.h"

_Optional _kernel_oserror *os_plot(int command, int x, int y)
{
  /* Well, what did you expect? */
  DEBUG_VERBOSEF("OSPlot: Calling OS_Plot with %d,%d,%d\n", command, x, y);
  assert(x >= SHRT_MIN);
  assert(y >= SHRT_MIN);
  assert(x <= SHRT_MAX);
  assert(y <= SHRT_MAX);

  _kernel_swi_regs regs;
  regs.r[0] = command;
  regs.r[1] = x;
  regs.r[2] = y;

  return _kernel_swi(OS_Plot, &regs, &regs);
}
