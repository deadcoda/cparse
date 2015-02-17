
/*!
 * @header Structure, methods and constants related to variants
 * @ignore __VARIANT_H_
 */
#ifndef CPARSE_JSON_H_
#define CPARSE_JSON_H_

#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

#include <cparse/defines.h>

/* JSON */
typedef enum
{
    kCParseJSONNumber,
    kCParseJSONReal,
    kCParseJSONString,
    kCParseJSONBoolean,
    kCParseJSONObject,
    kCParseJSONArray,
    kCParseJSONNull
} CParseJSONType;

/* value initializers */
CPARSE_JSON *cparse_json_new();
CPARSE_JSON *cparse_json_new_reference(CPARSE_JSON *orig);
CPARSE_JSON *cparse_json_new_number(cparse_number);
CPARSE_JSON *cparse_json_new_real(double);
CPARSE_JSON *cparse_json_new_bool(bool value);
CPARSE_JSON *cparse_json_new_string(const char *str);
CPARSE_JSON *cparse_json_new_array();

/* copy method */
void cparse_json_copy(CPARSE_JSON *orig, CPARSE_JSON *other, bool replaceOnConflict);

/* value cleanup */
void cparse_json_free(CPARSE_JSON *value);

/* value setters */

void cparse_json_set(CPARSE_JSON *obj, const char *key, CPARSE_JSON *value);
void cparse_json_set_number(CPARSE_JSON *obj, const char *key, cparse_number value);
void cparse_json_set_real(CPARSE_JSON *obj, const char *key, double value);
void cparse_json_set_string(CPARSE_JSON *obj, const char *key, const char *value);
void cparse_json_set_bool(CPARSE_JSON *obj, const char *key, bool value);

/* object getters */
CPARSE_JSON *cparse_json_get(CPARSE_JSON *obj, const char *key);
cparse_number cparse_json_get_number(CPARSE_JSON *obj, const char *key, cparse_number def);
double cparse_json_get_real(CPARSE_JSON *obj, const char *key, double def);
bool cparse_json_get_bool(CPARSE_JSON *obj, const char *key);
const char *cparse_json_get_string(CPARSE_JSON *obj, const char *key);

bool cparse_json_is_array(CPARSE_JSON *obj);
CParseJSONType cparse_json_type(CPARSE_JSON *obj);
int cparse_json_num_keys(CPARSE_JSON *obj);
bool cparse_json_contains(CPARSE_JSON *obj, const char *key);

CPARSE_JSON *cparse_json_remove(CPARSE_JSON *obj, const char *key);

CPARSE_JSON *cparse_json_tokenize(const char *str);

/* array setters */
void cparse_json_array_add_number(CPARSE_JSON *array, cparse_number value);
void cparse_json_array_add_real(CPARSE_JSON *array, double value);
void cparse_json_array_add_bool(CPARSE_JSON *array, bool value);
void cparse_json_array_add_string(CPARSE_JSON *array, const char *value);
void cparse_json_array_add(CPARSE_JSON *array, CPARSE_JSON *obj);

/* array getters */
cparse_number cparse_json_array_get_number(CPARSE_JSON *array, size_t index);
double cparse_json_array_get_real(CPARSE_JSON *array, size_t index);
const char *cparse_json_array_get_string(CPARSE_JSON *array, size_t index);
bool cparse_json_array_get_bool(CPARSE_JSON *array, size_t index);
CPARSE_JSON *cparse_json_array_get(CPARSE_JSON *array, size_t index);

size_t cparse_json_array_size(CPARSE_JSON *value);

/* converters */

cparse_number cparse_json_to_number(CPARSE_JSON *);
double cparse_json_to_real(CPARSE_JSON *);
bool cparse_json_to_bool(CPARSE_JSON *);
const char *cparse_json_to_string(CPARSE_JSON *value);

const char *cparse_json_to_json_string(CPARSE_JSON *value);

#define cparse_json_object_foreach_start(obj, key, val) do { \
char *key; struct json_object *val; struct lh_entry *entry;\
for(entry = json_object_get_object(obj)->head; entry && ( ( (key = (char*)entry->k)  || entry) && ( (val = (struct json_object*)entry->v)  || entry)); entry = entry->next )

#define cparse_json_object_foreach_end  } while(0);

#ifdef __cplusplus
}
#endif

#endif
