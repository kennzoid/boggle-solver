#ifndef BOGGLEUTIL_H
#define BOGGLEUTIL_H

#include <set>
#include <vector>
#include <string>

using namespace std;

// The node class for my ternary trie
class trieNode {

public:

  // Important member variables
  bool isEnd = false;
  char letter;
  trieNode* left;
  trieNode* middle;
  trieNode* right;

  // Default constructor
  trieNode(
  char letter = 0,
  trieNode* left = 0,
  trieNode* middle = 0,
  trieNode* right = 0)
  : letter(letter), left(left), middle(middle), right(right) { }
};


// The ternary trie class that will represent the dictionary/lexicon
class Trictionary {

private:

  // Recursive node removal function
  void removeNode(trieNode* node);

public:
  trieNode* root;

  // Default constructor and destructor
  Trictionary(){
    root = nullptr;
  }

  ~Trictionary();

  // Adds the given word to the trie
  void addWord(string word);

};

#endif // BOGGLEUTIL_H
