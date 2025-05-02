#ifndef clox_object_h
#define clox_object_h

#include "common.h"
#include "value.h"
ObjString* copyString(const char* chars, int length);
void printObject(Value value);
ObjString* takeString(char* chars, int length);

typedef enum
{
    OBJ_STRING
} ObjType;

typedef struct Obj
{
    struct Obj *next; // 指向下一个对象
    ObjType type;
} Obj;

typedef struct ObjString
{
    Obj obj; // 为了向上转型成 Obj
    int length;
    char *chars;
    uint32_t hash;
} ObjString;

static inline bool isObjType(Value value, ObjType type)
{
    return IS_OBJ(value) && AS_OBJ(value)->type == type;
}

#define OBJ_TYPE(value) (AS_OBJ(value)->type)
#define IS_STRING(value) isObjType(value, OBJ_STRING)

#define AS_STRING(value) ((ObjString *)AS_OBJ(value))
#define AS_CSTRING(value) (((ObjString *)AS_OBJ(value))->chars)

#endif
