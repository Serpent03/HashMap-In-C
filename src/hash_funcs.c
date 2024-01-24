#include "common.h"
#include <math.h>
// all non-public functions are stored here

#define PRIME_1 151
#define PRIME_2 163

static KVPair* newKVpair(const char *k, const char *s) {
  KVPair *kvp = (KVPair*)calloc(1, sizeof(KVPair));
  kvp->key = strdup(k);
  kvp->value = strdup(k);
  return kvp;
}

void deleteKVPair(KVPair *kvp) {
  free(kvp->key);
  free(kvp->value);
  free(kvp);
}

HashMap* newHashMap() {
  HashMap *hm = (HashMap*)malloc(1 * sizeof(HashMap));
  hm->size = 53; 
  hm->count = 0;
  hm->items = (KVPair**)calloc((size_t)hm->size, sizeof(KVPair*));
  return hm;
}

void deleteHashMap(HashMap *hm) {
  // stored key-value pairs need to be deleted, otherwise we're causing
  // memory leaks with continued operations!
  for (int i = 0; i < hm->size; i++) {
    KVPair *kvp = hm->items[i];
    if (kvp != NULL) {
      deleteKVPair(kvp);
    }
  }
  free(hm->items);
  free(hm);
}

int hash_generator(const char *string, const int hashKey, const int size) {
  long hash = 0;
  const int len = strlen(string);
  for (int i = 0; i < len; i++) {
    hash += (long)pow(hashKey, len - (i+1)) * string[i];
    hash = hash % size; 
  }
  return hash;
}

// this deals with hashkey collisions. try combinations of two hash_generators
// added benefit is that we don't expose the actual hash function to end user
int get_hash(const char *string, const int size, const int attempt) {
  const int hash_a = hash_generator(string, PRIME_1, size);
  const int hash_b = hash_generator(string, PRIME_2, size);
  return hash_a + (attempt * (hash_b + 1)) % size;
}

void insertIntoHM(HashMap *a, char *k, char *v) {
  KVPair *kvp = newKVpair(k, v); 
  int index = get_hash(k, a->size, 0); 
  
  // check if there is a collision at the current index. if so, generate a new hash
  // and check again, until we find an empty slot.
  KVPair *current = a->items[index];
  int attempt = 1;
  while (current != NULL) {
    index = get_hash(k, a->size, attempt);
    current = a->items[index];
    attempt++;
  }
  a->items[index] = kvp;  
  a->count++;
}

char* searchInHM(HashMap *a, char *k) {
  int index = get_hash(k, a->size, 0);
  

  return NULL;
}










