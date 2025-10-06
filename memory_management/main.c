#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct SnekObject snek_object_t;

typedef enum { INTEGER, FLOAT, STRING, VECTOR3, ARRAY } snek_object_kind_t;

typedef struct {
  snek_object_t *x;
  snek_object_t *y;
  snek_object_t *z;
} snek_vector_t;

typedef struct {
  size_t size;
  snek_object_t **elements;
} snek_array_t;

typedef union {
  int v_int;
  float v_float;
  char *v_string;
  snek_vector_t v_vector3;
  snek_array_t v_array;
} snek_object_data_t;

typedef struct SnekObject {
  snek_object_kind_t kind;
  snek_object_data_t data;
} snek_object_t;

typedef struct {
  size_t count;
  size_t capacity;
  void **data;
} stack_t;

snek_object_t *new_snek_integer(int value) {
  snek_object_t *obj = malloc(sizeof(snek_object_t));
  if (obj == NULL) {
    return NULL;
  }

  obj->kind = INTEGER;
  obj->data.v_int = value;

  return obj;
}

snek_object_t *new_snek_float(float value) {
  snek_object_t *obj = malloc(sizeof(snek_object_t));
  if (obj == NULL) {
    return NULL;
  }

  obj->kind = FLOAT;
  obj->data.v_float = value;

  return obj;
}

snek_object_t *new_snek_string(char *value) {
  snek_object_t *obj = malloc(sizeof(snek_object_t));
  if (obj == NULL) {
    return NULL;
  }

  obj->kind = STRING;
  obj->data.v_string = malloc(strlen(value) + 1);
  if (obj->data.v_string == NULL) {
    free(obj);
    return NULL;
  }
  strcpy(obj->data.v_string, value);

  return obj;
}

snek_object_t *new_snek_vector3(snek_object_t *x, snek_object_t *y,
                                snek_object_t *z) {
  if (x == NULL || y == NULL || z == NULL) {
    return NULL;
  }

  snek_object_t *obj = malloc(sizeof(snek_object_t));
  if (obj == NULL) {
    return NULL;
  }

  obj->kind = VECTOR3;
  obj->data.v_vector3.x = x;
  obj->data.v_vector3.y = y;
  obj->data.v_vector3.z = z;

  return obj;
}

snek_object_t *new_snek_array(size_t size) {
  snek_object_t *obj = malloc(sizeof(snek_object_t));
  if (obj == NULL) {
    return NULL;
  }

  obj->kind = ARRAY;

  snek_object_t **elements = calloc(size, sizeof(snek_object_t *));
  if (elements == NULL) {
    return NULL;
  }

  snek_array_t array = {.size = size, .elements = elements};
  obj->data.v_array = array;

  return obj;
}

bool snek_array_set(snek_object_t *snek_obj, size_t index,
                    snek_object_t *value) {
  if (snek_obj == NULL || value == NULL || snek_obj->kind != ARRAY ||
      snek_obj->data.v_array.size <= index) {
    return false;
  }

  snek_obj->data.v_array.elements[index] = value;

  return true;
}

snek_object_t *snek_array_get(snek_object_t *snek_obj, size_t index) {
  if (snek_obj == NULL || snek_obj->kind != ARRAY ||
      snek_obj->data.v_array.size <= index) {
    return NULL;
  }

  return snek_obj->data.v_array.elements[index];
}

int snek_length(snek_object_t *obj) {
  if (obj == NULL) {
    return -1;
  }

  switch (obj->kind) {
  case INTEGER:
    return 1;
  case FLOAT:
    return 1;
  case STRING:
    return strlen(obj->data.v_string);
  case VECTOR3:
    return 3;
  case ARRAY:
    return obj->data.v_array.size;
  default:
    return -1;
  }
}

