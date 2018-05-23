#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include "trie.h"

static const int alphabet_num = 26;

// index(a) will return the index of char a 
//   like a...0, b...1, ..., z...25.
// requires: 'a' <= a <= 'z'
// time: O(1).
static int index (char a) {
  assert (a >= 'a');
  assert (a <= 'z');
  return a - 'a';
}


// create_enode() creates a trienode with 
//   word_end false and all children NULL.
// effects: allocates memory (must call trienode_destroy)
// time: O(1).
static struct trienode *create_enode (void) {
  struct trienode *n = malloc(sizeof(struct trienode));
  n->word_end = false;
  for (int i = 0; i < alphabet_num; ++i) {
    n->children[i] = NULL;
  }
  return n;
}


// is_child_empty(tn) will return true if tn->children are all NULL.
//   and false otherwise
// requires: tn is a valid struct trienode pointer
// time: O(1)
static bool is_child_empty (struct trienode *tn) {
  assert(tn);
  for (int i = 0; i < alphabet_num; ++i) {
    if (tn->children[i] != NULL) return false;
  }
  return true;
}


struct trie *trie_create(void) {
  struct trie *t = malloc(sizeof(struct trie));
  t->root = NULL;
  return t;
}


void trie_add(const char *s, struct trie *t) {
  const char *ds = s;
  // if there is no trienode before
  if (t->root == NULL) {
    struct trienode *node = create_enode();
    t->root = node;
  }
  // add char of string one by one
  struct trienode *pos = t->root;
  while (*ds != '\0') {
    if (pos->children[index(*ds)] == NULL) {
      pos->children[index(*ds)] = create_enode();
    }
    pos = pos->children[index(*ds)];
    ++ds;
  }
  // modify word_end to show "end"
  pos->word_end = true;  
}


// trienode_remove(s, tn) remove *s in tn and remove
//   any trienodes that are no longer needed
// require: s is valid
// effects: may modify tn
// time:    O(n), n is the length of s
static struct trienode *trienode_remove(const char *s, struct trienode *tn) {
  assert(s);
  if (tn == NULL) return NULL;  // if s is not in trie
  // no trienode use "the last" trienode
  else if (*s =='\0' && is_child_empty(tn)) {
    free(tn);
    return NULL;
  }
  // there exist some trienodes use "the last" trienode
  else if (*s == '\0') {
    tn->word_end = false;
    return tn;
  }
  // if *s != '\0', tn must have children who are not NULL
  else {
    tn->children[index(*s)] = trienode_remove(s + 1, tn->children[index(*s)]);
    // no other trienode use this trienode
    if (is_child_empty(tn) && !tn->word_end) {
      free(tn);
      return NULL;
    }
    else {  // there exist some trienodes use this trienode
      return tn;
    }
  }
}


void trie_remove(const char *s, struct trie *t) {
  assert(t); 
  // if t->root exist, then we can remove s
  if (t->root != NULL) { 
    t->root = trienode_remove(s, t->root);
  }
}


// trienode_lookup(s, tn) return true if *s
//   is in tn and false otherwise
// require: s is valid
// time:    O(n), n is the length of s
static bool trienode_lookup(const char *s, const struct trienode *tn) {
  assert(s);
  if (tn == NULL) return false; 
  else if (*s == '\0' && tn->word_end) {
    return true;
  }  
  else if (*s == '\0') {
    return false;
  }
  else return trienode_lookup(s + 1, tn->children[index(*s)]);
}


bool trie_lookup(const char *s, const struct trie *t) {
  assert(t);
  const char *ds = s;
  return trienode_lookup(ds, t->root);
}


// trienode_destroy(tn) frees all memory for tn
// effects: tn is no longer valid
// time:    O(1)
static void trienode_destroy(struct trienode *tn) {
  if (tn != NULL) {
    for (int i = 0; i < alphabet_num; ++i) {
      trienode_destroy(tn->children[i]);
    }
    free(tn);
  }
}


void trie_destroy(struct trie *t) {
  trienode_destroy(t->root);
  free(t);
}


struct word {
  int maxlen;
  char *s;
};


