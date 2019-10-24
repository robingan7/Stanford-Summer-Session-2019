#include "bogglegui.h"
#include <string>
#include "grid.h"
#include "lexicon.h"
#include "set.h"
#include "vector.h"
#include "strlib.h"
using namespace std;

Vector< Vector<int> > findFirstOnes(Grid<char>& board,string word);

Vector< Vector<int> > findNeighbors(Grid<char>& board,int x,int y);

bool humanWordSearchHelper(Grid<char>& board, Lexicon& dictionary, string word,unsigned int index,
                           Vector<string>& used, int x, int y);

void score(bool isHuman, string word);

void computerWordSearchHelper(Grid<char>& board, Lexicon& dictionary, Set<string>& humanWords,
                              int index, int x, int y, string word, Vector<string>& used, Set<string>& words);

bool humanWordSearch(Grid<char>& board, Lexicon& dictionary, string word) {
    Vector<string> used_word;
    return humanWordSearchHelper(board, dictionary, word, 0, used_word, 0, 0);
}


Set<string> computerWordSearch(Grid<char>& board, Lexicon& dictionary, Set<string>& humanWords) {
    Vector<int> current_;
    Vector<string> used_word;
    Set<string> words;
    string current_word = "";

    //passing every point on grid to explore
    for (int i = 0; i < board.numRows(); i++) {
        for (int j = 0; j < board.numCols(); j++) {
            computerWordSearchHelper(board, dictionary, humanWords,
                                      0, i, j, current_word,used_word, words);
        }
    }

    return words;
}

/**
 * @brief computerWordSearchHelper takes extra index, used x, y parameter returns
 *        if the word is valid and a solution to the boggle
 *
 * @param index: the current index we are at now
 * @param used: contains the word we already passed
 * @param x: x coordinate of the point
 * @param y: y coordinate of the point
 * @param word: the word we have so far
 * @param used: contains the word we already passed
 * @param words: the reuslt of computerWordSearch
 *
 * @return: if the word is valid and a solution to the boggle
 */
void computerWordSearchHelper(Grid<char>& board, Lexicon& dictionary, Set<string>& humanWords,
                              int index, int x, int y, string word, Vector<string>& used, Set<string>& words){

    if(index < board.numCols() * board.numRows() && board.inBounds(x, y)) {
        word += board[x][y];// forms the next word

        // add to used indicating we've already explored the word
        used.add(integerToString(x) + integerToString(y));
        string lower = toLowerCase(word);
        string upper = toUpperCase(word);

        if(word.size() >= BoggleGUI::MIN_WORD_LENGTH && dictionary.contains(lower) && !humanWords.contains(upper)) {
            if(!words.contains(word)) {
                words.add(word);
                score(false, word);
            }
        }

        if (dictionary.containsPrefix(lower)) {// check if there are any words starts with that prefix
            Vector< Vector<int> > neighbors = findNeighbors(board, x, y);
            int current_x, current_y;// x , y coordinates of current neighbor we are testing

            for (int i = 0; i < neighbors.size(); i++) {
                current_x = neighbors[i][0];
                current_y = neighbors[i][1];
                string string_xy = integerToString(current_x) + integerToString(current_y);

                if (!used.contains(string_xy)) {
                    computerWordSearchHelper(board, dictionary, humanWords,index + 1, current_x, current_y,
                                            upper, used, words);
                }
            }
        }

        //unexplored
        if (!used.isEmpty()) {
            used.removeBack();
        }
        word.substr(0,word.size());
    }
}

/**
 * @brief humanWordSearchHelper takes extra index, used x, y parameter returns
 *        if the word is valid and a solution to the boggle
 *
 * @param index: the current index we are at now
 * @param used: contains the word we already passed
 * @param x: x coordinate of the point
 * @param y: y coordinate of the point
 *
 * @return: if the word is valid and a solution to the boggle
 */
