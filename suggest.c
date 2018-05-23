#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "suggest.h"

// sdelete(len, s, wl, is_word) remove each letter from word s and if
//   new deletion meets is_word, add it into wl.
// require: len >= 0
//          s, wl, is_word is valid
// effect: may modify wl
// time: O(n2), n is the len of word s
static struct wordlist *sdelete(int len, const char *s, struct wordlist *wl, 
                                bool (*is_word)(const char *)) {
  assert(len >= 0);
  assert(s);
  assert(wl);
  assert(is_word);
  char *ds = malloc(len * sizeof(char));
  for (int i = 0; i < len; ++i) {
    for (int j = 0; j < i; ++j) {
      ds[j] = s[j];
    }
    for (int j = i + 1; j < len; ++j) {
      ds[j - 1] = s[j];
    }
    ds[len - 1] = '\0';
    if (is_word(ds)) wordlist_add(ds, wl);
  }
  free(ds);
  return wl;
}


// sinsert(len, s, wl, is_word) insert every possible letter (a...z) before
//   each letter in the keyword (or at the end of the keyword) and if
//   new insertion meets is_word, add it into wl.
// require: len >= 0
//          s, wl, is_word is valid
// effect: may modify wl
// time: O(n2), n is the len of word s
static struct wordlist *sinsert(int len, const char *s, struct wordlist *wl, 
                                bool (*is_word)(const char *)) {
  assert(len >= 0);
  assert(s);
  assert(wl);
  assert(is_word);
  char *ds = malloc((len + 2) * sizeof(char));
  for (int i = 0; i <= len; ++i) {
    for (int j = 0; j < i; ++j) {
      ds[j] = s[j];
    }
    for (int j = i + 1; j <= len; ++j) {
      ds[j] = s[j - 1];
    }
    for (int letter = 0; letter < 26; ++letter) {
      ds[i] = 'a' + letter;
      ds[len + 1] = '\0';
      if (is_word(ds)) wordlist_add(ds, wl);
    }
  }
  free(ds);
  return wl;
}


// ssubstitute(len, s, wl, is_word) substitue each letter with every possible
//   letter and if new substitution meets is_word, add it into wl.
// require: len >= 0
//          s, wl, is_word is valid
// effect: may modify wl
// time: O(n2), n is the len of word s
static struct wordlist *ssubstitute(int len, const char *s, 
                                    struct wordlist *wl, 
                                    bool (*is_word)(const char *)) {
  char *ds = malloc((len + 1) * sizeof(char));
  for (int i = 0; i < len; ++i) {
    for (int j = 0; j < i; ++j) {
      ds[j] = s[j];
    }
    for (int j = i + 1; j < len; ++j) {
      ds[j] = s[j];
    }
    for (int letter = 0; letter < 26; ++letter) {
      ds[i] = 'a' + letter;
      ds[len] = '\0';
      if (is_word(ds)) wordlist_add(ds, wl);
    }
  }
  free(ds);
  return wl;
}


// transpose(len, s, wl, is_word) transpose (swap) each adjacent pair
//   of letters in the word and if new substitution meets is_word, 
//   add it into wl.
// require: len >= 0
//          s, wl, is_word is valid
// effect: may modify wl
// time: O(n2), n is the len of word s
static struct wordlist *transpose(int len, const char *s, struct wordlist *wl, 
                                  bool (*is_word)(const char *)) {
  char *ds = malloc((len + 1) * sizeof(char));
  for (int i = 0; i < len - 1; ++i) {
    for (int j = 0; j < i; ++j) {
      ds[j] = s[j];
    }
    for (int j = i + 2; j < len; ++j) {
      ds[j] = s[j];
    }
    ds[i] = s[i + 1];
    ds[i + 1] = s[i];
    ds[len] = '\0';
    if (is_word(ds)) wordlist_add(ds, wl);
  }
  free(ds);
  return wl;
}


struct wordlist *suggest(const char *s,
                         bool (*is_word)(const char *)) {
  struct wordlist *wl = wordlist_create();
  int len = strlen(s);
  
  // check for single deletions:
  wl = sdelete(len, s, wl, is_word);
  
  // check for single insertion:
  wl = sinsert(len, s, wl, is_word);
  
  // check for single substitution:
  wl = ssubstitute(len, s, wl, is_word);
  
  // check for transposition:
  wl = transpose(len, s, wl, is_word);
  
  // remove the original keyword and empty string("")
  wordlist_remove(s, wl);
  wordlist_remove("", wl);
  
  return wl;
}
