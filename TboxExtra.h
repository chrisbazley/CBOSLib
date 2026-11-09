/*
 * CBOSLib: Toolbox string-buffer getters
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
Dependencies: Acorn Toolbox library and strb_t.
Message tokens: BufOFlo.
History:
  CJB: 09-Nov-26: Added string-buffer forms of Toolbox text getters.
*/

#ifndef TboxExtra_h
#define TboxExtra_h

#include "toolbox.h"
#include "strb.h"

/* These getters have the same flags, object and component meanings as their
 * ToolboxLib counterparts. They write the complete string at output's current
 * position, respecting its insertion or overwrite mode. The caller controls
 * storage allocation and may supply fixed character storage.
 *
 * Returns: NULL on success, the original Toolbox error on lookup failure, or
 * the global BufOFlo error if the string buffer cannot provide enough space.
 * A failed size query or write preparation leaves contents and position
 * unchanged. Output may have been modified if the second Toolbox call fails.
 * The existing ToolboxLib functions and headers are unchanged.
 */

_Optional _kernel_oserror *button_get_value_sb(unsigned int flags,
  ObjectId object, ComponentId component, strb_t *output);

_Optional _kernel_oserror *displayfield_get_value_sb(unsigned int flags,
  ObjectId object, ComponentId component, strb_t *output);

_Optional _kernel_oserror *writablefield_get_value_sb(unsigned int flags,
  ObjectId object, ComponentId component, strb_t *output);

_Optional _kernel_oserror *menu_get_entry_text_sb(unsigned int flags,
  ObjectId object, ComponentId component, strb_t *output);

_Optional _kernel_oserror *saveas_get_file_name_sb(unsigned int flags,
  ObjectId object, strb_t *output);

_Optional _kernel_oserror *textarea_get_text_sb(unsigned int flags,
  ObjectId object, ComponentId component, strb_t *output);

#endif /* TboxExtra_h */
