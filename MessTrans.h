/*
 * CBOSLib: Veneers for message translation SWIs
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

/* MessageTrans.h declares several functions which look up messages from tokens
   and optionally perform parameter subsitution.

Dependencies: Acorn library kernel.
Message tokens: None.
History:
  CJB: 06-Oct-09: Created this header file.
  CJB: 21-Oct-09: Added prototypes of new functions messagetrans_file_info,
                  messagetrans_open_file and messagetrans_close_file.
  CJB: 11-Dec-20: Remove redundant uses of the 'extern' keyword.
  CJB: 29-Aug-22: Use size_t rather than unsigned int for nparam.
  CJB: 07-May-25: Dogfooding the _Optional qualifier.
*/

#ifndef MessTrans_h
#define MessTrans_h

/* ISO library headers */
#include <stddef.h>
#include <stdarg.h>

/* Acorn C/C++ library headers */
#include "kernel.h"
#include "toolbox.h"

#if !defined(USE_OPTIONAL) && !defined(_Optional)
#define _Optional
#endif

/* Flags output by the messagetrans_file_info function */
#define MessageTrans_FileInfo_HeldInMemory (1u << 0)

_Optional _kernel_oserror *messagetrans_file_info(const char   */*filename*/,
                                        unsigned int */*flags*/,
                                        size_t       */*buff_size*/);
   /*
    * Gives information about the messages file specified by 'filename'. Unless
    * 'flags' is a null pointer, a flags value will be written to the integer
    * that it points to. Flags bit 0 indicates that the file is held in memory.
    * Unless 'buff_size' is a null pointer, the integer to which it points will
    * be set to the size of buffer (in bytes) that would be required to hold
    * the file data.
    * Returns: a pointer to an OS error block, or else NULL for success.
    */

_Optional _kernel_oserror *messagetrans_open_file(MessagesFD      */*mfd*/,
                                                  const char      */*filename*/,
                                                  _Optional void  */*buffer*/);
   /*
    * Opens the messages file specified by 'filename' for subsequent use by
    * messagetrans_lookup and similar functions. The messages file descriptor
    * pointed to by 'mfd' will be initialised and the file loaded into the
    * specified buffer (which must be large enough). If 'buffer' is a null
    * pointer then the file will be used directly if already held in memory;
    * otherwise a buffer will be allocated in the module area.
    * Returns: a pointer to an OS error block, or else NULL for success.
    */

_Optional _kernel_oserror *messagetrans_close_file(MessagesFD */*mfd*/);
   /*
    * Closes the messages file associated with the messages file descriptor
    * that 'mfd' points to.
    */

_Optional _kernel_oserror *messagetrans_lookup(_Optional MessagesFD   */*mfd*/,
                                               const char             */*token*/,
                                               _Optional char         */*buffer*/,
                                               size_t                  /*buff_size*/,
                                               _Optional size_t       */*nbytes*/,
                                               size_t                  /*nparam*/,
                                               ...);
   /*
    * Looks up the message associated with a given token: first in the messages
    * file specified by 'mfd' and then (as a fallback) in the global messages
    * file. If a matching token was found, up to buff_size-1 characters of the
    * associated message will be copied to the 'buffer' array and a NUL
    * terminator appended. If 'buff_size' is 0 then nothing will be written and
    * 'buffer' may be a null pointer. Unless 'nbytes' is a null pointer, the
    * integer to which it points will be set to the number of bytes that would
    * have been output had a large enough buffer been provided. Up to four
    * parameters can be substituted into the message by replacing occurrences
    * of %0 to %3 with strings specified by the caller. The number of parameters
    * is specified by 'nparam', which must be followed by the expected number of
    * string pointers (any of which may be null to suppress substitution).
    * Returns: a pointer to an OS error block, or else NULL for success.
    */

_Optional _kernel_oserror *messagetrans_vlookup(_Optional MessagesFD   */*mfd*/,
                                                const char             */*token*/,
                                                _Optional char         */*buffer*/,
                                                size_t                  /*buff_size*/,
                                                _Optional size_t       */*nbytes*/,
                                                size_t                  /*nparam*/,
                                                va_list                 /*params*/);
   /*
    * Equivalent to messagetrans_lookup except that the variable argument list
    * is replaced by 'params', which must have been initialised by the va_start
    * macro (and possibly subsequent va_arg calls). Does not invoke va_end.
    * Returns: a pointer to an OS error block, or else NULL for success.
    */

_kernel_oserror *messagetrans_error_lookup(
                                _Optional MessagesFD */*mfd*/,
                                int                   /*errnum*/,
                                const char           */*token*/,
                                size_t                /*nparam*/,
                                ...);
   /*
    * Looks up the error message associated with a given token: first in the
    * messages file specified by 'mfd' and then (as a fallback) in the global
    * messages file. If a matching token was found then the error number and up
    * to 251 characters of the error message will be copied to an internal
    * buffer, and a NUL terminator appended. There are several such buffers,
    * which are shared between all tasks and continuously recycled. Parameter
    * substitution is performed as for messagetrans_lookup.
    * Returns: a pointer to an error block, which will contain a different
    *          error number and message if the token was not found.
    */

_kernel_oserror *messagetrans_error_vlookup(
                                _Optional MessagesFD  */*mfd*/,
                                int                    /*errnum*/,
                                const char            */*token*/,
                                size_t                 /*nparam*/,
                                va_list                /*params*/);
   /*
    * Equivalent to messagetrans_error_lookup except that the variable
    * argument list is replaced by 'params', which must have been initialised
    * by the va_start macro (and possibly subsequent va_arg calls). Does not
    * invoke va_end.
    * Returns: a pointer to an OS error block, or else NULL for success.
    */

#endif
