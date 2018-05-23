#include <stdlib.h>
#include "trie.h"
#include "wordlist.h"

struct wordlist {
  int len;
  struct trie *t;
};


struct wordlist *wordlist_create(void) {
  struct wordlist *wl = malloc(sizeof(struct wordlist));
  wl->len = 0;
  wl->t = trie_create();
  return wl;
}


void wordlist_add(const char *s, struct wordlist *wl) {
  if(!wordlist_lookup(s, wl)) ++(wl->len);
  trie_add(s, wl->t);
}


void wordlist_remove(const char *s, struct wordlist *wl) {
  if(wordlist_lookup(s, wl)) --(wl->len);
  trie_remove(s, wl->t);
}


bool wordlist_lookup(const char *s, const struct wordlist *wl) {
  return trie_lookup(s, wl->t);
}


void wordlist_destroy(struct wordlist *wl) {
  trie_destroy(wl->t);
  free(wl);
}


void wordlist_print(const struct wordlist *wl) {
  trie_print(wl->t);
}


int wordlist_count(const struct wordlist *wl) {
  return wl->len;
}
