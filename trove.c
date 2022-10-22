#include "trove.h"

void buildTroveFile(int argc, char **argv, ht_t *hashtable, int index) {
    int i = index;
    printf("INITIALISING BUILD TROVE.......\n");
    hashtable = ht_create(CAPACITY);
    printf("CREATED HASHTABLE\n");
    for (; i < argc; i++) {
        if (is_regular_file(argv[i])) charCounter(argv[i], hashtable);
        else listFiles(argv[i], hashtable);
    }
    pid_t p1 = fork();
    if (p1 == 0) {
        writeToFile(hashtable);
        return;
    }
    pid_t w_pid1 = wait(NULL);
    if(w_pid1 == -1) {
        exit(0);
    }
    if(w_pid1 > 0) {
        zipFile();
    }
}

void updateTroveFile(int argc, char **argv, ht_t *hashtable, int index) {
    printf("UPDATING......\n");
    int i = index;
    hashtable = ht_create(CAPACITY);
    unzipFile();
    readTroveFile(hashtable);
    for (; i < argc; i++) {
        if (is_regular_file(argv[i])) charCounter(argv[i], hashtable);
        else listFiles(argv[i], hashtable);
    }
    pid_t p1 = fork();
    if (p1 == 0) {
        writeToFile(hashtable);
        return;
    }
    pid_t w_pid1 = wait(NULL);
    if(w_pid1 == -1) {
        exit(0);
    }
    if(w_pid1 > 0) {
        zipFile();
    }
    
}

void removeFile(int argc, char **argv, ht_t *hashtable, int index) {
    unzipFile();
    int i = index;
    hashtable = ht_create(CAPACITY);
    readTroveFile(hashtable);
    

    ht_t *filedirs = ht_create(CAPACITY);
    for (; i < argc; i++) {
        printf("file: %s", argv[i]);
        if (is_regular_file(argv[i])) {
            ht_set(filedirs, argv[i], "nothing");
        }
        else {
            listFilesToList(argv[i], filedirs);
        }
    }

    for (int i = 0; i < CAPACITY; i++) {
        entry_t *entry = hashtable->entries[i];
        if(entry == NULL) continue;

        for(;;) {
            for (int x = 0; x < CAPACITY; x++) {
                entry_t *entrydirs = filedirs->entries[x];
                if (entrydirs == NULL) continue;
                if (strcmp(entry->value, entrydirs->key) == 0) {
                    entry->value = "nothing";
                }       
            }

            if(entry->next == NULL) {
                break;
            }
            entry = entry ->next;
        }
    }
    pid_t p1 = fork();
    if (p1 == 0) {
        writeToFile(hashtable);
        return;
    }
    pid_t w_pid1 = wait(NULL);
    if(w_pid1 == -1) {
        exit(0);
    }
    if(w_pid1 > 0) {
        zipFile();
    }

}

void searchWord(int argc, char **argv, ht_t *hashtable, int index) {
    unzipFile();
    printf("SEARCHING WORD.....\n");
    hashtable = ht_create(CAPACITY);
    readTroveFile(hashtable);
    ht_get(hashtable, argv[3]);
}

int main(int argc, char **argv) {

    int option_val = 0;
    int SelectFunction = 0;
    ht_t *HT = NULL;
    int index = 2;

    while ((option_val = getopt(argc, argv, "f:bru:l:")) != -1)
    {
        switch (option_val)
        {
        case 'f':
            changeTrovefilename(optarg);
            index = 4;
            break;
        case 'l':
            printf("changing length");
            changeWordLength(optarg);
            break;
        case 'b':
            SelectFunction = 1;
            break;
        case 'u':
            SelectFunction = 2;
            break;
        case 'r':
            SelectFunction = 3;
            break;
        default:
            return 1;
            break;
        }
    }    
    printf("SELECTED: %d\n", SelectFunction);

    if (SelectFunction == 0) {
        searchWord(argc, argv, HT , index);
    }
    else if(SelectFunction == 1) {
        buildTroveFile(argc, argv, HT, index);
    }
    else if (SelectFunction == 2) {
        updateTroveFile(argc, argv, HT, index);
    }
    else if (SelectFunction == 3) {
        printf("REMOVING.......");
        removeFile(argc, argv, HT, index);
    }

return 0;

}