#ifndef LRU_CACHE_H
#define LRU_CACHE_H
#include<stdio.h>
#include <stddef.h> 
#include <time.h>   
#include <sys/stat.h> 
#include<sys/types.h>
#include <stdbool.h> 
#define MAX_PATH_LEN 256
#define HASH_SIZE 64 

typedef struct FileMetadata {
    size_t size;          
    time_t last_modified; 
    ino_t inode;          
    time_t openedTime; 
} FileMetadata;

typedef struct LRUNode {
    char filepath[MAX_PATH_LEN]; 
    FileMetadata info;           
    struct LRUNode *prev;   
    struct LRUNode *next;   
    struct LRUNode *hash_next; 
} LRUNode;



typedef struct LRUCache {
    size_t capacity;          
    size_t size;              
    LRUNode *head;       
    LRUNode *tail;       
    LRUNode *hash_table[HASH_SIZE];
} LRUCache;


LRUCache* lru_create(size_t capacity);
bool lru_addentry(LRUCache *cache, const char *filepath, const FileMetadata *metadata);
bool lru_Search(LRUCache *cache, const char *filepath, FileMetadata *out_metadata);
bool lru_RemoveEntry(LRUCache *cache, const char *filepath);
void lru_Destroy(LRUCache *cache);
void Print_Cache(LRUCache *cache);

#endif 