#include "boggleplayer.h"
#include "boggleutil.h"

/** Implementation of the BogglePlayer class */

// Default constructor, sets zeroes
BogglePlayer::BogglePlayer(){
  numRows = 0;
  numCols = 0;
  lexBuilt = false;
  boardBuilt = false;
}

// Default destructor
BogglePlayer::~BogglePlayer(){

}

// Builds the lexicon given a word list
// Utilizes a ternary trie
void BogglePlayer::buildLexicon(const vector<string>& word_list){

  // First, randomize the word list to get an efficient insertion for the trie
  srand(unsigned(time(NULL)));
  vector<string> shuffledWords = word_list;
  random_shuffle(shuffledWords.begin(), shuffledWords.end());

  string lowerWord;
  int wlSize = shuffledWords.size();

  // Loop through the now shuffled list and insert into trie
  for(int i = 0; i < wlSize; i++) {
    lowerWord = shuffledWords[i];
    transform(lowerWord.begin(), lowerWord.end(), lowerWord.begin(), ::tolower);
    lexicon.addWord(lowerWord);
  }

  // Indicate it has been built
  lexBuilt = true;

}

// Sets up the board for use by other functions
void BogglePlayer::setBoard(unsigned int rows, unsigned int cols, string** diceArray){

  // Save diceArray pointer and size to member variables for use later
  board = diceArray;
  numRows = rows;
  numCols = cols;

  // Clear the states values, which also resizes that
  clearStates();

  // Indicate the board has been built
  boardBuilt = true;

}


// Searches the board for all valid words
// Follows the graph AND the trie at the same time
bool BogglePlayer::getAllValidWords(unsigned int minimum_word_length, set<string>* words){

  // If built
  if(lexBuilt && boardBuilt){

    // Loop through every tile on the board and start a recursive path search from there
    for(int i = 0; i < numRows; i++){
      for(int j = 0; j< numCols; j++){
        dfSearch(i, j, "", lexicon.root, minimum_word_length, words);
        states[i][j] = false;
      }
    }

    // Just in case, clear states
    clearStates();
    return true;
  }

  else{
    return false;
  }

}

// Recursive function, checks current tile and node then checks all its neighbors
void BogglePlayer::dfSearch(int currRow, int currCol, string currPath, trieNode* currNode, int minLength, set<string>* words){

  // Set variables
  string currTile = board[currRow][currCol];
  string newString;
  int newStringLength;
  trieNode* foundNode = isInSubtree(currTile, currNode);

  // If the character isn't in the tree at that point, then exit
  if(foundNode != nullptr){

    // If it is in the tree, check if it is a word end
    if(foundNode->isEnd){

      // If it is, add it to the current path and send that to the list
      newString = currPath+currTile;
      newStringLength = newString.length();

      if(newStringLength >= minLength){
        transform(newString.begin(), newString.end(), newString.begin(), ::tolower);
        words->insert(newString);
      }
    }

    // If that node is a prefix that continues, keep moving!
    if(foundNode->middle != nullptr){

      // Set visited
      states[currRow][currCol] = true;

      // Visit adjacent tiles

      // Top-left
      if(currRow > 0 && currCol > 0 && !states[currRow-1][currCol-1]){
        dfSearch(currRow-1, currCol-1, currPath+currTile, foundNode->middle, minLength, words);
        states[currRow-1][currCol-1] = false;
      }

      // Top
      if(currRow > 0 && !states[currRow-1][currCol]){
        dfSearch(currRow-1, currCol, currPath+currTile, foundNode->middle, minLength, words);
        states[currRow-1][currCol] = false;
      }

      // Top-right
      if(currRow > 0 && currCol < numCols-1 && !states[currRow-1][currCol+1]){
        dfSearch(currRow-1, currCol+1, currPath+currTile, foundNode->middle, minLength, words);
        states[currRow-1][currCol+1] = false;
      }

      // Left
      if(currCol > 0 && !states[currRow][currCol-1]){
        dfSearch(currRow, currCol-1, currPath+currTile, foundNode->middle, minLength, words);
        states[currRow][currCol-1] = false;
      }

      // Right
      if(currCol < numCols-1 && !states[currRow][currCol+1]){
        dfSearch(currRow, currCol+1, currPath+currTile, foundNode->middle, minLength, words);
        states[currRow][currCol+1] = false;
      }

      // Bottom-left
      if(currRow < numRows-1 && currCol > 0 && !states[currRow+1][currCol-1]){
        dfSearch(currRow+1, currCol-1, currPath+currTile, foundNode->middle, minLength, words);
        states[currRow+1][currCol-1] = false;
      }

      // Bottom
      if(currRow < numRows-1 && !states[currRow+1][currCol]){
        dfSearch(currRow+1, currCol, currPath+currTile, foundNode->middle, minLength, words);
        states[currRow+1][currCol] = false;
      }

      // Bottom-left
      if(currRow < numRows-1 && currCol < numCols-1 && !states[currRow+1][currCol+1]){
        dfSearch(currRow+1, currCol+1, currPath+currTile, foundNode->middle, minLength, words);
        states[currRow+1][currCol+1] = false;
      }

    }
  }
}

// Returns true if the word is in a built lexicon, false otherwise
bool BogglePlayer::isInLexicon(const string& word_to_check){

  // Call the search, starting at the root
  trieNode* foundNode = isInSubtree(word_to_check, lexicon.root);
  if(foundNode != nullptr){
    return foundNode->isEnd;
  }

  else{
    return false;
  }
}

