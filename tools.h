#ifndef _TOOLS_H_
#define _TOOLS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <ctype.h>
#include <stdbool.h>
#include <limits.h>
#include <sys/stat.h>
#include <getopt.h>

#endif

typedef struct entry_t {
    char *key;
    char *value;
    struct entry_t *next;
} entry_t;

typedef struct {
    entry_t **entries;
} ht_t;

typedef struct node {
    struct node *next;
    char *value;
} node;



// Define the Linkedlist here

#define CAPACITY 100000


ht_t *ht_create(int table_size);
void ht_set(ht_t *hashtable, char *key, char *value);
char *ht_get(ht_t *hashtable, char *key);
void ht_del(ht_t *hashtable, char *key);
void ht_dump(ht_t *hashtable);

extern void createTroveFile(char *filedir, char *filepaths);

extern void printList(node *head);
extern void push(node **head_ref, char *string);
extern char *fileTrove;
extern void readTroveFile(ht_t *hashtable);
extern void changeTrovefilename(char *filename);
extern void writeToFile(ht_t *hashtable);
extern bool is_regular_file(const char *path);
extern char *getLine(FILE *file);
extern bool isFileBinary(char *file);
extern void copyfile(char source[]);
extern int *readImage(char *file);
extern void ReadBinFile(char *file);
extern char *stringAppend(char *string1, char *string2);
extern int getFileSize(char *file);
extern void listFiles(char *dirname, ht_t *hashtable);
extern FILE *openFile(char *file);
extern char *ReadFile(char *file);
extern int wordCounter(char *file);
extern int charCounter(char *file, ht_t *hashtable);
extern void changeWordLength(char *length);
extern void listFilesToList(char *dirname, ht_t *hashtable);
extern void zipFile();
extern void unzipFile();