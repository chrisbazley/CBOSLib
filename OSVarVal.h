/*
 * CBOSLib: Veneers for operating system variables
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

/*
Dependencies: Acorn library kernel.
Message tokens: None.
History:
  CJB: 21-Sep-26: Created this header file.
*/

#ifndef OSVarVal_h
#define OSVarVal_h

/* Acorn C/C++ library headers */
#include "kernel.h"

#if !defined(USE_OPTIONAL) && !defined(_Optional)
#define _Optional
#endif

_Optional _kernel_oserror *os_set_var_val(const char * /*name*/,
  _Optional const char * /*value*/);
   /*
    * Sets the global string variable named 'name' to 'value'. If 'value' is a
    * null pointer then the variable is deleted instead.
    * Returns: a pointer to an OS error block, or else NULL for success.
    */

#endif
