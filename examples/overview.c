#include <leveldb/c.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
typedef struct _pair{
    char *key;
    char *value;
} pair;
#define array_size(arr) (sizeof(arr)/sizeof(arr[0]))
int main() {
    leveldb_t *db;
    leveldb_options_t *options;
    leveldb_readoptions_t *roptions;
    leveldb_writeoptions_t *woptions;
    char *err = NULL;
    char *read;
    size_t read_len;

    /******************************************/
    /* OPEN */

    options = leveldb_options_create();
    leveldb_options_set_create_if_missing(options, 1);
    db = leveldb_open(options, "./testdb", &err);

    if (err != NULL) {
        fprintf(stderr, "Open fail.\n");
        return(1);
    }

    /* reset error var */
    leveldb_free(err); err = NULL;

    /******************************************/
    pair p[] = {
        {"k1","v1"},
        {"k2","v2"},
        {"k3","v3"},
        {"k4","v4"}
    }; 
    /* WRITE */
    woptions = leveldb_writeoptions_create();

    for(int i = 0; i < array_size(p); i ++) {
        leveldb_put(db, woptions, p[i].key, strlen(p[i].key), p[i].value, strlen(p[i].value), &err);

        if (err != NULL) {
            fprintf(stderr, "Write fail.\n");
            return(1);
        }

        leveldb_free(err); err = NULL;
    }

    /******************************************/
    /* READ */

    roptions = leveldb_readoptions_create();
    printf("key:value\n");
    for(int i = 0; i < array_size(p); i ++) {
        read = leveldb_get(db, roptions, p[i].key, strlen(p[i].key), &read_len, &err);

        if (err != NULL) {
            fprintf(stderr, "Read fail.\n");
            return(1);
        }

        *(read + read_len) = '\0';
        printf("%s:%s\n", p[i].key, read);
        free(read);

        leveldb_free(err); err = NULL;
    }
    /******************************************/
    /* DELETE */

    leveldb_delete(db, woptions, "key", 3, &err);

    if (err != NULL) {
        fprintf(stderr, "Delete fail.\n");
        return(1);
    }

    leveldb_free(err); err = NULL;

    /******************************************/
    /* CLOSE */

    leveldb_close(db);

    /******************************************/
    /* DESTROY */
#if 0
    leveldb_destroy_db(options, "testdb", &err);

    if (err != NULL) {
        fprintf(stderr, "Destroy fail.\n");
        return(1);
    }

    leveldb_free(err); err = NULL;
#endif

    return(0);
}
