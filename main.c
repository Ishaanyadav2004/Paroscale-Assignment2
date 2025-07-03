#include <stdio.h>
#include <time.h>    
#include <sys/stat.h> 
#include <unistd.h>  
#include <errno.h>  
#include <string.h>  
#include "LRU_Cache.h" // included my library


bool GetInfo(const char *filepath, FileMetadata *metadata) {
    struct stat st;
    if (stat(filepath, &st) == -1) {
        fprintf(stderr, "Error :Could not stat file '%s': %s.\n", filepath, strerror(errno));
        return false;
    }
    metadata->size = st.st_size;
    metadata->last_modified = st.st_mtime;
    metadata->inode = st.st_ino;
    metadata->openedTime = time(NULL); 
    return true;
}

int main() {
    LRUCache *my_lru_cache = lru_create(3); 
    if (!my_lru_cache) {
        fprintf(stderr, "Falied to create LRU cache\n");
        return 1;
    }
    FileMetadata file_info;
    bool operationSucess;
    printf("--- Adding entries to LRU cache------------------------\n");

    const char* paths_to_process[] = {
        //Common linux files
        "/etc/passwd",
        "/etc/hosts",
        "/proc/cpuinfo", 
        "/etc/fstab",   
        "/var/log/dmesg" 
    };

    for (size_t i = 0; i < sizeof(paths_to_process) / sizeof(paths_to_process[0]); ++i) {
        const char* current = paths_to_process[i];
        printf("Attempting to add: '%s'\n", current);
        if (GetInfo(current, &file_info)) {
        operationSucess = lru_addentry(my_lru_cache, current, &file_info);
            if (operationSucess) {
                printf("  Successfully added/updated '%s'.\n", current);
            } else {
                fprintf(stderr, "  Failed to add/update '%s' in cache.\n", current);
            }
        } else {
           
            fprintf(stderr, "  Skipping '%s' as metadata could not be retireved...\n", current);
        }
        Print_Cache(my_lru_cache);
        printf("\n");
    }

    printf("--- Searching for /etc/hosts (should move to MRU) ---\n");
    operationSucess = lru_Search(my_lru_cache, "/etc/hosts", &file_info);
    if (operationSucess) {
    printf("Found '/etc/hosts': Size: %zu bytes, Opened at: %ld\n", file_info.size, (long)file_info.openedTime);
    } else {   printf("'/etc/hosts' not found in cache.\n");
    }
Print_Cache(my_lru_cache);
    printf("\n");

    printf("--- Removing /proc/cpuinfo ---\n");
    operationSucess = lru_RemoveEntry(my_lru_cache, "/proc/cpuinfo");
    if (operationSucess) {
        printf("Successfully removed '/proc/cpuinfo' from cache.\n");
    } else {
        printf("'/proc/cpuinfo' not found in cache for removal.\n");
    }
    Print_Cache(my_lru_cache);
    printf("\n");

    printf("--- Searching for a non-existent file ---\n");
    operationSucess = lru_Search(my_lru_cache, "/path/to/definitely_non_existent_file", &file_info);
    if (operationSucess) {
        printf("Found '/path/to/definitely_non_existent_file.xyz' (unexpected).\n");
    } else {
        printf("'/path/to/definitely_non_existent_file.xyz' not found in cache (as expected).\n");
    }
    Print_Cache(my_lru_cache); 
    printf("\n");


    printf("--- Destroying the LRU cache ---\n");
    lru_Destroy(my_lru_cache);
    printf("Cache Destroyed\n");
    printf("Exiting................");

    return 0;
}