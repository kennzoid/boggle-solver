#include "boggleutil.h"

/** Implementation of the Trictionary class */

// Add the entire word characyer by charcter into the tree
void Trictionary::addWord(string word){

  // If trie is empty, put the first letter of first word into root
  if(root == nullptr){
    root = new trieNode(word[0]);
  }

  trieNode* curr = root;
  trieNode* prev = nullptr;
  int wordLength = word.length();

  // Loop through the word, 1 char at a time
  for(int i = 0; i<wordLength; i++){

    // If you descended from a fresh node, it needs a new middle
    if(curr == nullptr){
      curr = new trieNode(word[i]);
      prev->middle = curr;
    }

    // While you are not at the node with the letter
    while(word[i] != curr->letter){

      // If letter is less than current
      if(word[i] < curr->letter){

        // If there is a left node keep going
        if(curr->left != nullptr){
          curr = curr->left;
        }

        // If not, make a new one and go to it
        else{
          curr->left = new trieNode(word[i]);
          curr = curr->left;
        }
      }

      // If greater
      else{

        // If there is a right node keep going
        if(curr->right != nullptr){
          curr = curr->right;
        }

        // Else make a new one
        else{
          curr->right = new trieNode(word[i]);
          curr = curr->right;
        }
      }
    }

    // You are now at the node with the letter, move down
    prev = curr;
    curr = curr->middle;
  }

  // You have now inserted every letter into the trie
  prev->isEnd = true;
}

// Default destructor
Trictionary::~Trictionary(){
  removeNode(root);
}

// Recursive node removal, delete children then deletes itself
void Trictionary::removeNode(trieNode* node){

  if(node->left != nullptr){
    removeNode(node->left);
  }

  if(node->right != nullptr){
    removeNode(node->right);
  }

  if(node->middle != nullptr){
    removeNode(node->middle);
  }

  delete node;
}
