#include "tools.h"

#define CHUNK 1024

char *fileTrove = "/tmp/trove";
int word_length = 4;
FILE *pfileTrove = NULL;

FILE *openFile(char *file) //
{   // Function to open the file, assign a pointer to the file and raise an error if unable to open the file
    FILE *pFile = fopen(file, "r");

    if (pFile == NULL)
    {
        printf("error loading file '%s'\n", file);
        perror("fopen");
        exit(EXIT_FAILURE);
    }
    return pFile;
}

void changeTrovefilename(char *filename) {//
    fileTrove = strdup(filename);
}

void changeWordLength(char *length) {
    printf("OLD Word Length is: %d\n", word_length);
    int num = atoi(length);
    word_length = num;
    printf("NEW Word Length is: %d\n", word_length);
}

bool isFileBinary(char *file) {
    FILE *pFile = openFile(file);
    char ch;
    while ((ch = fgetc(pFile)) != EOF ) {
        if ((!isascii(ch) || iscntrl(ch)) && !isspace(ch)) {
            return true;
        }
    }
    return false;
}

int getFileSize(char *file) {
    FILE *pFile = openFile(file);
    fseek(pFile, 0, SEEK_END);
    int size = ftell(pFile);
    fseek(pFile, 0, SEEK_SET);
    fclose(pFile);
    return size;
}

bool is_regular_file(const char *path)
{
    struct stat path_stat;
    stat(path, &path_stat);
    return S_ISREG(path_stat.st_mode);
}

void push(node **head_ref, char *string) {
    node *new_list = (node*)malloc(sizeof(node));

    new_list->value = string;
    new_list->next = (*head_ref);
    (*head_ref) = new_list;

}

bool search(node* head, char *string)
{
    node* current = head; // Initialize current
    //printf("\nSEARCHING TABLE\n");
    while (current != NULL) {
        //printf("%s\n", current->value);
        if (strcmp(current->value, string) == 0){
            return true;
        }
        current = current->next;
    }
    return false;
}

void writeToFile(ht_t *hashtable) {
    printf("\t writing trove-file\n");
    char* writeFile = fileTrove;
    const char *TABLE_FORMAT_OUT = "%d,%s,";
    int words = 0;
    FILE *file = fopen(writeFile, "w+");
    node *head = NULL;
    if (file == NULL) {
        return;
    }
    for (int i = 0; i < CAPACITY; ++i) {
        entry_t *entry = hashtable->entries[i];

        if (entry == NULL) {
            continue;
        }
        fprintf(file, TABLE_FORMAT_OUT, i, entry->key);
        words++;
        /*
        */
        while (entry != NULL) {
            if (search(head, entry->value) != true && strcmp(entry->value, " ") != 0) {
                push(&head, entry->value);
            }
            if (strcmp(entry->value, " ") == 0) {
                fprintf(file, "%s", "nothing");
            }
            entry = entry->next;
        }
        fprintf(file, "(");
        while (head != NULL) {
            if (head->next == NULL) {
                fprintf(file, "%s", head->value);
            }
            else {
            fprintf(file, "%s,", head->value);
            }
            head = head->next;
        }
        fprintf(file, ")");
        fprintf(file, "\n");
    }
    printf("\t %d unique words\n", words);
}

int charCounter(char *file, ht_t *hashtable) {
    FILE *pfile = openFile(file);
    char ch;
    int words, i;
    char curr_word[1000];
    /*
     * Logic to count characters, words and lines.
     */
    words = i = 0;
    printf("\t finding words in '%s'\n", file);
    if (isFileBinary(file) != true) {
        int filesize = getFileSize(file);
        char *curr_word =(char *)malloc(1);
        for (int i = 0; i < filesize; i++)
        {
            ch = fgetc(pfile);
            /* Check words */
            if (isalnum(ch) == false) {

                if (strlen(curr_word) >= word_length)
                {
                    words++;
                    //printf("\n");
                    //printf("index: %d\t%s\t%s\n", words, curr_word, file);
                    ht_set(hashtable, curr_word, file);
                }
                curr_word[0] = 0;

                }
            if (isalnum(ch) == true) {
                
                strncat(curr_word, &ch, 1);
                }
        }
    }
    if (isFileBinary(file) == true) {
        int filesize = getFileSize(file);
        
        for (int i = 0; i < filesize; i++) {
            ch = fgetc(pfile);
            /* Check words */
            if (isalnum(ch) == false) {

                if (strlen(curr_word) >= 4)
                {
                    words++;
                    //printf("\n");
                    //printf("index: %d\t%s\t%s\n", words, curr_word, file);
                    ht_set(hashtable, curr_word, file);
                }
                strcpy(curr_word, "");

                }
            if (isalnum(ch) == true) {
                strncat(curr_word, &ch, 1);
                }
        }
    }
    /* Close files to release resources */
    fclose(pfile);
    printf("\t found %d\n", words);

    return 0;
}

