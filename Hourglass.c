/*
 * CBOSLib: Display an indication that a program is busy (and progress)
 * Copyright (C) 2018 Christopher Bazley
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
  CJB: 28-Oct-18: Created this source file.
  CJB: 18-Jun-22: Assert that the value passed to hourglass_percentage()
                  is within a sensible range.
  CJB: 07-May-25: Dogfooding the _Optional qualifier.
*/

/* Acorn C/C++ library headers */
#include "swis.h"
#include "kernel.h"

/* Local headers */
#include "Internal/CBOSMisc.h"
#include "Hourglass.h"

/* ----------------------------------------------------------------------- */
/*                         Public functions                                */

_Optional _kernel_oserror *hourglass_on(void)
{
  DEBUGF("Hourglass on\n");
  return _swix(Hourglass_On, 0);
}

/* ----------------------------------------------------------------------- */

_Optional _kernel_oserror *hourglass_off(void)
{
  DEBUGF("Hourglass off\n");
  return _swix(Hourglass_Off, 0);
}

/* ----------------------------------------------------------------------- */

_Optional _kernel_oserror *hourglass_percentage(const int n)
{
  DEBUGF("Hourglass_Percentage %d\n", n);
  assert(n >= 0);
  assert(n <= 100);
  return _swix(Hourglass_Percentage, _IN(0), n);
}
