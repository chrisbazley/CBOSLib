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

/* Hourglass.h defines three macros that give access to the facilities provided
   by the RISC OS Hourglass module.

Dependencies: Acorn library kernel.
Message tokens: None.
History:
  CJB: 28-Oct-18: Created this source file.
  CJB: 11-Dec-20: Remove redundant uses of the 'extern' keyword.
  CJB: 07-May-25: Dogfooding the _Optional qualifier.
*/

#ifndef Hourglass_h
#define Hourglass_h

/* Acorn C/C++ library headers */
#include "kernel.h"

#if !defined(USE_OPTIONAL) && !defined(_Optional)
#define _Optional
#endif

_Optional _kernel_oserror *hourglass_on(void);
_Optional _kernel_oserror *hourglass_off(void);
_Optional _kernel_oserror *hourglass_percentage(int /*n*/);

#endif
