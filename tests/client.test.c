#include "../src/client.h"
#include "../src/private.h"
#include "parse.test.h"
#include <check.h>

static void cparse_test_setup()
{
}

static void cparse_test_teardown()
{
}

START_TEST(test_cparse_client_payload)
{
    cParseRequest *request = cparse_client_request_with_method_and_path(HttpRequestMethodGet, "users");

    cparse_client_request_add_data(request, "key", "value");

    cparse_client_request_set_payload(request, "key=value");

    fail_unless(request->data != NULL);

    fail_unless(request->data->next == NULL);

    fail_unless(request->data->key == NULL);

    fail_unless(!strcmp(request->data->value, "key=value"));
}
END_TEST


START_TEST(test_cparse_client_bad_request)
{
    cParseError *error;

    cParseRequest *request = cparse_client_request_with_method_and_path(HttpRequestMethodGet, "classes/" TEST_CLASS "/sk4k3kmf");

    cParseJson *json = cparse_client_request_get_json(request, &error);

    fail_unless(json == NULL);
}
END_TEST

Suite *cparse_client_suite (void)
{
    Suite *s = suite_create ("Client");

    TCase *tc = tcase_create("Request");
    tcase_add_checked_fixture(tc, cparse_test_setup, cparse_test_teardown);
    tcase_add_test(tc, test_cparse_client_payload);
    tcase_add_test(tc, test_cparse_client_bad_request);
    suite_add_tcase(s, tc);

    return s;
}
