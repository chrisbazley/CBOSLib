/*
 * CBOSLib: RISC OS file types
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

/*
Dependencies: None.
Message tokens: None.
History:
  CJB: 29-Oct-18: Created this header file.
  CJB: 20-Oct-19: Added declarations of functions dealing with arrays of
                  file types.
  CJB: 26-Oct-19: Added declaration of in_file_types().
  CJB: 11-Dec-20: Remove redundant uses of the 'extern' keyword.
*/

#ifndef FileTypes_h
#define FileTypes_h

#include <stdbool.h>
#include <stddef.h>

enum
{
  FileType_Squash      = 0xfca,
  FileType_Sprite      = 0xff9,
  FileType_Data        = 0xffd,
  FileType_Text        = 0xfff,
  FileType_CSV         = 0xdfe,
  FileType_Obey        = 0xfeb,
  FileType_Directory   = 0x1000,
  FileType_Application = 0x2000,
  FileType_None        = 0x3000, /* From Wimp_MDataOpen */
  FileType_Null        = -1 /* Also used for untyped files, e.g. by
                               OS_FSControl 18 */
};

size_t count_file_types(const int */*file_types*/);
   /*
    * Counts the number of file types in an array terminated by
    * FileType_Null, such as that used in the drag and drop message
    * protocol.
    * Returns: number of file types, excluding the terminator.
    */

bool in_file_types(int /*file_type*/, const int */*file_types*/);
   /*
    * Finds whether or not a given file type is in an array terminated by
    * FileType_Null, such as that used in the drag and drop message
    * protocol.
    * Returns: true if the file type is a member of the array.
    */

int common_file_type(const int */*rec_file_types*/,
  const int */*send_file_types*/);
   /*
    * Finds the first file type in the 'rec_file_types' array that is also
    * in 'send_file_types'. Both arrays are in order of preference and must
    * be terminated by FileType_Null.
    * Returns: the chosen file type from 'send_file_types', or
    *          FileType_Null if there is no matching type.
    */

int pick_file_type(const int */*rec_file_types*/,
  const int */*send_file_types*/);
   /*
    * Picks the first file type in the 'rec_file_types' array that is also
    * in 'send_file_types', or the first file type in 'send_file_types' if
    * there is no matching type. Both arrays are in order of preference and
    * must be terminated by FileType_Null.
    * Returns: the chosen file type from 'send_file_types'.
    */

size_t copy_file_types(int */*dst*/, const int */*src*/, size_t /*max*/);
   /*
    * Copies up to 'max' file types from 'src' to 'dst'. FileType_Null and
    * file types that follow it are not copied. FileType_Null is always
    * appended to the result but not included in the count of file types.
    * Returns: number of file types copied, excluding the terminator.
    */

#endif