// word_add(len, c, w) add char c on w[len]
// require: len >= 0
//          'a' <= c <= 'z' or c = '\0'
//          w is valid
// effect: modify w
// time: Amortized O(1)
static void word_add (int len, char c, struct word *w) {
  assert(len >= 0);
  assert((c >= 'a' && c <= 'z') || c == '\0');
  assert(w);
  if (len == w->maxlen) {
    w->maxlen *= 2;
    w->s = realloc(w->s, w->maxlen * sizeof(char));
  }
  w->s[len] = c;
}


// trienode_print(len, tn, w) print out w when reaches
//   the end of the word based on tn->word_end, and use
//   len to denote the length of w
// require: len >= 0
//          w is valid
// effect: modify w
//         print out chars in w
// time: O(n), n is len
static void trienode_print(int len, const struct trienode *tn, struct word *w) {
  assert(len >= 0);
  assert(w);
  if (tn) {
    // if the word is end, print out the char in w
    if(tn->word_end) {
      for (int i = 0; i < len; ++i) {
        printf("%c", w->s[i]);
      }
      printf("\n");
    }
    // fill in all its children's char in w 
    for (int i = 0; i < alphabet_num; ++i) {
      if (tn->children[i] != NULL) {
        word_add(len, 'a' + i, w);
        trienode_print(len + 1, tn->children[i], w);
      }
    }
  }
}


void trie_print(const struct trie *t) {
  // modify memory
  struct word *w = malloc(sizeof(struct word));
  w->maxlen = 1;
  w->s = malloc(w->maxlen * sizeof(char));
  
  int orig_len = 0;
  trienode_print(orig_len, t->root, w);
  
  // destroy w
  free(w->s);
  free(w);
}


// trienode_to_aos(index, ttaos, tn, len, w) fill in
//   char in tn into w[len], and fill char in w into
//   into ttaos[index] when reach the word_end
// require: len >= 0;
//          ttaos, w is valid
// effect: may modify ttaos and w, and s_index
// time: O(1)
static void trienode_to_aos(int *index, char **ttaos, struct trienode *tn,
                            int len, struct word *w) {
  assert(len >= 0);
  assert(ttaos);
  assert(w);
  if (tn) {
    if(tn->word_end) {
      // add \0 at the end of string.
      word_add(len, '\0', w);
      // count the length of w and allocate memory for char *ttaos[s_index].
      int w_len = strlen(w->s); 
      ttaos[*index] = malloc((w_len + 1) * sizeof(char)); 
      strcpy(ttaos[*index], w->s);
      ++(*index);
    }
    for (int i = 0; i < alphabet_num; ++i) {
      if (tn->children[i] != NULL) {
        word_add (len, 'a' + i, w);
        //trienode_print(len + 1, tn->children[i], w);
        trienode_to_aos(index, ttaos, tn->children[i], len + 1, w);
      }
    }
  }
}


char **trie_to_aos(const struct trie *t) {
  if (t->root == NULL) return NULL;
  int s_num = trie_count(t);
  char **ttaos = malloc(s_num * sizeof(char *));
  
  // create a struct word *w to store char when encountered
  struct word *w = malloc(sizeof(struct word));
  w->maxlen = 1;
  w->s = malloc(w->maxlen * sizeof(char));
  
  // create a int *index to store index
  int *index = malloc(sizeof(int)); 
  *index = 0;
  
  int orig_len = 0;
  trienode_to_aos(index, ttaos, t->root, orig_len, w); 
  
  // free memory
  free(index);
  free(w->s);
  free(w);
  return ttaos;
}


// trienode_count(count, tn) count how many word_end in tn
// require: count >= 0
// time: O(n), n is the number of tn
static int trienode_count(int count, const struct trienode *tn) {
  if (tn == NULL) return 0;
  else {
    int child_count = 0;
    for(int i = 0; i < alphabet_num; ++i) {
      child_count += trienode_count(0, tn->children[i]);
    }
    if (tn->word_end) return child_count + 1;
    else return child_count;
  }
}
  

int trie_count(const struct trie *t) {
  int count = 0;
  return trienode_count(count, t->root);
}
