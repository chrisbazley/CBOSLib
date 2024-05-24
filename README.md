# CBOSLib
(C) 2018 Christopher Bazley

Release 7 (17 Jun 2023)

Introduction
------------
  This C library provides type-safe veneers to miscellaneous OS-specific
functionality not provided by the Wimp and Toolbox libraries supplied with
Acorn C/C++. It should have minimal dependencies on other libraries and
must not include any code or interfaces that could be useful on other
platforms. It should not dynamically allocate memory.

Rebuilding the library
----------------------
  You should ensure that the standard C library and CBDebugLib (by the same
author as CBOSLib) are on your C$Path, otherwise the compiler won't be able
to find the required header files. The dependency on CBDebugLib isn't very
strong: it can be eliminated by modifying the make file so that the macro
USE_CBDEBUG is no longer predefined.

  Two make files are supplied:

- 'Makefile' is intended for use with Acorn Make Utility (AMU) and the
   Norcroft C compiler supplied with the Acorn C/C++ Development Suite.
- 'GMakefile' is intended for use with GNU Make and the GNU C Compiler.

These make files share some variable definitions (lists of objects to be
built) by including a common make file.

  The APCS variant specified for the Norcroft compiler is 32 bit for
compatibility with ARMv5 and fpe2 for compatibility with older versions of
the floating point emulator. Generation of unaligned data loads/stores is
disabled for compatibility with ARM v6.

  The suffix rules put object files in one of several directories, depending
on the compiler options used to compile them:

o: Assertions and debugging output are disabled. The code is optimised for
   execution speed.

oz: Assertions and debugging output are disabled. The code is suitable for
    inclusion in a relocatable module (multiple instantiation of static
    data and stack limit checking disabled). When the Norcroft compiler is
    used, the compiler optimises for smaller code size. (The equivalent GCC
    option seems to be broken.)

debug: Assertions and debugging output are enabled. The code includes
       symbolic debugging data (e.g. for use with DDT). The macro FORTIFY
       is pre-defined to enable Simon P. Bullen's fortified shell for memory
       allocations.

d: 'GMakefile' passes '-MMD' when invoking gcc so that dynamic dependencies
   are generated from the #include commands in each source file and output
   to a temporary file in the directory named 'd'. GNU Make cannot
   understand rules that contain RISC OS paths such as /C:Macros.h as
   prerequisites, so 'sed', a stream editor, is used to strip those rules
   when copying the temporary file to the final dependencies file.

  The above suffixes must be specified in various system variables which
control filename suffix translation on RISC OS, including at least
UnixEnv$ar$sfix, UnixEnv$gcc$sfix and UnixEnv$make$sfix.
Unfortunately GNU Make doesn't apply suffix rules to make object files in
subdirectories referenced by path even if the directory name is in
UnixEnv$make$sfix, which is why 'GMakefile' uses the built-in function
addsuffix instead of addprefix to construct lists of the objects to be
built (e.g. foo.o instead of o.foo).

  Before compiling the library for RISC OS, move the C source and header
files with .c and .h suffixes into subdirectories named 'c' and 'h' and
remove those suffixes from their names. You probably also need to create
'o', 'oz', 'd' and 'debug' subdirectories for compiler output.

Licence and disclaimer
----------------------
  This library is free software; you can redistribute it and/or modify it
under the terms of the GNU Lesser General Public License as published by the
Free Software Foundation; either version 2.1 of the License, or (at your
option) any later version.

  This library is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License
for more details.

  You should have received a copy of the GNU Lesser General Public License
along with this library; if not, write to the Free Software Foundation,
Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

Credits
-------
  This library was derived from CBLibrary. Both libraries are free software,
available under the same license.

History
-------
Release 1 (04 Nov 2018)
- Extracted the relevant components from CBLib to make a standalone library.

Release 2 (03 Nov 2019)
- Added functions for handling -1 terminated arrays of file types.
- Added veneers for SWI OS_SpriteOp, OS_SetColour, OS_ReadMonotonicTime and
  OS_FSControl 26 (copy objects).
- Added a veneer for SWI OS_Word 21,1 (set mouse pointer bounding box).
- Added definitions of OS_SpriteOp 62 (read save area size) and a struct
  for scale factors used with OS_SpriteOp 52 (plot scaled).

Release 3 (30 Sep 2020)
- Fixed null pointers instead of strings passed to DEBUGF.
- messagetrans_lookup() now outputs the number of bytes required to store
  the full output (as documented) instead of 'size of result before
  terminator' (like the SWI).
- Palette data passed to nearest_palette_entry() can now be const.
- Minor refactoring of PalEntry.c.

Release 4 (28 Oct 2020)
- Made definition of Wimp_ReportError_UseCategory conditional for
  compatibility with newer "wimp.h".
- Fixed null pointer instead of string passed to DEBUGF in
  messagetrans_error_vlookup().

Release 5 (30 Jul 2022)
- Added wimp_drag_box2() for drag operations within windows.
- Added wimp_auto_scroll() for automatic scrolling of windows.
- Added os_sprite_op_flip_x() and os_sprite_op_flip_y() for flipping sprites
  horizontally or vertically.
- Added os_sprite_op_plot_trans_quad_sprite() and
  os_sprite_op_plot_trans_matrix_sprite() for plotting transformed sprites.
- Added os_sprite_op_plot_sprite() for plotting sprites with no scaling or
  colour translation.
- Assert that the values passed to hourglass_percentage() and os_plot() are
  within sensible ranges.
- Remove redundant uses of the 'extern' keyword.
- Use initializers on _kernel_swi_regs declarations where suitable.
- Correct text of debugging output from os_sprite_op_save().
- Allow 'const' scale factors and colour translation table to be used with
  os_sprite_op_plot_scaled_sprite() and os_sprite_op_set_pointer().

Release 6 (01 Aug 2022)
- Only assert that a non-null pointer was passed to wimp_auto_scroll() if
  enabling scrolling.

Release 7 (17 Jun 2023)
- Use size_t rather than unsigned int for the number of substitution
  parameters passed to messagetrans functions.

Contact details
---------------
Christopher Bazley

Email: mailto:cs99cjb@gmail.com

WWW:   http://starfighter.acornarcade.com/mysite/
