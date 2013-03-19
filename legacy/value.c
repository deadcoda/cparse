#include <string.h>
#include <stdio.h>
#include <cparse/value.h>
#include <cparse/object.h>

#include <assert.h>

CParseValue *cparse_value_new()
{
    CParseValue *value = malloc(sizeof(CParseValue));

    value->number_value = 0;
    value->type = kCPValueNumber;

    return value;
}

CParseValue *cparse_value_copy(CParseValue *other)
{
    CParseValue *value = cparse_value_new();

    value->type = other->type;

    switch(other->type)
    {
        case kCPValueNumber:
        case kCPValueBoolean:
            value->number_value = other->number_value;
            break;
        case kCPValueReal:
            value->real_value = other->real_value;
            break;
        case kCPValueString:
            value->object_value = strdup((char*)other->object_value);
            break;
        case kCPValueObject:
            value->object_value = cparse_object_copy((CParseObject*) other->object_value);
            break;
        case kCPValueArray:
            value->object_value = cparse_array_copy((CParseArray*) other->object_value);
            break;
    }
    return value;
}

CParseArray *cparse_array_copy(CParseArray *other)
{
    CParseArray *value = cparse_array_new();
    int i;

    value->size = other->size;
    value->value = calloc(sizeof(CParseValue*), other->size);

    for(i = 0; i < other->size; i++) {
        value->value[i] = cparse_value_copy(other->value[i]);
    }

    return value;
}

bool cparse_value_equals(CParseValue *value, CParseValue *other)
{
    switch(value->type)
    {
        case kCPValueNumber:
        case kCPValueBoolean:
            return value->number_value == other->number_value;
        case kCPValueReal:
        return value->real_value == other->real_value;
        case kCPValueString:
        return !strcmp((char*) value->object_value, (char*) other->object_value);
        case kCPValueObject:
        case kCPValueArray:
        default:
            return value->object_value == other->object_value;
    }
}

void cparse_value_clear(CParseValue *value)
{
    if (value->object_value)
    {
        switch (value->type)
        {
        case kCPValueString:
            free(value->object_value);
            break;
        case kCPValueObject:
            cparse_object_free((CParseObject *) value->object_value);
            break;
        case kCPValueArray:
            cparse_array_free((CParseArray *) value->object_value);
            break;
        default:
            break;
        }
    }
}
void cparse_value_free(CParseValue *value)
{
    cparse_value_clear(value);

    free(value);
}

CParseArray *cparse_array_new()
{
    CParseArray *value = malloc(sizeof(CParseArray));

    value->value = NULL;
    value->size = 0;

    return value;
}

void cparse_array_free(CParseArray *arr)
{
    if (arr->value)
    {
        int i;

        for (i = 0; i < arr->size; i++)
        {
            cparse_value_free(arr->value[i]);
        }
        free(arr->value);
    }

    free(arr);
}

void cparse_value_set_number(CParseValue *v, long long value)
{
    assert(v != 0);
    cparse_value_clear(v);
    v->type = kCPValueNumber;
    v->number_value = value;
}

void cparse_value_set_real(CParseValue *v, long double value)
{
    assert(v != 0);
    cparse_value_clear(v);
    v->type = kCPValueReal;
    v->real_value = value;
}

void cparse_value_set_bool(CParseValue *v, bool value)
{
    assert(v != 0);
    cparse_value_clear(v);
    v->type = kCPValueBoolean;
    v->number_value = value;
}

void cparse_value_set_string(CParseValue *v, const char *value)
{
    assert(v != 0);
    cparse_value_clear(v);
    v->type = kCPValueString;
    v->object_value = strdup(value);
}
void cparse_value_set_object(CParseValue *v, CParseObject *value)
{
    assert(v != 0);
    cparse_value_clear(v);
    v->type = kCPValueObject;
    v->object_value = value;
}

void cparse_value_set_array(CParseValue *v, CParseArray *value)
{
    assert(v != 0);
    cparse_value_clear(v);
    v->type = kCPValueArray;
    v->object_value = value;
}

long long cparse_value_get_number(CParseValue *v)
{
    assert(v != 0);
    return v->number_value;
}

bool cparse_value_get_bool(CParseValue *v)
{
    assert(v != 0);
    return v->number_value != 0;
}

long double cparse_value_get_real(CParseValue *v)
{
    assert(v != 0);
    return v->real_value;
}

char *cparse_value_get_string(CParseValue *v)
{
    assert(v != 0);
    return (char *) v->object_value;
}

CParseObject *cparse_value_get_object(CParseValue *v)
{
    assert(v != 0);
    return v->object_value;
}

int cparse_value_type(CParseValue *v)
{
    assert(v != 0);
    return v->type;
}

CParseArray *cparse_value_get_array(CParseValue *v)
{
    assert(v != 0);
    return (CParseArray *) v->object_value;
}

void cparse_array_add_value(CParseArray *arr, CParseValue *value)
{
    arr->value = realloc(arr->value, arr->size + 1);

    if (arr->value == NULL)
    {
        fputs("unable to allocate memory", stderr);
        exit(EXIT_FAILURE);
    }

    arr->value[arr->size++] = cparse_value_copy(value);
}

void cparse_array_add_number(CParseArray *arr, long long value)
{
    CParseValue v;
    cparse_value_set_number(&v, value);
    cparse_array_add_value(arr, &v);
}

void cparse_array_add_real(CParseArray *arr, long double real)
{
    CParseValue v;
    cparse_value_set_real(&v, real);
    cparse_array_add_value(arr, &v);
}

void cparse_array_add_bool(CParseArray *arr, bool real)
{
    CParseValue v;
    cparse_value_set_bool(&v, real);
    cparse_array_add_value(arr, &v);
}


