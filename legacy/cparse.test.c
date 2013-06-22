
#include <check.h>
#include <stdlib.h>
#include <stdio.h>
#include <cparse/json.h>
#include <cparse/parse.h>

Suite *cparse_parse_suite();
Suite *cparse_json_suite();
Suite *cparse_object_suite();

void die(const char *message)
{
    printf("ERROR: %s\n", message);
    abort();
}

void read_test_config()
{
    FILE *file = fopen("./parse.test.json", "rb");
    char *text;
    long fsize;
    CParseJSON *config;

    if (!file)
        die("parse.test.json not found");

    fseek(file, 0, SEEK_END);
    fsize = ftell(file);
    fseek(file, 0, SEEK_SET);

    text = malloc(sizeof(char) * fsize + 1);

    memset(text, 0, sizeof(char) * fsize + 1);

    fread(text, sizeof(char), fsize, file);

    fclose(file);

    config = cparse_json_tokenize(text);

    if (cparse_json_contains(config, "parseAppId"))
        cparse_set_application_id(cparse_json_get_string(config,"parseAppId"));
    else
        die("No app id");

    if (cparse_json_contains(config, "parseApiKey"))
        cparse_set_api_key(cparse_json_get_string(config, "parseApiKey"));
    else
        die("No api key");

}
int main(void)
{
    int number_failed;
    SRunner *sr = srunner_create(cparse_parse_suite());

    read_test_config();

    srunner_add_suite(sr, cparse_json_suite());
    srunner_add_suite(sr, cparse_object_suite());
    srunner_run_all (sr, CK_NORMAL);
    number_failed = srunner_ntests_failed (sr);
    srunner_free (sr);
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
