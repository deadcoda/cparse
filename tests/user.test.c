#include <check.h>
#include <stdio.h>
#include <cparse/user.h>

#include "parse.test.h"

static void cparse_test_setup()
{

}

static void cparse_test_teardown()
{
    cparse_cleanup_test_objects();
}

START_TEST(test_cparse_user_sign_up)
{
    cParseError *error = NULL;
    cParseObject *user = cparse_user_new();

    cparse_user_set_name(user, "user123");

    cparse_object_set_string(user, "bio", "hello, world");

    error = NULL;

    cparse_user_sign_up(user, "Passw0rd!", &error);

    if (error)
        printf("user signup error: %s\n", cparse_error_message(error));

    fail_unless(error == NULL);

    fail_unless(cparse_user_session_token(user) != NULL);

    cparse_user_delete(user, &error);

    if (error)
        printf("user delete error: %s\n", cparse_error_message(error));

    cparse_object_free(user);
}
END_TEST

START_TEST(test_cparse_user_login)
{
    cParseError *error = NULL;

    cParseObject *user123 = cparse_user_new(), *user;

    cparse_user_set_name(user123, "user123");

    cparse_object_set_string(user123, "bio", "hello, world");

    fail_unless(cparse_user_sign_up(user123, "Passw0rd!", &error));

    if (error)
        printf("user signup error: %s\n", cparse_error_message(error));

    fail_unless(error == NULL);

    cparse_object_free(user123);

    user = cparse_user_login("user123", "Passw0rd!", &error);

    if (error)
        printf("user login error: %s\n", cparse_error_message(error));

    fail_unless(error == NULL);

    fail_unless(user != NULL);

    fail_unless(!strcmp(cparse_user_name(user), "user123"));

    fail_unless(cparse_user_session_token(user) != NULL);

    cparse_user_delete(user123, &error);

    if (error)
        printf("user delete error: %s\n", cparse_error_message(error));

    cparse_object_free(user);
}
END_TEST

Suite *cparse_user_suite (void)
{
    Suite *s = suite_create ("User");

    /* Core test case */
    TCase *tc = tcase_create ("User");
    tcase_add_checked_fixture(tc, cparse_test_setup, cparse_test_teardown);
    tcase_add_test(tc, test_cparse_user_sign_up);
    tcase_add_test(tc, test_cparse_user_login);

    tcase_set_timeout(tc, 30);

    suite_add_tcase(s, tc);

    return s;
}

