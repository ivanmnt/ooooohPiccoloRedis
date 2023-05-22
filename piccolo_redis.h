#ifndef PICCOLO_REDIS_H
#define PICCOLO_REDIS_H

#define MAX_KEY_SIZE 50
#define MAX_VALUE_SIZE 200

struct KeyValue {
    char key[MAX_KEY_SIZE];
    char value[MAX_VALUE_SIZE];
};

void set(const char* key, const char* value);
char* get(const char* key);
void del(const char* key);

#endif
