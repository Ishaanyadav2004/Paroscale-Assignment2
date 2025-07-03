#include "LRU_Cache.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h> 

static unsigned int HashFnc(const char *str) {
    unsigned int hash = 538132;
    int c;
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c; 
    }
    return hash;
}

static LRUNode* _create_node(const char *filepath, const FileMetadata *metadata) {
    LRUNode *newNode = (LRUNode*)malloc(sizeof(LRUNode));
    if (!newNode) {
        perror("Failed to allocaate");
        return NULL;
    }
    strncpy(newNode->filepath, filepath, MAX_PATH_LEN - 1);
    newNode->filepath[MAX_PATH_LEN - 1] = '\0'; 
    newNode->info = *metadata;
    newNode->prev = NULL;
    newNode->next = NULL;
    newNode->hash_next = NULL; 
    return newNode;
}


static void _lru_move_to_front(LRUCache *cache, LRUNode *node) {
    if (node == cache->head) {
        return;
    }
    if (node->prev) {
        node->prev->next = node->next;

    }
    if (node->next) {

        node->next->prev = node->prev;
    }

    if (node == cache->tail) {

        cache->tail = node->prev;
    }

    node->next = cache->head;
    node->prev = NULL; 
    if (cache->head) {
        cache->head->prev = node; 
    }
    cache->head = node; 
    if (!cache->tail) {
        cache->tail = node;
    }

}
 void _lru_add_to_front(LRUCache *cache, LRUNode *node) {
    node->next = cache->head;
    node->prev = NULL;
    if (cache->head) {
        cache->head->prev = node;
    }
    cache->head = node;

    if (!cache->tail) { 
        cache->tail = node;
    }
    cache->size++;
}


static void _lru_remove_node_from_dll(LRUCache *cache, LRUNode *node) {
    if (node->prev) {
        node->prev->next = node->next;
    } else { 
        cache->head = node->next;
    }

    if (node->next) {
        node->next->prev = node->prev;
    } else { 

        cache->tail = node->prev;
    }
    cache->size--;
}

static LRUNode* _hash_map_search(LRUCache *cache, const char *filepath) {
    unsigned int hash_val = HashFnc(filepath);
    unsigned int bucket_idx = hash_val % HASH_SIZE;
    LRUNode *current = cache->hash_table[bucket_idx];
    while (current != NULL) {
        if (strcmp(current->filepath, filepath) == 0) {
            return current; 
        }
        current = current->hash_next;
    }
    return NULL; 
}


static void _hash_map_add(LRUCache *cache, LRUNode *node) {
    unsigned int hash_val = HashFnc(node->filepath);
    unsigned int bucket_idx = hash_val % HASH_SIZE;
    node->hash_next = cache->hash_table[bucket_idx];
    cache->hash_table[bucket_idx] = node;
}

static bool _hash_map_remove(LRUCache *cache, const char *filepath) {
    unsigned int hash_val = HashFnc(filepath);
    unsigned int bucket_idx = hash_val % HASH_SIZE;

    LRUNode *current = cache->hash_table[bucket_idx];
    LRUNode *prevbucket = NULL;

    while (current != NULL) {
        if (strcmp(current->filepath, filepath) == 0) {
            if (prevbucket) {
            prevbucket->hash_next = current->hash_next;
            } else { 
                cache->hash_table[bucket_idx] = current->hash_next;
            }
            return true;
        }
        prevbucket = current;
        current = current->hash_next;
    }
    return false; 
}

LRUCache* lru_create(size_t capacity) {
    if (capacity == 0) {
        fprintf(stderr, "Error: LRU cache capacity cannot be zeroo........\n");
        return NULL;
    }

    LRUCache *cache = (LRUCache*)malloc(sizeof(LRUCache));
    if (!cache) {
        perror("Failed to allocate LRU cache structure");
        return NULL;
    }

    cache->capacity = capacity;
    cache->size = 0;
    cache->head = NULL;
    cache->tail = NULL;
    for (int i = 0; i < HASH_SIZE; ++i) {
        cache->hash_table[i] = NULL;
    }

    return cache;
}

bool lru_addentry(LRUCache *cache, const char *filepath, const FileMetadata *metadata) {
    if (!cache || !filepath || !metadata) {
        fprintf(stderr, "Error to LRU add entry\n");
        return false;
    }
    if (strlen(filepath) >= MAX_PATH_LEN) {
        fprintf(stderr, "Error: Filepath '%s' exceeds MAX_PATH_LEN (%d).\n", filepath, MAX_PATH_LEN);
        return false;
    }
   
    LRUNode *node_found = _hash_map_search(cache, filepath);
    if (node_found != NULL) { 
        node_found->info = *metadata;
        _lru_move_to_front(cache, node_found);
    } else { 
        if (cache->size >= cache->capacity) {
            if (cache->tail == NULL) { 
                fprintf(stderr, "Warning: Cache size mismatch, tail is NULL but cache is full.\n");
                return false;
            }
            LRUNode *lruNode = cache->tail; 
            
            _hash_map_remove(cache, lruNode->filepath);
            // Remove from Doubly linked list
            _lru_remove_node_from_dll(cache, lruNode);
            
            free(lruNode);
        }

        
        LRUNode *newNode = _create_node(filepath, metadata);
        if (!newNode) {
            return false; 
        }

        //Add node to MRU front of Doubly Linked List
        _lru_add_to_front(cache, newNode);

        // node to hashMap
        _hash_map_add(cache, newNode);
    }
    return true;
}

bool lru_Search(LRUCache *cache, const char *filepath, FileMetadata *out_metadata) {
    if (!cache || !filepath || !out_metadata) {
        fprintf(stderr, "Error: Invalid arguments to lru_search_entry.\n");
        return false;
    }

    LRUNode *node_found = _hash_map_search(cache, filepath);
    if (node_found != NULL) { // Found
        *out_metadata = node_found->info; // Copy metadata
        _lru_move_to_front(cache, node_found); // Move to front (MRU)
        return true;
    }
    // Not found
    return false;
}

bool lru_RemoveEntry(LRUCache *cache, const char *filepath) {
    if (!cache || !filepath) {
        fprintf(stderr, "Error to Remove LRU cache.\n");
        return false;
    }

    LRUNode *node_to_remove = _hash_map_search(cache, filepath);
    if (node_to_remove != NULL) { 
        // Remove from hash map
        _hash_map_remove(cache, filepath);
        _lru_remove_node_from_dll(cache, node_to_remove);

        // Free node memory
        free(node_to_remove);
        return true;
    }
    return false;
}

void lru_Destroy(LRUCache *cache) {
    if (!cache) {
        return;
    }
    LRUNode *current = cache->head;
    while (current) {
        LRUNode *nextNode = current->next; 
        free(current);
        current = nextNode;
    }
    free(cache);
}




void Print_Cache(LRUCache *cache) {
    if (!cache) {
        printf("Cache is NULL.\n");
        return;
    }
    printf("--- LRU Cache Content (Size: %zu/%zu) ---\n", cache->size, cache->capacity);
    if (cache->head == NULL) {
        printf("Cache is empty.\n");
    } else {
        LRUNode *current = cache->head;
        int count = 0;
        while (current != NULL) {
            printf("  [%d] Path:  '%s', Size: %zu bytes, Opened: %ld, Modified:  %ld, Inode: %lu\n",
                   ++count, current->filepath, current->info.size,
                   (long)current->info.openedTime, (long)current->info.last_modified, (unsigned long)current->info.inode);
            current = current->next;
        }
    }
    printf("\n");
}