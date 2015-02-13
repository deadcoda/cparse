#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <cparse/user.h>
#include <cparse/parse.h>
#include <cparse/json.h>
#include <cparse/object.h>

#include "protocol.h"
#include "client.h"

#define USER_CLASS_NAME "users"

#define USER_USERNAME "username"

#define USER_EMAIL "email"

#define USER_IS_NEW "isNew"

#define USER_SESSION_TOKEN "sessionToken"

char current_user_token[BUFSIZ + 1] = {0};

extern CPARSE_QUERY *cparse_query_new();

extern void cparse_object_includes_to_buffer(CPARSE_OBJ *obj, char *buf, size_t bufSiz);

CPARSE_OBJ *cparse_user_new()
{
    return cparse_object_with_class_name(USER_CLASS_NAME);
}

CPARSE_OBJ *cparse_user_with_name(const char *username)
{
    CPARSE_OBJ *obj = cparse_object_with_class_name(USER_CLASS_NAME);

    cparse_object_set_string(obj, USER_USERNAME, username);

    return obj;
}

CPARSE_OBJ *cparse_current_user(CPARSE_ERROR **error)
{
    CPARSE_OBJ *user;

    if (!*current_user_token)
        return NULL;

    user = cparse_object_with_class_name(USER_CLASS_NAME);

    if (!cparse_user_validate(user, current_user_token, error))
    {
        cparse_object_free(user);

        return NULL;
    }

    return user;
}

/* getters/setters */

const char *cparse_user_name(CPARSE_OBJ *user)
{
    if (!user) return NULL;

    return cparse_object_get_string(user, USER_USERNAME);
}

void cparse_user_set_name(CPARSE_OBJ *user, char *name)
{
    if (user)
    {
        cparse_object_set_string(user, USER_USERNAME, name);
    }
}

const char *cparse_user_email(CPARSE_OBJ *user)
{
    return cparse_object_get_string(user, USER_EMAIL);
}

const char *cparse_user_session_token(CPARSE_OBJ *user)
{
    return cparse_object_get_string(user, USER_SESSION_TOKEN);
}

bool cparse_user_is_new(CPARSE_OBJ *user)
{
    return cparse_object_get_bool(user, USER_IS_NEW);
}

/* functions */

CPARSE_OBJ *cparse_user_login(const char *username, const char *password, CPARSE_ERROR **error)
{
    char buf[BUFSIZ + 1];
    CPARSE_OBJ *user;
    CPARSE_JSON *data;

    if (!username || !*username)
    {
        if (error)
            *error = cparse_error_with_message("No username, set with cparse_user_set_name()");

        return false;
    }

    if (!password || !*password)
    {
        if (error)
            *error = cparse_error_with_message("No password provided");

        return false;

    }
    user = cparse_object_with_class_name(USER_CLASS_NAME);

    snprintf(buf, BUFSIZ, "username=%s&password=%s", username, password);

    if (!cparse_client_object_request(user, HTTPRequestMethodGet, "login", buf, error))
    {

        cparse_object_free(user);
        return NULL;
    }

    if (cparse_object_contains(user, USER_SESSION_TOKEN))
    {
        const char *sessionToken = cparse_object_get_string(user, USER_SESSION_TOKEN);

        if (sessionToken)
        {
            strncpy(current_user_token, sessionToken, BUFSIZ);
        }
    }
    return user;
}

void cparse_user_login_in_background(const char *username, const char *password, CPARSE_OBJ_CALLBACK callback)
{

}

void cparse_user_logout()
{

}

bool cparse_user_delete(CPARSE_OBJ *obj, CPARSE_ERROR **error)
{
    CPARSE_REQUEST *request;
    char buf[BUFSIZ + 1];
    const char *userId;
    const char *sessionToken;

    if (!obj || !(userId = cparse_object_id(obj)) || !*userId)
    {
        if (error)
            *error = cparse_error_with_message("User has no id");
        return false;
    }

    if (!(sessionToken = cparse_user_session_token(obj)) || !*sessionToken)
    {
        if (error)
            *error = cparse_error_with_message("User has no session token");
        return false;
    }

    request = cparse_client_request_new();

    snprintf(buf, BUFSIZ, "%s/%s", USER_CLASS_NAME, userId);

    request->path = strdup(buf);

    request->method = HTTPRequestMethodDelete;

    cparse_client_request_add_header(request, HEADER_SESSION_TOKEN, sessionToken);

    cparse_client_request_perform(request, error);

    cparse_client_request_free(request);

    return error == NULL || *error == NULL;
}


