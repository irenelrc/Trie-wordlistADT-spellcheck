# Trie-wordlistADT-spellcheck
Trie data structure + wordlist ADT + spellcheck function

! This is a school project of CS136(2018 Winter) of University of Waterloo !
! With thanks to all of the profs !

Three parts:

a) Trie data structure, a tree where each node has 26 children, which represent 26 letters of the alphabet.
   https://www.student.cs.uwaterloo.ca/~cs136/assignments/a10/trie.png (to see how it works)

b) wordlist ADT wrapper for trie data structure

c) Suggest function determines ifi a word is spelled correctly, and return a wordlist containing 
   correctly spelled words that are "close" to the keyword(have one change)
   change include deletion / insertion / substitution / transposition
