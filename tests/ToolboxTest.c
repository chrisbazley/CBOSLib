/* CBOSLib host tests: ToolboxLib string getters.
 * Copyright (C) 2026 Christopher Bazley
 */
#undef NDEBUG
#include <assert.h>
#include <limits.h>
#include <string.h>
#include "gadgets.h"
#include "menu.h"
#include "saveas.h"
#include "textarea.h"
#include "MessTrans.h"
#include "TboxExtra.h"
#ifdef FORTIFY
#include "fortify.h"
#endif

enum Getter { Getter_Button, Getter_Display, Getter_Writable, Getter_Menu, Getter_SaveAs, Getter_TextArea };
static enum Getter expected_getter;
static const char *text = "value";
static bool fail_query, fail_read;
static int calls;
static _kernel_oserror native_error, space_error;

static _kernel_oserror *get_text(enum Getter getter, unsigned int flags,
  ObjectId object, ComponentId component, char *buffer, int size, int *nbytes)
{
  assert(getter == expected_getter);
  assert(flags == 0x12);
  assert(object == 42);
  assert(component == (getter == Getter_SaveAs ? NULL_ComponentId : 24));
  ++calls;
  if (buffer ? fail_read : fail_query)
  {
    return &native_error;
  }
  int const required = (int)strlen(text) + 1;
  if (buffer)
  {
    assert(size == required);
    memcpy(buffer, text, (size_t)required);
  }
  else
  {
    assert(size == 0);
    assert(nbytes);
  }
  if (nbytes)
  {
    *nbytes = required;
  }
  return NULL;
}

_kernel_oserror *button_get_value(unsigned int flags, ObjectId object,
  ComponentId component, char *buffer, int size, int *nbytes)
{
  return get_text(Getter_Button, flags, object, component, buffer, size, nbytes);
}

_kernel_oserror *displayfield_get_value(unsigned int flags, ObjectId object,
  ComponentId component, char *buffer, int size, int *nbytes)
{
  return get_text(Getter_Display, flags, object, component, buffer, size, nbytes);
}

_kernel_oserror *writablefield_get_value(unsigned int flags, ObjectId object,
  ComponentId component, char *buffer, int size, int *nbytes)
{
  return get_text(Getter_Writable, flags, object, component, buffer, size, nbytes);
}

_kernel_oserror *menu_get_entry_text(unsigned int flags, ObjectId object,
  ComponentId component, char *buffer, int size, int *nbytes)
{
  return get_text(Getter_Menu, flags, object, component, buffer, size, nbytes);
}

_kernel_oserror *saveas_get_file_name(unsigned int flags, ObjectId object,
  char *buffer, int size, int *nbytes)
{
  return get_text(Getter_SaveAs, flags, object, NULL_ComponentId, buffer, size, nbytes);
}

_kernel_oserror *textarea_get_text(unsigned int flags, ObjectId object,
  ComponentId component, char *buffer, int size, int *nbytes)
{
  return get_text(Getter_TextArea, flags, object, component, buffer, size, nbytes);
}

_kernel_oserror *messagetrans_error_lookup(MessagesFD *mfd, int errnum,
  const char *token, size_t nparam, ...)
{
  (void)errnum;
  assert(!mfd);
  assert(!strcmp(token, "BufOFlo"));
  assert(nparam == 0);
  return &space_error;
}

static _kernel_oserror *get_saveas(unsigned int flags, ObjectId object,
  ComponentId component, strb_t *output)
{
  assert(component == 24);
  return saveas_get_file_name_sb(flags, object, output);
}

int main(void)
{
  static struct {
    enum Getter getter;
    _kernel_oserror *(*get)(unsigned int, ObjectId, ComponentId, strb_t *);
  } const getters[] = {
    {Getter_Button, button_get_value_sb},
    {Getter_Display, displayfield_get_value_sb},
    {Getter_Writable, writablefield_get_value_sb},
    {Getter_Menu, menu_get_entry_text_sb},
    {Getter_SaveAs, get_saveas},
    {Getter_TextArea, textarea_get_text_sb}
  };

  for (size_t i = 0; i < sizeof getters / sizeof getters[0]; ++i)
  {
    expected_getter = getters[i].getter;
#ifdef FORTIFY
    Fortify_SetNumAllocationsLimit(0);
#endif
    STRB_DECL_FIXED(output, 63);
    assert(!strb_puts(output, "prefix:tail"));
    strb_seek(output, 7);
    assert(!getters[i].get(0x12, 42, 24, output));
    assert(!strcmp(strb_cptr(output), "prefix:valuetail"));
    assert(strb_tell(output) == 12);

    strb_empty(output);
    assert(!strb_puts(output, "ABCDEFGHIJK"));
    strb_setmode(output, strb_overwrite);
    strb_seek(output, 2);
    assert(!getters[i].get(0x12, 42, 24, output));
    assert(!strcmp(strb_cptr(output), "ABvalueHIJK"));
    assert(strb_getmode(output) == strb_overwrite);

    text = "";
    strb_seek(output, 2);
    assert(!getters[i].get(0x12, 42, 24, output));
    assert(!strcmp(strb_cptr(output), "ABvalueHIJK"));
    assert(strb_tell(output) == 2);
    text = "value";

    fail_query = true;
    assert(getters[i].get(0x12, 42, 24, output) == &native_error);
    assert(!strcmp(strb_cptr(output), "ABvalueHIJK"));
    assert(strb_tell(output) == 2);
    fail_query = false;
    fail_read = true;
    assert(getters[i].get(0x12, 42, 24, output) == &native_error);
    fail_read = false;

    STRB_DECL_FIXED(small, 3);
    assert(!strb_puts(small, "old"));
    calls = 0;
    assert(getters[i].get(0x12, 42, 24, small) == &space_error);
    assert(calls == 1);
    assert(!strcmp(strb_cptr(small), "old"));
    assert(strb_tell(small) == 3);
    assert(strb_error(small));

#ifdef FORTIFY
    STRB_DECL_ALLOC(growing, 0);
    assert(getters[i].get(0x12, 42, 24, growing) == &space_error);
    Fortify_SetNumAllocationsLimit(ULONG_MAX);
    assert(!strcmp(strb_cptr(growing), ""));
    assert(strb_tell(growing) == 0);
    strb_free(growing);
#endif
  }
  return 0;
}
