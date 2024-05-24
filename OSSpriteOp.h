/*
 * CBOSLib: Veneers for the OS_SpriteOp SWI
 * Copyright (C) 2019 Christopher Bazley
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
Dependencies: Acorn library kernel
Message tokens: None
History:
  CJB: 16-Mar-19: Created this header file
  CJB: 31-Mar-19: Corrected os_sprite_op_select output type.
  CJB: 26-Jun-22: Allow 'const' scale factors and colour translation table.
                  Added declarations of os_sprite_op_flip_x,
                  os_sprite_op_flip_y, os_sprite_op_plot_sprite,
                  os_sprite_op_plot_trans_quad_sprite and
                  os_sprite_op_plot_trans_matrix_sprite.

*/
#ifndef OSSpriteOp_h
#define OSSpriteOp_h

#include <stdbool.h>
#include "wimp.h"
#include "kernel.h"
#include "SprFormats.h"

_kernel_oserror *os_sprite_op_initialise(SpriteAreaHeader *area);

_kernel_oserror *os_sprite_op_load(SpriteAreaHeader *area,
  const char *file_name);

_kernel_oserror *os_sprite_op_save(SpriteAreaHeader *area,
  const char *file_name);

_kernel_oserror *os_sprite_op_read_header(SpriteAreaHeader *area,
  SpriteAreaHeader *hdr);

_kernel_oserror *os_sprite_op_verify(SpriteAreaHeader *area);

_kernel_oserror *os_sprite_op_create_sprite(SpriteAreaHeader *area,
  const char *name, bool has_palette, int width, int height, int mode);

_kernel_oserror *os_sprite_op_create_mask(SpriteAreaHeader *area,
  const char *name);

_kernel_oserror *os_sprite_op_read_sprite_info(SpriteAreaHeader *area,
  const char *name, bool *has_mask, int *width, int *height, int *mode);

_kernel_oserror *os_sprite_op_rename(SpriteAreaHeader *area,
  const char *old_name, const char *new_name);

_kernel_oserror *os_sprite_op_delete(SpriteAreaHeader *area,
  const char *name);

_kernel_oserror *os_sprite_op_set_pointer(SpriteAreaHeader *area,
  const char *name, unsigned int flags, int x, int y, ScaleFactors const *scale,
  void const *colours);

_kernel_oserror *os_sprite_op_plot_sprite(SpriteAreaHeader *area,
  const char *name, int x, int y, int action);

_kernel_oserror *os_sprite_op_plot_scaled_sprite(SpriteAreaHeader *area,
  const char *name, int x, int y, int action, ScaleFactors const *scale,
  void const *colours);

_kernel_oserror *os_sprite_op_plot_trans_quad_sprite(SpriteAreaHeader *area,
  const char *name, BBox const *src, int action, TransformQuad const *quad,
  void const *colours);

_kernel_oserror *os_sprite_op_plot_trans_matrix_sprite(SpriteAreaHeader *area,
  const char *name, BBox const *src, int action, TransformMatrix const *matrix,
  void const *colours);

typedef struct {
  int reason;
  SpriteAreaHeader *area;
  void *name_or_addr;
  void *buffer;
} SpriteRestoreOutputBlock;

_kernel_oserror *os_sprite_op_output_to_sprite(SpriteAreaHeader *area,
  const char *name, void *buffer, size_t buff_size, size_t *nbytes,
  SpriteRestoreOutputBlock *restore);

_kernel_oserror *os_sprite_op_output_to_mask(SpriteAreaHeader *area,
  const char *name, void *buffer, size_t buff_size, size_t *nbytes,
  SpriteRestoreOutputBlock *restore);

_kernel_oserror *os_sprite_op_restore_output(
  const SpriteRestoreOutputBlock *output);

_kernel_oserror *os_sprite_op_select(SpriteAreaHeader *area,
  const char *name, SpriteHeader **address);

_kernel_oserror *os_sprite_op_flip_x(SpriteAreaHeader *area,
  const char *name);

_kernel_oserror *os_sprite_op_flip_y(SpriteAreaHeader *area,
  const char *name);

#endif /* OSSpriteOp_h */
