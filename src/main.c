#include "common.h"

int main() {
  HashMap *hm = newHashMap();
  char *result;

  insertIntoHM(hm, "key", "string");
  // char *result = searchInHM(hm, "key");
  // printf("%s\n", result == NULL ? "NOPE" : result);

  insertIntoHM(hm, "key1", "bob");
  // result = searchInHM(hm, "key1");
  // printf("%s\n", result == NULL ? "NOPE" : result);

  insertIntoHM(hm, "key", "updated string");
  // result = searchInHM(hm, "key");
  // printf("%s\n", result == NULL ? "NOPE" : result);

  insertIntoHM(hm, "ke2", "l factor");
  // result = searchInHM(hm, "ke2");
  // printf("%s\n", result == NULL ? "NOPE" : result);

  insertIntoHM(hm, "ke4", "load factor should decrease..");
  result = searchInHM(hm, "ke2");
  printf("%s\n", result == NULL ? "NOPE" : result);
}