void listFiles(char *dirname, ht_t *hashtable) {
    printf("\t reading directory '%s'\n", dirname);
    DIR* dir = opendir(dirname);
    if (dir == NULL) {
        return;
    }
    char directory[1024];
    //printf(" -----Reading files in: %s\n", dirname);

    struct dirent* entity;
    entity = readdir(dir);
    while (entity != NULL) {
        if (entity -> d_type == DT_REG && strstr(entity->d_name, ".DS") == NULL) 
        {
            //printf("%s/%s\t", dirname, entity->d_name);
            snprintf(directory, sizeof(directory), "%s/%s", dirname, entity->d_name);
            charCounter(directory, hashtable);
        }

        if (entity->d_type == DT_DIR && strcmp(entity->d_name, ".") != 0 && strcmp(entity->d_name, "..") != 0) {
            char path[100] = { 0 };
            strcat(path, dirname);
            strcat(path, "/");
            strcat(path, entity->d_name);
            listFiles(path, hashtable);
        }
        entity = readdir(dir);
    }
    closedir(dir);
    
}

void listFilesToList(char *dirname, ht_t *hashtable) {
    DIR* dir = opendir(dirname);
    if (dir == NULL) {
        return;
    }
    char directory[1024];
    //printf(" -----Reading files in: %s\n", dirname);

    struct dirent* entity;
    entity = readdir(dir);
    while (entity != NULL) {
        if (entity -> d_type == DT_REG && strstr(entity->d_name, ".DS") == NULL) 
        {
            printf("START DEBUG.......");
            //printf("%s/%s\t", dirname, entity->d_name);
            snprintf(directory, sizeof(directory), "%s/%s", dirname, entity->d_name);
            ht_set(hashtable, directory, "nothing");
            printf("END DEBUG.......");
        }

        if (entity->d_type == DT_DIR && strcmp(entity->d_name, ".") != 0 && strcmp(entity->d_name, "..") != 0) {
            char path[1024] = { 0 };
            strcat(path, dirname);
            strcat(path, "/");
            strcat(path, entity->d_name);
            listFilesToList(path, hashtable);
        }
        entity = readdir(dir);
    }

    closedir(dir);
    
}

void printList(node *head) {
    printf("\n------LINKEDLIST------\n");
    while (head != NULL) {
        printf("\n%s\n", head->value);
        head = head->next;
    }
}

bool checkIfInList(node *head, char *string) {
    while(head != NULL) {
        if(strcmp(head->value, string) == true) {
            printf("list = %s", string);
            return true;
        }
        head = head->next;
    }
    return false;
}

unsigned int hash_function(const char *str)
{
    unsigned long int value = 0;
    unsigned int i = 0;
    unsigned int key_len = strlen(str);

    // do several rounds of multiplication
    for (; i < key_len; ++i) {
        value = value * 37 + str[i];
    }

    // make sure value is 0 <= value < TABLE_SIZE
    value = value % CAPACITY;

    return value;
}

entry_t *ht_pair(char *key, char *value) {
    // allocate the entry
    entry_t *entry = malloc(sizeof(entry_t) * 1);
    entry->key = malloc(strlen(key) + 1);
    entry->value = malloc(strlen(value) + 1);

    // copy the key and value in place
    strcpy(entry->key, key);
    strcpy(entry->value, value);

    // next starts out null but may be set later on
    entry->next = NULL;

    return entry;
}

ht_t *ht_create(int table_size) {
    // allocate table
    ht_t *hashtable = malloc(sizeof(ht_t) * 1);

    // allocate table entries
    hashtable->entries = malloc(sizeof(entry_t*) * table_size);

    // set each to null (needed for proper operation)
    int i = 0;
    for (; i < table_size; ++i) {
        hashtable->entries[i] = NULL;
    }

    return hashtable;
}

