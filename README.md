# Paroscale-Assignment2
I have successfully implemented the LRU cahe using doubly linked list and HashMap.Doubly linked list maints a access order having MOst REcently used Cache(MRU) at the head and LRU(Least recently used at the tail.THe hash maps provide O(1) average loop up time for cached items by mapping items with corresponding nodes in linked list.

# Command
gcc -o LRU main.c Assignment_2.c -Wall -Wextra -g
# To Run:
./LRU


# Output:Screenshot

![image alt](https://github.com/Ishaanyadav2004/Paroscale-Assignment2/blob/main/Screenshot%20from%202025-07-03%2020-20-05.png?raw=true)
![image alt](https://github.com/Ishaanyadav2004/Paroscale-Assignment2/blob/main/Screenshot%20from%202025-07-03%2020-20-13.png?raw=true)
![image alt](https://github.com/Ishaanyadav2004/Paroscale-Assignment2/blob/main/Screenshot%20from%202025-07-03%2020-20-23.png?raw=true)

# Output:
-- Adding entries to LRU cache------------------------
Attempting to add: '/etc/passwd'
  Successfully added/updated '/etc/passwd'.
--- LRU Cache Content (Size: 1/3) ---
  [1] Path:  '/etc/passwd', Size: 2870 bytes, Opened: 1751554191, Modified:  1751519328, Inode: 656902


Attempting to add: '/etc/hosts'
  Successfully added/updated '/etc/hosts'.
--- LRU Cache Content (Size: 2/3) ---
  [1] Path:  '/etc/hosts', Size: 234 bytes, Opened: 1751554191, Modified:  1751518892, Inode: 654474
  [2] Path:  '/etc/passwd', Size: 2870 bytes, Opened: 1751554191, Modified:  1751519328, Inode: 656902


Attempting to add: '/proc/cpuinfo'
  Successfully added/updated '/proc/cpuinfo'.
--- LRU Cache Content (Size: 3/3) ---
  [1] Path:  '/proc/cpuinfo', Size: 0 bytes, Opened: 1751554191, Modified:  1751553984, Inode: 4026532021
  [2] Path:  '/etc/hosts', Size: 234 bytes, Opened: 1751554191, Modified:  1751518892, Inode: 654474
  [3] Path:  '/etc/passwd', Size: 2870 bytes, Opened: 1751554191, Modified:  1751519328, Inode: 656902


Attempting to add: '/etc/fstab'
  Successfully added/updated '/etc/fstab'.
--- LRU Cache Content (Size: 3/3) ---
  [1] Path:  '/etc/fstab', Size: 473 bytes, Opened: 1751554191, Modified:  1751518828, Inode: 654482
  [2] Path:  '/proc/cpuinfo', Size: 0 bytes, Opened: 1751554191, Modified:  1751553984, Inode: 4026532021
  [3] Path:  '/etc/hosts', Size: 234 bytes, Opened: 1751554191, Modified:  1751518892, Inode: 654474


Attempting to add: '/var/log/dmesg'
  Successfully added/updated '/var/log/dmesg'.
--- LRU Cache Content (Size: 3/3) ---
  [1] Path:  '/var/log/dmesg', Size: 51121 bytes, Opened: 1751554191, Modified:  1751545176, Inode: 798235
  [2] Path:  '/etc/fstab', Size: 473 bytes, Opened: 1751554191, Modified:  1751518828, Inode: 654482
  [3] Path:  '/proc/cpuinfo', Size: 0 bytes, Opened: 1751554191, Modified:  1751553984, Inode: 4026532021


--- Searching for /etc/hosts (should move to MRU) ---
'/etc/hosts' not found in cache.
--- LRU Cache Content (Size: 3/3) ---
  [1] Path:  '/var/log/dmesg', Size: 51121 bytes, Opened: 1751554191, Modified:  1751545176, Inode: 798235
  [2] Path:  '/etc/fstab', Size: 473 bytes, Opened: 1751554191, Modified:  1751518828, Inode: 654482
  [3] Path:  '/proc/cpuinfo', Size: 0 bytes, Opened: 1751554191, Modified:  1751553984, Inode: 4026532021


--- Removing /proc/cpuinfo ---
Successfully removed '/proc/cpuinfo' from cache.
--- LRU Cache Content (Size: 2/3) ---
  [1] Path:  '/var/log/dmesg', Size: 51121 bytes, Opened: 1751554191, Modified:  1751545176, Inode: 798235
  [2] Path:  '/etc/fstab', Size: 473 bytes, Opened: 1751554191, Modified:  1751518828, Inode: 654482


--- Searching for a non-existent file ---
'/path/to/definitely_non_existent_file.xyz' not found in cache (as expected).
--- LRU Cache Content (Size: 2/3) ---
  [1] Path:  '/var/log/dmesg', Size: 51121 bytes, Opened: 1751554191, Modified:  1751545176, Inode: 798235
  [2] Path:  '/etc/fstab', Size: 473 bytes, Opened: 1751554191, Modified:  1751518828, Inode: 654482


--- Destroying the LRU cache ---
Cache Destroyed
Exiting................