CPARSE_QUERY *cparse_user_query()
{
    CPARSE_QUERY *query = cparse_query_new();

    return query;
}

bool cparse_user_sign_up(CPARSE_OBJ *user, const char *password, CPARSE_ERROR **error)
{
    const char *username;
    const char *attributes;

    bool rval;

    if (!user) return false;

    username = cparse_object_get_string(user, USER_USERNAME);

    if (!username || !*username)
    {
        if (error)
            *error = cparse_error_with_message("No username, set with cparse_user_set_name()");

        return false;
    }

    if (!password || !*password)
    {
        if (error)
            *error = cparse_error_with_message("No password provided");

        return false;
    }

    cparse_object_set_string(user, "password", password);

    attributes = cparse_object_to_json_string(user);

    cparse_object_remove(user, "password");

    if (cparse_client_object_request(user, HTTPRequestMethodPost, USER_CLASS_NAME, attributes, error))
    {
        if (cparse_object_contains(user, USER_SESSION_TOKEN))
        {
            const char *sessionToken = cparse_object_get_string(user, USER_SESSION_TOKEN);

            strncpy(current_user_token, sessionToken, BUFSIZ);
        }
        return true;
    }

    return false;
}

bool cparse_user_fetch(CPARSE_OBJ *obj, CPARSE_ERROR **error)
{
    const char *id;
    char buf[BUFSIZ + 1] = {0};
    char includes[BUFSIZ + 1] = {0};

    if (!obj || !(id = cparse_object_id(obj)) || !*id)
    {
        if (error)
            *error = cparse_error_with_message("object has no id");

        return false;
    }

    snprintf(buf, BUFSIZ, "%s/%s", USER_CLASS_NAME, id);

    cparse_object_includes_to_buffer(obj, includes, BUFSIZ);

    return cparse_client_object_request(obj, HTTPRequestMethodGet, buf, includes, error);
}

bool cparse_user_refresh(CPARSE_OBJ *obj, CPARSE_ERROR **error)
{
    const char *id;
    char buf[BUFSIZ + 1] = {0};

    if (!obj || !(id = cparse_object_id(obj)) || !*id)
    {
        if (error)
            *error = cparse_error_with_message("user has no id");
        return false;
    }

    snprintf(buf, BUFSIZ, "%s/%s", USER_CLASS_NAME, id);

    return cparse_client_object_request(obj, HTTPRequestMethodGet, buf, NULL, error);
}

bool cparse_user_validate(CPARSE_OBJ *user, const char *sessionToken, CPARSE_ERROR **error)
{
    CPARSE_REQUEST *request;

    CPARSE_JSON *json;

    request = cparse_client_request_new();

    request->method = HTTPRequestMethodGet;

    request->path = strdup("me");

    cparse_client_request_add_header(request, HEADER_SESSION_TOKEN, sessionToken);

    json = cparse_client_request_perform_and_get_json(request, error);

    cparse_client_request_free(request);

    if (json == NULL)
    {
        return false;
    }

    cparse_object_merge_json(user, json);

    cparse_json_free(json);

    return true;
}

bool cparse_user_validate_email(CPARSE_OBJ *user, CPARSE_ERROR **error)
{
    if (!user) return false;

    if (!cparse_object_contains(user, "emailVerified"))
    {
        return false;
    }

    if (!cparse_object_get_bool(user, "emailVerified"))
    {
        if (!cparse_user_refresh(user, error))
        {
            return false;
        }

        return cparse_object_get_bool(user, "emailVerified");
    }

    return true;
}

bool cparse_user_reset_password(CPARSE_OBJ *user, CPARSE_ERROR **error)
{
    CPARSE_REQUEST *request;
    CPARSE_JSON *json;

    if (!user) return false;

    if (!cparse_object_contains(user, USER_EMAIL))
    {
        if (error)
            *error = cparse_error_with_message("User has no email");
        return false;
    }

    if (!cparse_user_validate_email(user, error))
    {
        if (error)
            *error = cparse_error_with_message("User has no valid email");
        return false;
    }

    request = cparse_client_request_new();

    request->method = HTTPRequestMethodPost;

    request->path = strdup("requestPasswordReset");

    json = cparse_json_new();

    cparse_json_set_string(json, USER_EMAIL, cparse_object_get_string(user, USER_EMAIL));

    request->payload = strdup(cparse_json_to_json_string(json));

    cparse_json_free(json);

    json = cparse_client_request_perform_and_get_json(request, error);

    if (json == NULL)
    {
        return false;
    }

    return true;
}

