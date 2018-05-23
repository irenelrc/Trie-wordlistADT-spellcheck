#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "trie.h"

// a simple assertion-based client for the trie module

int main(void) {
  struct trie *t = trie_create();
  assert(t);
  assert(t->root == NULL);
  assert(trie_count(t) == 0);
  trie_add("a", t);
  assert(trie_lookup("a", t)); 
  assert(trie_count(t) == 1);
  assert(t->root->children[0]);
  assert(t->root->children[0]->word_end);
  trie_print(t); // should just print out "a\n"
  char **aos = trie_to_aos(t);
  assert(aos);
  assert(!strcmp("a", aos[0]));
  free(aos[0]);
  free(aos);
  assert(!trie_lookup("b", t));
  trie_add("b", t);
  assert(t->root->children[1]);
  assert(t->root->children[1]->word_end);
  assert(trie_lookup("b", t));
  assert(trie_count(t) == 2);
  trie_remove("b", t);
  assert(t->root->children[1] == NULL);
  assert(!trie_lookup("b", t));
  assert(trie_count(t) == 1);
  trie_destroy(t); 
  
  // own test
  struct trie *t2 = trie_create();
  assert(t2);
  assert(t2->root == NULL);
  assert(trie_count(t2) == 0);
  trie_add("a", t2);
  assert(trie_count(t2) == 1);
  trie_print(t2);   // print out "a\n"
  assert(!trie_lookup("", t2));
  trie_add("", t2);
  assert(t2->root->word_end == true);
  trie_print(t2);  // print out "\na\n"
  assert(!trie_lookup("ace", t2)); 
  trie_add("ace", t2);
  trie_add("apple", t2);
  assert(trie_count(t2) == 4);
  trie_print(t2);  // print out "\na\nace\napple\n"
  assert(trie_lookup("ace", t2)); 
  assert(trie_lookup("apple", t2)); 
  assert(!trie_lookup("acepple", t2)); 
  assert(t2->root->children[0]);
  assert(t2->root->children[0]->word_end);
  assert(!trie_lookup("cs", t2)); 
  trie_add("cs", t2);
  trie_add("math", t2);
  trie_add("zoo", t2);
  trie_print(t2);  // print out "\na\nace\napple\ncs\nmath\nzoo\n"
  assert(trie_count(t2) == 7);
  char **aoss = trie_to_aos(t2);
  assert(aoss);
  assert(!strcmp("", aoss[0]));
  assert(!strcmp("a", aoss[1]));
  assert(!strcmp("ace", aoss[2]));
  assert(!strcmp("apple", aoss[3]));
  assert(!strcmp("cs", aoss[4]));
  assert(!strcmp("math", aoss[5]));
  assert(!strcmp("zoo", aoss[6]));
  free(aoss[0]);
  free(aoss[1]);
  free(aoss[2]);
  free(aoss[3]);
  free(aoss[4]);
  free(aoss[5]);
  free(aoss[6]);
  free(aoss);
  trie_remove("a", t2);
  assert(!trie_lookup("a", t2));
  assert(trie_count(t2) == 6);
  trie_remove("cs", t2);
  trie_remove("math", t2);
  trie_remove("apple", t2);
  assert(trie_count(t2) == 3);
  trie_remove("apple", t2);
  assert(trie_count(t2) == 3);
  trie_print(t2); // print out "\nace\nzoo\n"
  trie_remove("", t2);
  assert(t2->root->word_end == false);
  assert(trie_count(t2) == 2);
  trie_destroy(t2);
}