// Search function for the trie, checks if a node is in the subtree starting at 'start'
// Returns a pointer to the found node, for use by other functions
trieNode* BogglePlayer::isInSubtree(const string& word_to_check, trieNode* start){

  if(lexBuilt){

    trieNode* curr = start;
    int wordLength = word_to_check.length();

    int i = 0;

    // Move through tree, if a null is reached return false
    while(curr != nullptr){

      // If not at current letter, move left or right
      if(curr->letter != tolower(word_to_check[i])){
        if(tolower(word_to_check[i]) < curr->letter){
          curr = curr->left;
        }

        else{
          curr = curr->right;
        }
      }

      // If you found the letter
      else{

        // If that's the last letter, check if it ends a word
        if(i == (wordLength - 1) )
        {
          return curr;
        }

        // If not last, descend and check next letter
        else{
          curr = curr->middle;
          i++;
        }
      }
    }

    return nullptr;

  }

  else{
    return nullptr;
  }

}

// Checks the board for the given word
vector<int> BogglePlayer::isOnBoard(const string& word_to_check){

  vector<int> path;
  bool found;
  clearStates();

  //Start with first symbol, search its paths through the recursive function
  for(int i = 0; i < numRows; i++){
    for(int j = 0; j < numCols; j++){
      found = dfSearchPath(i, j, word_to_check, path);
      states[i][j] = false;
      if(found == true){
        return path;
      }
    }
  }

  return path;
}

// Recursive function that checks if the current tile starts the remaining string
// Then calls the same function for all its neighbors
// The path is built through all the calls
bool BogglePlayer::dfSearchPath(int currRow, int currCol, const string remaining, vector<int>& path){

  string currTile = board[currRow][currCol];
  transform(currTile.begin(), currTile.end(), currTile.begin(), ::tolower);
  int ctLength = currTile.length();
  string remainder = remaining;
  bool found = false;

  // If this tile continues the word
  size_t strFound = remainder.find(currTile);
  if(strFound == 0){

    // Add it to the path, set it as visited, remove its part from the word
    path.push_back(currRow*numCols+currCol);
    states[currRow][currCol] = true;
    remainder.erase(0, ctLength);

    // If the whole string has been found, return true
    if(remainder.length() == 0){
      return true;
    }

    // Check adjacent tiles

    // Top-left
    if(currRow > 0 && currCol > 0 && !states[currRow-1][currCol-1]){
      found = dfSearchPath(currRow-1, currCol-1, remainder, path);
      states[currRow-1][currCol-1] = false;

      if(found){
        return found;
      }
    }

    // Top
    if(currRow > 0 && !states[currRow-1][currCol]){
      found = dfSearchPath(currRow-1, currCol, remainder, path);
      states[currRow-1][currCol] = false;

      if(found){
        return found;
      }
    }

    // Top-right
    if(currRow > 0 && currCol < numCols-1 && !states[currRow-1][currCol+1]){
      found = dfSearchPath(currRow-1, currCol+1, remainder, path);
      states[currRow-1][currCol+1] = false;

      if(found){
        return found;
      }
    }

    // Left
    if(currCol > 0 && !states[currRow][currCol-1]){
      found = dfSearchPath(currRow, currCol-1, remainder, path);
      states[currRow][currCol-1] = false;

      if(found){
        return found;
      }
    }

    // Right
    if(currCol < numCols-1 && !states[currRow][currCol+1]){
      found = dfSearchPath(currRow, currCol+1, remainder, path);
      states[currRow][currCol+1] = false;

      if(found){
        return found;
      }
    }

    // Bottom-left
    if(currRow < numRows-1 && currCol > 0 && !states[currRow+1][currCol-1]){
      found = dfSearchPath(currRow+1, currCol-1, remainder, path);
      states[currRow+1][currCol-1] = false;

      if(found){
        return found;
      }
    }

    // Bottom
    if(currRow < numRows-1 && !states[currRow+1][currCol]){
      found = dfSearchPath(currRow+1, currCol, remainder, path);
      states[currRow+1][currCol] = false;

      if(found){
        return found;
      }
    }

    // Bottom-left
    if(currRow < numRows-1 && currCol < numCols-1 && !states[currRow+1][currCol+1]){
      found = dfSearchPath(currRow+1, currCol+1, remainder, path);
      states[currRow+1][currCol+1] = false;

      if(found){
        return found;
      }
    }

  }

  else{
    return false;
  }

  // If none of the neighbors led to the word, pop this tile off the path
  path.pop_back();
  return false;
}

// Creates a custom board that populates the new_board array
// This function is currently configured to use the provided speed test
// A 20 x 23 array with the specified tiles in "brd.txt"
void BogglePlayer::getCustomBoard(string** &new_board, unsigned int *rows, unsigned int *cols){

  ifstream infile("brd.txt");

  string rowLine;
  string colLine;

  getline(infile, rowLine);
  getline(infile, colLine);

  *rows = atoi(rowLine.c_str());
  *cols = atoi(colLine.c_str());

  new_board = new string*[*rows];
  for(unsigned int i = 0; i <*rows; i++){
    new_board[i] = new string[*cols];
  }

  string tile;
  for(unsigned int j = 0; j < *rows; j++){
    for(unsigned int k = 0; k < *cols; k++){
      getline(infile, tile);
      new_board[j][k] = tile;
    }
  }

}

// Function that clears the array of states
// The states array keeps track of nodes that have been visited
void BogglePlayer::clearStates(){

  int statesSize = states.size();
  for(int i = 0; i < statesSize; i++){
    states[i].clear();
  }

  states.clear();

  // create states board, set all to false
  for(int i = 0; i<numRows; i++){
    vector<bool> row;
    for(int j = 0; j<numCols; j++){
      row.push_back(false);
    }
    states.push_back(row);
  }

}
