#ifndef BOGGLEPLAYER_H
#define BOGGLEPLAYER_H

#include "iboggleplayer.h"
#include "boggleutil.h"
#include <set>
#include <vector>
#include <string>
#include <unordered_set>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <climits>
#include <time.h>

using namespace std;

class BogglePlayer: public IBogglePlayer {

private:

  // Major private variables
  // The dictionary, the states map, size, and built board/lexicon
  Trictionary lexicon;
  string** board;
  vector< vector<bool> > states;
  int numRows;
  int numCols;
  bool lexBuilt;
  bool boardBuilt;

  // These are functions that are called recursively to trace paths

  // Traces both the trie and the board, returning any valid words of minimum length
  void dfSearch(int currRow, int currCol, string currPath, trieNode* currNode, int minLength, set<string>* words);

  // Traces just the board, returning the path formed when the word is found
  bool dfSearchPath(int currRow, int currCol, string remaining, vector<int>& path);

  // Find the substring in the dictionary starting at the given node
  trieNode* isInSubtree(const string& word_to_check, trieNode* start);

  // Clears the 2d array that keeps track of visits
  void clearStates();

public:

  //Constructor/Destructor
  BogglePlayer();
  ~BogglePlayer();

  // IBogglePlayer Functions
  void buildLexicon(const vector<string>& word_list);
  void setBoard(unsigned int rows, unsigned int cols, string** diceArray);
  bool getAllValidWords(unsigned int minimum_word_length, set<string>* words);
  bool isInLexicon(const string& word_to_check);
  vector<int> isOnBoard(const string& word_to_check);
  void getCustomBoard(string** &new_board, unsigned int *rows, unsigned int *cols);

};

#endif // BOGGLEPLAYER_H
