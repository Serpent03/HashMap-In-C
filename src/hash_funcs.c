#include "common.h"
#include <math.h>
// all non-public functions are stored here

// the primes can only be something bigger than the size
#define HASH_GEN_CONST_1 151
#define HASH_GEN_CONST_2 163

// return the next prime after base
int getNextPrime(int base) {
  // sieve of eratothenes..
  int prime = 0;
  int valuesLen = base * 2;
  bool *values = (bool*)calloc(valuesLen, sizeof(bool));

  // initialize all of them to true
  for (int i = 0; i < valuesLen; i++) {
    values[i] = true;
  }

  for (int i = 2; i < valuesLen; i++) {
    if (values[i]) {
      int j = (int)pow(i, 2);
      while (j <= valuesLen) {
        values[j] = false;
        j = j + i;
      }
    } 
  }
  for (int i = valuesLen - 1; i >= 0; i--) {
    if (values[i]) {
      prime = i;
      break;
    }
  }
  free(values);
  return prime;
}

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
  hm->load = 0.0f;
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

void resizeHashMap(HashMap *hm, int size) {
  
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
  const int hash_a = hash_generator(string, HASH_GEN_CONST_1, size);
  const int hash_b = hash_generator(string, HASH_GEN_CONST_2, size);
  return hash_a + (attempt * (hash_b + 1)) % size;
}

// TODO automatically resizing the items KVPair array
void insertIntoHM(HashMap *a, char *k, char *v) {
  if (a->load > 0.7) {
    int newSize = getNextPrime(a->size * 2);
    resizeHashMap(a, newSize);
  }

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
  a->load = (float)a->count / (float)a->size;
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
      a->count--;
    }
    index = get_hash(k, a->size, attempt);
    current = a->items[index];
    attempt++;
  }
}










