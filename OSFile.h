/*
 * CBOSLib: Veneers for filing system SWIs
 * Copyright (C) 2014 Christopher Bazley
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

/* OSFile.h declares functions to deal with files on RISC OS.

Dependencies: Acorn library kernel.
Message tokens: None.
History:
  CJB: 08-Nov-14: Created this header file.
  CJB: 23-Nov-14: Added veneers for os_file_generate_error and
                  os_file_read_catalogue_info_no_path.
  CJB: 29-May-16: Added a declaration of os_file_set_type.
  CJB: 11-Dec-20: Remove redundant uses of the 'extern' keyword.
*/

#ifndef OSFile_h
#define OSFile_h

/* ISO library headers */
#include <stddef.h>

/* Acorn C/C++ library headers */
#include "kernel.h"

/* Local headers */
#include "OSDate.h"

/* Constant for use with the os_file_create_dir function */
enum
{
  OS_File_CreateDir_DefaultNoOfEntries = 0
};

/* Constants for use with the os_file_generate_error function */
enum
{
  OS_File_GenerateError_FileNotFound,
  OS_File_GenerateError_IsAFile,
  OS_File_GenerateError_IsADirectory,
  OS_File_GenerateError_DirectoryNotFound = 0x100
};

/* --- RISC OS object types --- */

enum
{
  ObjectType_NotFound,
  ObjectType_File,
  ObjectType_Directory,
  ObjectType_Image
};

/* Catalogue information for a filing system object. */
typedef struct
{
  int load;                /* Load address of untyped file or file type and high byte
                              of date stamp */
  int exec;                /* Execution address of untyped file or low bytes of
                              date stamp */
  long int length;         /* Length of the file, in bytes.
                              Type is intended to match ftell and fseek. */
  unsigned int attributes; /* Bit mask specifying file access rights */
  int object_type;         /* i.e. file, directory or image file */
}
OS_File_CatalogueInfo;

int decode_load_exec(int /*load*/, int /*exec*/, OS_DateAndTime * /*time*/ );
   /*
    * Decodes an filing system object's load and execution addresses (e.g. from
    * a _kernel_osfile_block) to determine whether it has a date stamp and file
    * type. On entry 'time' must be a null pointer or point to an object in
    * which to store the date stamp in 5 byte UTC time format. If the load and
    * execution addresses are true addresses (i.e. the filing system object is
    * unstamped) then 00:00:00 01-Jan-1900 is stored instead.
    * Returns: The object's file type if it has one, otherwise FileType_None.
    *          Load and execution addresses cannot encode FileType_Directory,
    *          so check the basic object type separately, if it matters.
    */

_kernel_oserror *os_file_create_dir(const char * /*f*/, int /*n*/);
   /*
    * Creates a directory named 'f' with 'n' entries (or
    * OS_File_CreateDir_DefaultNoOfEntries). It is not an error if the directory
    * already exists.
    * Returns: a pointer to an OS error block, or else NULL for success.
    */

_kernel_oserror *os_file_generate_error(const char */*f*/, int /*object_type*/);
   /*
    * Creates an error message for an object of the given type named 'f'.
    * Returns: a pointer to an OS error block.
    */

_kernel_oserror *os_file_read_cat_no_path(const char */*f*/, OS_File_CatalogueInfo */*catalogue_info*/);
   /*
    * Reads catalogue information for an object named 'f'.
    * Returns: a pointer to an OS error block, or else NULL for success.
    */

_kernel_oserror *os_file_set_type(const char */*f*/, int /*type*/);
   /*
    * Sets the type of a specified file to indicate its contents (e.g. 0xfff
    * means text, whereas 0xfaf means HTML).
    * Returns: a pointer to an OS error block, or else NULL for success.
    */

#endif
