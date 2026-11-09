/* CBOSLib host tests: MessageTrans string-buffer output.
 * Copyright (C) 2026 Christopher Bazley
 */
#undef NDEBUG
#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include "MessTrans.h"
#ifdef FORTIFY
#include "fortify.h"
#endif

static MessagesFD file;
static MessagesFD *expected_file = &file;
static _kernel_oserror error;
static bool fail_measure, fail_write;

_kernel_oserror *messagetrans_error_lookup(MessagesFD *mfd, int errnum,
  const char *token, size_t nparam, ...)
{
  assert(!mfd);
  assert(!strcmp(token, "BufOFlo"));
  assert(nparam == 0);
  error.errnum = errnum;
  return &error;
}

_kernel_oserror *messagetrans_vlookup(MessagesFD *mfd, const char *token,
  char *buffer, size_t size, size_t *nbytes, size_t nparam, va_list params)
{
  assert(mfd == expected_file);
  assert(!strcmp(token, "Test"));
  if (buffer ? fail_write : fail_measure)
  {
    return &error;
  }
  const char *text = "plain";
  if (nparam)
  {
    assert(nparam == 2);
    const char *first = va_arg(params, char *), *second = va_arg(params, char *);
    assert(!strcmp(first, "first"));
    assert(!strcmp(second, "second"));
    text = "first:second";
  }
  size_t const needed = strlen(text) + 1;
  if (nbytes)
  {
    *nbytes = needed;
  }
  if (buffer)
  {
    assert(size == needed);
    memcpy(buffer, text, needed);
  }
  return NULL;
}

static _kernel_oserror *lookup(strb_t *output, size_t nparam, ...)
{
  va_list params;
  va_start(params, nparam);
  _kernel_oserror *const e =
    messagetrans_vlookup_sb(&file, "Test", output, nparam, params);
  va_end(params);
  return e;
}

int main(void)
{
#ifdef FORTIFY
  Fortify_SetNumAllocationsLimit(0);
#endif
  STRB_DECL_FIXED(output, 63);
  assert(!strb_puts(output, "prefix:tail"));
  strb_seek(output, 7);
  assert(!messagetrans_lookup_sb(&file, "Test", output, 0));
  assert(!strcmp(strb_cptr(output), "prefix:plaintail"));
  assert(strb_tell(output) == 12);

  strb_empty(output);
  assert(!strb_puts(output, "prefix:tail"));
  strb_seek(output, 7);
  assert(!lookup(output, 2, "first", "second"));
  assert(!strcmp(strb_cptr(output), "prefix:first:secondtail"));
  assert(strb_tell(output) == 19);

  strb_empty(output);
  assert(!strb_puts(output, "XXXXXXXXXXXX"));
  strb_setmode(output, strb_overwrite);
  strb_seek(output, 2);
  assert(!messagetrans_lookup_sb(&file, "Test", output, 0));
  assert(!strcmp(strb_cptr(output), "XXplainXXXXX"));
  assert(strb_getmode(output) == strb_overwrite);

  expected_file = NULL;
  strb_empty(output);
  assert(!messagetrans_lookup_sb(NULL, "Test", output, 0));
  assert(!strcmp(strb_cptr(output), "plain"));
  expected_file = &file;
  strb_empty(output);
  assert(!strb_puts(output, "XXXXXXXXXXXX"));
  strb_seek(output, 2);
  assert(!messagetrans_lookup_sb(&file, "Test", output, 0));

  STRB_DECL_FIXED(small, 3);
  assert(!strb_puts(small, "old"));
  assert(messagetrans_lookup_sb(&file, "Test", small, 0));
  assert(!strcmp(strb_cptr(small), "old"));
  assert(strb_tell(small) == 3);
  assert(strb_error(small));

  fail_measure = true;
  assert(messagetrans_lookup_sb(&file, "Test", output, 0) == &error);
  assert(!strcmp(strb_cptr(output), "XXplainXXXXX"));
  assert(strb_tell(output) == 7);
  fail_measure = false;
  fail_write = true;
  assert(lookup(output, 2, "first", "second") == &error);
  fail_write = false;
#ifdef FORTIFY
  STRB_DECL_ALLOC(growing, 0);
  assert(messagetrans_lookup_sb(&file, "Test", growing, 0));
  Fortify_SetNumAllocationsLimit(ULONG_MAX);
  assert(!strcmp(strb_cptr(growing), ""));
  assert(strb_tell(growing) == 0);
  strb_free(growing);
#endif
  return 0;
}
