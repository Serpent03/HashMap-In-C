#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

int getNextPrime(int base);

// with the way that our data storage works, a KeyValue pair makes a lot of sense:
// upon collision, we basically find a new index to store in. On retrieval, we may
// also get collision: which can be resolved by then comparing key and value to the 
// desired key and value, until we run out of candidates, I suppose
typedef struct KeyValuePair {
  char *key;
  char *value;
  bool isDeleted;
} KVPair;

// why exactly is this static?
static KVPair* newKVpair(const char *k, const char *v); 
void deleteKVPair(KVPair* kvp);

typedef struct HashMap {
  KVPair **items;
  int size;
  int count;
  float load;
  // load = count / size ;; load > 0.7 ? resize
} HashMap;

void resizeHashMap(HashMap *hm, int size);
int hash_generator(const char *string, const int hashKey, const int size);
int get_hash(const char *string, const int size, const int attempt);

/*** Public HashMap Functions ***/
HashMap* newHashMap();
void deleteHashMap(HashMap* hm);

char* searchInHM(HashMap *a, char *k);
void insertIntoHM(HashMap *a, char *k, char *v);
void deleteFromHM(HashMap *a, char *k);
