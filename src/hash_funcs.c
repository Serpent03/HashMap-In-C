#include "common.h"
#include <math.h>
// all non-public functions are stored here

#define PRIME_1 151
#define PRIME_2 163

static KVPair* newKVpair(const char *k, const char *v) {
  KVPair *kvp = (KVPair*)calloc(1, sizeof(KVPair));
  kvp->key = strdup(k);
  kvp->value = strdup(v);
  kvp->isDeleted = false;
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

// TODO automatically resizing the items KVPair array
void insertIntoHM(HashMap *a, char *k, char *v) {
  KVPair *kvp = newKVpair(k, v); 
  int index = get_hash(k, a->size, 0); 
  // check if there is a collision at the current index. if so, generate a new hash
  // and check again, until we find an empty slot.
  KVPair *current = a->items[index];
  int attempt = 1;

  // if the node we're at is 'deleted', then we can insert data there.
  // what if we are trying to update the value for an existing key though?
  // we can just mark it as 'deleted', and then the loop should break.
  while (current != NULL) {
    if (strcmp(current->key, k) == 0) {
      current->isDeleted = true;
    }
    if (current->isDeleted) {
      break;
    }
    index = get_hash(k, a->size, attempt);
    current = a->items[index];
    attempt++;
  }
  a->items[index] = kvp;  
  a->items[index]->isDeleted = false; 
  // this only changes things if the key-value pair was 'deleted' previously
  a->count++;
}

char* searchInHM(HashMap *a, char *k) {
  int index = get_hash(k, a->size, 0);
  KVPair *current = a->items[index];
  int attempt = 1;

  while (current != NULL) {
    if (!current->isDeleted && strcmp(k, current->key) == 0) {
      return current->value;
    }
    index = get_hash(k, a->size, attempt);
    current = a->items[index];
    attempt++;
  }
  return NULL;
}

// since we're on the open addressing collision resolution method, deleting
// will be complex. if we just delete the KVPair normally, then we could be
// breaking a potential collision chain, which would mean we will not be able to find
// elements in the hashmap, even if they exist. Not very reliable.
// Solution: why not temporarily mark it as deleted, until a new insertion can
// replace the old data there?
void deleteFromHM(HashMap *a, char *k) {
  int index = get_hash(k, a->size, 0);
  KVPair *current = a->items[index];
  int attempt = 1;

  while (current != NULL) {
    if (!current->isDeleted && strcmp(k, current->key) == 0) {
      // deleteKVPair(current);
      current->isDeleted = true;
    }
    index = get_hash(k, a->size, attempt);
    current = a->items[index];
    attempt++;
  }
}










