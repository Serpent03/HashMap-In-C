#include "common.h"

int main() {
  HashMap *hm = newHashMap();
  insertIntoHM(hm, "key", "string");

  char *result = searchInHM(hm, "key");
  printf("%s\n", result == NULL ? "NOPE" : result);

  insertIntoHM(hm, "key", "updated string");
  result = searchInHM(hm, "key");
  printf("%s\n", result == NULL ? "NOPE" : result);
}
