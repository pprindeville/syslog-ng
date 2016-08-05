#include "str-repr/decode.h"
#include "testutils.h"

#define str_repr_decode_testcase_begin(func, args)             \
  do                                                            \
    {                                                           \
      testcase_begin("%s(%s)", func, args);                     \
    }                                                           \
  while (0)

#define str_repr_decode_testcase_end()                           \
  do                                                            \
    {                                                           \
      testcase_end();                                           \
    }                                                           \
  while (0)

#define STR_REPR_DECODE_TESTCASE(x, ...) \
  do {                                                          \
      str_repr_decode_testcase_begin(#x, #__VA_ARGS__);  		\
      x(__VA_ARGS__);                                           \
      str_repr_decode_testcase_end();                                \
  } while(0)


GString *value;

static void
assert_decode_equals(const gchar *input, const gchar *expected)
{
  GString *str = g_string_new("");
  const gchar *end;

  str_repr_decode(str, input, &end);
  assert_string(str->str, expected, "Decoded value does not match expected");
  g_string_free(str, TRUE);
}

static void
test_decode_unquoted_strings(void)
{
  assert_decode_equals("a", "a");
  assert_decode_equals("alma", "alma");
  assert_decode_equals("al ma", "al");
}

static void
test_decode_double_quoted_strings(void)
{
  assert_decode_equals("\"al ma\"", "al ma");
  /* embedded quote */
  assert_decode_equals("\"\\\"value1\"", "\"value1");
  /* control sequences */
  assert_decode_equals("\"\\b \\f \\n \\r \\t \\\\\"", "\b \f \n \r \t \\");
  /* unknown backslash escape is left as is */
  assert_decode_equals("\"\\p\"", "\\p");
}

static void
test_decode_apostrophe_quoted_strings(void)
{
  assert_decode_equals("'al ma'", "al ma");
  /* embedded quote */
  assert_decode_equals("'\\'value1'", "'value1");

  /* control sequences */
  assert_decode_equals("'\\b \\f \\n \\r \\t \\\\'", "\b \f \n \r \t \\");
  /* unknown backslash escape is left as is */
  assert_decode_equals("'\\p\'", "\\p");
}

int
main(int argc, char *argv[])
{
  STR_REPR_DECODE_TESTCASE(test_decode_unquoted_strings);
  STR_REPR_DECODE_TESTCASE(test_decode_double_quoted_strings);
  STR_REPR_DECODE_TESTCASE(test_decode_apostrophe_quoted_strings);
  return 0;
}