void cparse_array_add_string(CParseArray *arr, const char *value)
{
    CParseValue v;
    cparse_value_set_string(&v, value);
    cparse_array_add_value(arr, &v);
}


void cparse_array_add_object(CParseArray *arr, CParseObject *value)
{
    CParseValue v;
    cparse_value_set_object(&v, value);
    cparse_array_add_value(arr, &v);
}

void cparse_array_add_array(CParseArray *arr, CParseArray *value)
{
    CParseValue v;
    cparse_value_set_array(&v, value);
    cparse_array_add_value(arr, &v);
}

CParseValue *cparse_array_get_value(CParseArray *arr, size_t index)
{
    assert(arr != NULL);

    if(index >= arr->size)
        return NULL;

    return arr->value[index];
}

long long cparse_array_get_number(CParseArray *arr, size_t index, long long defaultValue)
{
    CParseValue *value = cparse_array_get_value(arr, index);

    if(value)
        return cparse_value_get_number(value);
    else
        return defaultValue;
}
bool cparse_array_get_bool(CParseArray *arr, size_t index)
{
    CParseValue *value = cparse_array_get_value(arr, index);

    if(value)
        return cparse_value_get_bool(value);
    else
        return false;
}
long double cparse_array_get_real(CParseArray *arr, size_t index, long double defaultValue)
{
    CParseValue *value = cparse_array_get_value(arr, index);

    if(value)
        return cparse_value_get_real(value);
    else
        return defaultValue;
}

char* cparse_array_get_string(CParseArray *arr, size_t index)
{
    CParseValue *value = cparse_array_get_value(arr, index);

    if(value)
        return cparse_value_get_string(value);
    else
        return NULL;
}

CParseObject* cparse_array_get_object(CParseArray *arr, size_t index)
{
    CParseValue *value = cparse_array_get_value(arr, index);

    if(value)
        return cparse_value_get_object(value);
    else
        return NULL;
}

CParseArray* cparse_array_get_array(CParseArray *arr, size_t index)
{
    CParseValue *value = cparse_array_get_value(arr, index);

    if(value)
        return cparse_value_get_array(value);
    else
        return NULL;
}

CParseValue *cparse_array_remove(CParseArray *arr, size_t index)
{
    assert(arr != NULL);

    CParseValue *value, **temp;
    int i, j;

    if (index >= arr->size) return NULL;

    value = arr->value[index];

    temp = arr->value;

    arr->value = calloc(sizeof(CParseValue), arr->size - 1);

    for (i = j = 0; i < arr->size; i++)
    {
        if (i != index)
        {
            arr->value[j++] = temp[i];
        }
    }

    free(temp);

    return value;
}

size_t cparse_array_size(CParseArray *array) {
    return array->size;
}

size_t cparse_array_to_json(CParseArray *array, char *data, size_t pos)
{
    int i;

    pos += sprintf(&data[pos], "[");

    for(i = 0; i < array->size; i++) {
        pos = cparse_value_to_json(array->value[i], data, pos);
        if(i +1 < array->size) {
            pos += sprintf(&data[pos], ",");
        }
    }

    pos += sprintf(&data[pos], "]");

    return pos;
}

size_t cparse_value_to_json(CParseValue *value, char *data, size_t pos)
{

    switch(cparse_value_type(value))
    {
        case kCPValueBoolean:
            pos += sprintf(&data[pos], "%s", cparse_value_get_bool(value) ? "true" : "false");
        break;
        case kCPValueNumber:
            pos += sprintf(&data[pos], "%lld", cparse_value_get_number(value));
        break;
        case kCPValueReal:
            pos += sprintf(&data[pos], "%Lf", cparse_value_get_real(value));
        break;
        case kCPValueString:
        {
            char *temp = cparse_value_get_string(value);

            pos += sprintf(&data[pos], "\"%s\"", temp == NULL ? "null" : temp);
        }
        break;
        case kCPValueObject:
            pos = cparse_object_to_json(cparse_value_get_object(value), data, pos);
        break;
        case kCPValueArray:
            pos = cparse_array_to_json(cparse_value_get_array(value), data, pos);
        break;
    }
    return pos;
}

CParseValue *cparse_value_from_json(json_object *jobj)
{
    CParseValue *value = NULL;

    switch (json_object_get_type(jobj))
    {
    case json_type_null:
        break;
    case json_type_boolean: 
        value = cparse_value_new();
        cparse_value_set_bool(value, json_object_get_boolean(jobj));
        break;
    case json_type_double:
        value = cparse_value_new();
        cparse_value_set_real(value, json_object_get_double(jobj));
        break;
    case json_type_int:
        value = cparse_value_new();
        cparse_value_set_number(value, json_object_get_int(jobj));
        break;
    case json_type_object:
        value = cparse_value_new();
        cparse_value_set_object(value, cparse_object_from_json(jobj));
        break;
    case json_type_array:
        value = cparse_value_new();
        cparse_value_set_array(value, cparse_array_from_json(jobj));
    break;
    case json_type_string: 
        value = cparse_value_new();
        cparse_value_set_string(value, json_object_get_string(jobj));
        break;
    }
    return value;
}

CParseArray *cparse_array_from_json(json_object *jarray)
{
    size_t size = json_object_array_length(jarray);
    int i;

    CParseArray *array = cparse_array_new();

    for(i = 0; i < size; i++)
    {   
        json_object *jobj = json_object_array_get_idx(jarray, i);

        CParseValue *value = cparse_value_from_json(jobj);

        if(value != NULL) {
            cparse_array_add_value(array, value);
        }
    }

    return array;
}

