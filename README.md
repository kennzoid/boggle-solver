bogglesolver
=======================
boggleplayer contains the methods that create a boggle board, solve it (find every valid word on the board), and also the methods that allow for a human player to compete (basically checks the words they enter for validity).

boggleutil contains the additional helper classes and methods that were used; namely, a ternary trie for holding the lexicon. 

A custom dictionary, board size, board configuration, and word count can be supplied as well. The GUI is not included.

The basic logic for solving the board involves a depth first search of the board that traverses the ternary trie in parallel. When a bad prefix is reached on the trie, the depth first search on the board stops prematurely and backtracks to the next unsearched branch.