void removeChar(char *str, char garbage) {

    char *src, *dst;
    for (src = dst = str; *src != '\0'; src++) {
        *dst = *src;
        if (*dst != garbage) dst++;
    }
    *dst = '\0';
}

void ht_set_using_slot(ht_t *hashtable, char *key, char *value, unsigned int slot) {
    char *token;
    // try to look up an entry set

    removeChar(value, '(');
    removeChar(value, ')');
    // no entry means slot empty, insert immediately
    char *duped = strdup(value);
    while((token = strsep(&duped, ",")) != NULL) {
        ht_set(hashtable, key, token);
    }
}

void ht_set(ht_t *hashtable, char *key, char *value) {
    unsigned int slot = hash_function(key);

    // try to look up an entry set
    entry_t *entry = hashtable->entries[slot];

    // no entry means slot empty, insert immediately
    if (entry == NULL) {
        hashtable->entries[slot] = ht_pair(key, value);
        return;
    }

    entry_t *prev;

    // walk through each entry until either the end is
    // reached or a matching key is found
    while (entry != NULL) {
        // walk to next
        prev = entry;
        entry = prev->next;
    }

    // end of chain reached without a match, add new
    prev->next = ht_pair(key, value);
}

char *ht_get(ht_t *hashtable, char *key) {
    unsigned int slot = hash_function(key);
    node *head = NULL;
    // try to find a valid slot
    entry_t *entry = hashtable->entries[slot];

    // no slot means no entry
    if (entry == NULL) { 
        printf("no entry");
        return NULL;
    }
    // walk through each entry in the slot, which could just be a single thing
    while (entry != NULL) {
        // proceed to next key if available
        if (search(head, entry->value) != true) {
            push(&head, entry->value);
        }
        entry = entry->next;
        
    }
    printList(head);
    // reaching here means there were >= 1 entries but no key match

    return NULL;
}

void ht_del(ht_t *hashtable,   char *key) {
    unsigned int bucket = hash_function(key);

    // try to find a valid bucket
    entry_t *entry = hashtable->entries[bucket];

    // no bucket means no entry
    if (entry == NULL) {
        return;
    }

    entry_t *prev;
    int idx = 0;

    // walk through each entry until either the end is reached or a matching key is found
    while (entry != NULL) {
        // check key
        if (strcmp(entry->key, key) == 0) {
            // first item and no next entry
            if (entry->next == NULL && idx == 0) {
                hashtable->entries[bucket] = NULL;
            }

            // first item with a next entry
            if (entry->next != NULL && idx == 0) {
                hashtable->entries[bucket] = entry->next;
            }

            // last item
            if (entry->next == NULL && idx != 0) {
                prev->next = NULL;
            }

            // middle item
            if (entry->next != NULL && idx != 0) {
                prev->next = entry->next;
            }

            // free the deed entry
            free(entry->key);
            free(entry->value);
            free(entry);

            return;
        }

        // walk to next
        prev = entry;
        entry = prev->next;

        ++idx;
    }
}

void ht_dump(ht_t *hashtable) {
    printf("\n--------HASHTABLE---------\n");
    for (int i = 0; i < CAPACITY; ++i) {
        entry_t *entry = hashtable->entries[i];

        if (entry == NULL) {
            continue;
        }

        printf("slot[%4d] key: %s\t", i, entry->key);

        for(;;) {
            printf("\t%s ", entry->value);

            if (entry->next == NULL) {
                break;
            }

            entry = entry->next;
        }

        printf("\n");
    }
}

void readTroveFile(ht_t *hashtable) {
    FILE *tFile = pfileTrove;

    do {
        int index;
        char word[ARG_MAX];
        char filedir[ARG_MAX];
        fscanf(tFile, "%d,%100[^,],%s\n", &index, word, filedir);
        ht_set_using_slot(hashtable, word, filedir, index);
    } while (!feof(tFile));
}

void zipFile() {
    printf("\t compressing trove-file\n");
    char *filename = strdup(fileTrove);
    char *command = (char *)malloc(sizeof(char) * ARG_MAX);
    sprintf(command, "gzip -f '%s'", filename);
    system(command);
}

void readzipFile() {
    char *filename = strdup(fileTrove);
    char *command = (char *)malloc(sizeof(char) * ARG_MAX);
    sprintf(command, "zcat < '%s.gz'", filename);
    pfileTrove = popen(command, "r");
}