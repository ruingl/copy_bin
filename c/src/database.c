#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "cJSON.h"

char *read_database() {
    FILE *file = fopen("database.json", "r");
    if (!file) return NULL;

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    rewind(file);

    char *json_data = malloc(length + 1);
    if (!json_data) {
        fclose(file);
        return NULL;
    }

    if (fread(json_data, 1, length, file) != (size_t)length) {
        free(json_data);
        fclose(file);
        return NULL;
    }

    json_data[length] = '\0';
    fclose(file);
    return json_data;
}

int seed_database() {
    FILE *file = fopen("database.json", "w");
    if (!file) return 0;

    cJSON *json = cJSON_CreateObject(); // {}
    cJSON *bins = cJSON_AddObjectToObject(json, "bins"); // { "bins": {} }
    char *json_str = cJSON_PrintUnformatted(json);

    if (!json_str) {
        fclose(file);
        cJSON_Delete(json);
        return 0;
    }

    fprintf(file, "%s", json_str);
    free(json_str);

    fclose(file);
    cJSON_Delete(json);

    return 1;
}

cJSON *init_database() {
    char *json_str = read_database();
    if (!json_str) {
        if (seed_database() == 1) {
            // reread db after seed.
            json_str = read_database();
        }
        if (!json_str) return NULL;
    }

    cJSON *json = cJSON_Parse(json_str);
    free(json_str);

    if (!json) {
        if (seed_database() == 1) {
            // reread db after seed.
            json_str = read_database();
            if (json_str) {
                json = cJSON_Parse(json_str);
                free(json_str);
            }
        }
    }

    return json;
}

char *get_id(size_t length) {
    static const char charset[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890";
    size_t charset_size = sizeof(charset) - 1;
    char *str = malloc(length + 1);
    if (!str) return NULL;

    for (size_t i = 0; i < length; i++) {
        str[i] = charset[rand() % charset_size];
    }

    str[length] = '\0';
    return str;
}

char *get_bin(const char *id) {
    cJSON *json = init_database();
    if (!json) return NULL;

    if (!id) {
        cJSON_Delete(json);
        return strdup("Missing parameters!");
    }

    cJSON *bins = cJSON_GetObjectCaseSensitive(json, "bins");
    cJSON *bin = cJSON_GetObjectItemCaseSensitive(bins, id);
    char *result;

    if (cJSON_IsString(bin)) {
        result = strdup(bin->valuestring);
    } else {
        result = strdup("Not Found");
    }

    cJSON_Delete(json);
    return result;
}

int create_bin(const char *bin) {
    if (!bin) return 0;

    cJSON *json = init_database();
    if (!json) return 0;

    cJSON *bins = cJSON_GetObjectCaseSensitive(json, "bins");
    if (!bins) {
        cJSON_Delete(json);
        return 0;
    }

    char *id;
    do {
        id = get_id(8);
    } while (cJSON_GetObjectItemCaseSensitive(bins, id) != NULL);

    cJSON_AddItemToObject(bins, id, cJSON_CreateString(bin));

    FILE *file = fopen("database.json", "w");
    if (!file) {
        free(id);
        cJSON_Delete(json);
        return 0;
    }

    char *json_str = cJSON_PrintUnformatted(json);
    fprintf(file, "%s", json_str);

    fclose(file);
    free(json_str);
    free(id);
    cJSON_Delete(json);

    return 1;
}
