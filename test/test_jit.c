#include "test_util.h"
#include "jit/jit.h"

static vcode_unit_t context = NULL;
static vcode_type_t vint32 = VCODE_INVALID_TYPE;

static void setup(void)
{
   context = emit_context(ident_new("unit_test"));
   vint32 = vtype_int(INT32_MIN, INT32_MAX);
}

static void teardown(void)
{
   vcode_unit_unref(context);
}

START_TEST(test_add1)
{
   vcode_unit_t unit = emit_thunk(ident_new("add1"), context, vint32);
   emit_return(emit_const(vint32, 42));
   vcode_dump();

   uint32_t (*fn)(void) = jit_vcode_unit(unit);
   fail_if(fn == NULL);

   uint32_t result = (*fn)();
   printf("result=%d\n", result);
   fail_unless(result == 42);
}
END_TEST

Suite *get_jit_tests(void)
{
   Suite *s = suite_create("jit");

   TCase *tc = nvc_unit_test();
   tcase_add_test(tc, test_add1);
   tcase_add_checked_fixture(tc, setup, teardown);
   suite_add_tcase(s, tc);

   return s;
}