bool humanWordSearchHelper(Grid<char>& board, Lexicon& dictionary, string word,unsigned int index, Vector<string>& used, int x, int y) {
    toLowerCase(word);
    if (word.size() < BoggleGUI::MIN_WORD_LENGTH || !dictionary.contains(word)) {
        return false;
    } else if (index >= word.size() - 1) {
        return true;
    } else if (index == 0) { //if the index is 1 we find all point on grid that starts with that letter
        toUpperCase(word);

        // returns the coordinates of possible first letters
        Vector< Vector<int> > firstOne = findFirstOnes(board, word);

        int current_x, current_y;// current possible first letters x and y coordinates

        if(firstOne.size() == 0) {
            return false;
        } else {
            for(int i = 0; i < firstOne.size(); i++) {
                current_x = firstOne[i][0];
                current_y = firstOne[i][1];

                if(humanWordSearchHelper(board, dictionary, word,index + 1, used, current_x, current_y)){
                    BoggleGUI::clearHighlighting();
                    score(true, word);// scores if the word is valid
                    return true;
                }
            }

            BoggleGUI::clearHighlighting();
            return false;//if finishes the loop then we returns false because the word is invalid
        }

    } else {// if the index is not 1, then we do explore and unexplore process
        toUpperCase(word);

        //returns the all neighbors
        Vector< Vector<int> > neighbors = findNeighbors(board, x, y);

        int current_x, current_y;// x , y coordinates of current neighbor we are testing

        for (int i = 0; i < neighbors.size(); i++) {
            current_x = neighbors[i][0];
            current_y = neighbors[i][1];

            //string that contains x,y coordinates. it is for testing if we've already explore
            string string_xy = integerToString(current_x) + integerToString(current_y);
            BoggleGUI::setHighlighted(current_x, current_y, true);
            if(board[ current_x ][ current_y ] == word[index] && !used.contains(string_xy)) {
                used.add(string_xy);
                return humanWordSearchHelper(board, dictionary, word,index + 1, used, current_x, current_y);
            }
            BoggleGUI::setHighlighted(x, y, false);
        }

        if(!used.isEmpty()){     
            used.removeBack();
        }

        if(used.size() > 0){
            return humanWordSearchHelper(board, dictionary, word,index + 1, used, used[used.size() - 1][0], used[used.size() - 1][1]);
        } else {
            return humanWordSearchHelper(board, dictionary, word,index + 1, used, x, y);
        }
    }
}

/**
 * @brief return coordinates all target word in board
 *
 * @param word: the target word
 * @param board: the board that contains the values
 *
 * @return: coordinates all target word in board
 */
Vector< Vector<int> > findFirstOnes(Grid<char>& board,string word){
    Vector< Vector<int> > result;
    Vector<int> current_;// used to stores values of current possible first letter coordinates

    for (int i = 0; i < board.numRows(); i++) {
        for (int j = 0; j < board.numCols(); j++) {
            BoggleGUI::setHighlighted(i, j, true);
            if (board[i][j] == word[0]) {
                current_.add(i);
                current_.add(j);
                result.add(current_);

                current_.clear();
            }
            BoggleGUI::setHighlighted(i, j, false);
        }


    }
    return result;
}

/**
 * @brief turn coordinates all neighbors of the point at x ,y in board
 *
 * @param x: x coordinate of the target point
 * @param y: y coordinate of the target point
 * @param board: the board that contains the values
 *
 * @return vector of vectors contains coordinates of valid neighbors
 */
Vector< Vector<int> > findNeighbors(Grid<char>& board,int x,int y) {
    Vector<int> x_ {1 ,-1, 0, 0, 1, -1, -1, 1};// different x direction
    Vector<int> y_ {0, 0, 1, -1, 1, -1, 1, -1};// different y direction
    Vector< Vector<int> > result;
    Vector<int> current_;// used to stores values of current possible neighbor coordinates

    for (int i = 0; i < x_.size(); i++) {
        if (board.inBounds(x + x_[i], y + y_[i])) {
            current_.add(x + x_[i]);
            current_.add(y + y_[i]);
            result.add(current_);

            current_.clear();
        }
    }

    return result;
}

/**
 * @brief update the score board based on the length of word
 *
 * @param isHuman: if human scores
 * @param word: the word that is a valid solution
 */
void score(bool isHuman, string word){
    int points = 0;
    unsigned long length = word.length();

    if(length > 7) {
        points = 11;
    } else if (length > 6) {
        points = 5;
    } else if (length > 5) {
        points = 3;
    } else if (length > 4) {
        points = 2;
    } else {
        points = 1;
    }

    if (isHuman) {
        BoggleGUI::scorePointsHuman(points);
    } else {
        BoggleGUI::scorePointsComputer(points);
    }
}