snek_object_t *snek_add(snek_object_t *a, snek_object_t *b) {
  if (a == NULL || b == NULL) {
    return NULL;
  }

  switch (a->kind) {

  case INTEGER: {
    switch (b->kind) {
    case INTEGER:
      return new_snek_integer(a->data.v_int + b->data.v_int);
    case FLOAT:
      return new_snek_float((float)a->data.v_int + b->data.v_int);
    default:
      return NULL;
    }
  }

  case FLOAT: {
    switch (b->kind) {
    case INTEGER:
      return new_snek_float(a->data.v_int + (float)b->data.v_int);
    case FLOAT:
      return new_snek_float(a->data.v_int + b->data.v_int);
    default:
      return NULL;
    }
  }

  case STRING: {
    if (b->kind != STRING) {
      return NULL;
    }
    size_t length = strlen(a->data.v_string) + strlen(b->data.v_string) + 1;
    char *newstring = malloc(sizeof(char) * length);
    strcat(newstring, a->data.v_string);
    strcat(newstring, b->data.v_string);
    snek_object_t *obj = new_snek_string(newstring);
    free(newstring);
    return obj;
  }

  case VECTOR3: {
    if (b->kind != VECTOR3) {

      return NULL;
    }
    return new_snek_vector3(snek_add(a->data.v_vector3.x, b->data.v_vector3.x),
                            snek_add(a->data.v_vector3.y, b->data.v_vector3.y),
                            snek_add(a->data.v_vector3.z, b->data.v_vector3.z));
  }

  case ARRAY: {
    if (b->kind != ARRAY) {
      return NULL;
    }
    size_t length = a->data.v_array.size + b->data.v_array.size;
    snek_object_t *obj = new_snek_array(length);

    for (size_t i = 0; i < a->data.v_array.size; i++) {
      snek_array_set(obj, i, snek_array_get(a, i));
    }

    for (size_t i = 0; i < b->data.v_array.size; i++) {
      snek_array_set(obj, i + a->data.v_array.size, snek_array_get(b, i));
    }

    return obj;
  }

  default:
    return NULL;
  }
}

void stack_free(stack_t *stack) {
  printf("free stack\n");

  if (stack == NULL) {
    return;
  }
  if (stack->data != NULL) {
    free(stack->data);
  }

  free(stack);
}

void *stack_pop(stack_t *stack) {
  if (stack->count == 0) {
    return NULL;
  }
  stack->count--;
  return stack->data[stack->count];
}

void stack_push(stack_t *stack, void *obj) {
  if (stack->count == stack->capacity) {
    printf("out of capacity\n");

    stack->capacity *= 2;

    void **temp = realloc(stack->data, sizeof(void *) * stack->capacity);
    if (temp == NULL) {
      return;
    }

    stack->data = temp;
  }

  stack->data[stack->count] = obj;
  stack->count++;
}

stack_t *stack_new(size_t capacity) {
  printf("create stack\n");

  stack_t *s = malloc(sizeof(stack_t));
  if (s == NULL) {
    return NULL;
  }

  s->count = 0;
  s->capacity = capacity;
  s->data = malloc(sizeof(void *) * capacity);
  if (s->data == NULL) {
    free(s);
    return NULL;
  }

  return s;
}

int main(int argc, char *argv[]) {
  for (int i = 0; i < argc; i++) {
    printf("%d, %s\n", i, argv[i]);
  }

  // ========================
  // Stack
  // ========================

  stack_t *stack = stack_new(1);
  char *test = "testing";

  for (int i = 0; i < 3; i++) {
    stack_push(stack, test);
    printf("count[%ld], cap[%ld]\n", stack->count, stack->capacity);
  }

  for (int i = 0; i < 3; i++) {
    char *value = stack_pop(stack);
    printf("value[%s], count[%ld], cap[%ld]\n", value, stack->count,
           stack->capacity);
  }

  stack_free(stack);

  // ========================
  // Object
  // ========================

  snek_object_t *obj = new_snek_integer(42);
  printf("obj data[%d]\n", obj->data.v_int);
  free(obj);
  obj = new_snek_float(42.2);
  printf("obj data[%f]\n", obj->data.v_float);
  free(obj);
  obj = new_snek_array(3);
  printf("obj data.size[%ld]\n", obj->data.v_array.size);
  snek_array_set(obj, 0, obj);
  snek_object_t *objj = snek_array_get(obj, 0);
  printf("objj data.size[%ld], len[%d]\n", objj->data.v_array.size,
         snek_length(objj));
  free(obj);

  obj = new_snek_string("hello");
  objj = new_snek_string(" world");
  snek_object_t *objjj = snek_add(obj, objj);
  printf("obj data[%s]\n", objjj->data.v_string);
  free(obj);
  free(objj);
  free(objjj);

  return 0;
}
