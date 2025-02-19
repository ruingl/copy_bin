#ifndef DATABASE_H
#define DATABASE_H

#include <stdlib.h>
#include "cJSON.h"

char *read_database(void);
int seed_database(void);
cJSON *init_database(void);
char *get_id(size_t length);
char *get_bin(const char *id);
int create_bin(const char *bin);

#endif // DATABASE_H
