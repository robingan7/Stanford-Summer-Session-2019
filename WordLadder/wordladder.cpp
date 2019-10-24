//Robin Gan| Tommy Yang
//This is program that implments the word ladder algorithm

#include <iostream>
#include "console.h"
#include "lexicon.h"
#include "filelib.h"
#include "simpio.h"
#include "queue.h"
#include "stack.h"
#include "vector.h"
#include "strlib.h"
using namespace std;

void printIntro();

void getDictionaryFile(Lexicon& assign_dictionary);

void getWordOneAndTwo(string& word1,string& word2,bool& isStillPlaying,Lexicon& dictionary);

bool isValid(const string test_string, const Lexicon& dictionary);

void findLadder(const string end, const string start,const Lexicon& dictionary);

Vector<string> findNeighbor(const string input_string, const Lexicon& dictionary, Lexicon& used_word);

bool isFindEnd(Queue<Stack<string>> input_quene, const string target, const string start);

int main() {
    Lexicon main_dictionary;//dictionary that contains words from input dictionary
    string word1,word2;
    bool isPlaying=true;// indicates if the player is still playing

    printIntro();
    getDictionaryFile(main_dictionary);

    while(isPlaying) {
        getWordOneAndTwo(word1,word2,isPlaying,main_dictionary);

        if(isPlaying) {
            findLadder(word1,word2,main_dictionary);
        }
    }

    cout << "Have a nice day." << endl;
    return 0;
}

/*
 * print out the introduction message
 */
void printIntro() {
    cout << "Welcome to CS 106B/X Word Ladder!" << endl;
    cout << "Please give me two English words, and I will convert the" << endl;
    cout << "first into the second by modifying one letter at a time." << endl << endl;
}

/*
 * Get valid words from dictionary and assign them to dictionary in main method
 *
 * assign_dictionary: dictionary to be assigned
 */
void getDictionaryFile(Lexicon& assign_dictionary) {
    ifstream infile;
    string word;
    promptUserForFile(infile, "Dictionary file name:");

    while (getline(infile, word)) {
        assign_dictionary.add(word);
    }

    cout << endl;
    infile.close();
}

/*
 * Get valid words from user and assign them to word1 and word2 in main method
 * if the user enters nothing then ends the game.also check if the words are valid
 *
 * word1: words1 to be assigned
 * word2: words2 to be assigned
 * isStillPlaying: indicates if user wants to end the game
 * dictionary: the dictionary to check if word1 and word2 are is in it
 */
void getWordOneAndTwo(string& word1,string& word2,bool& isStillPlaying,Lexicon& dictionary) {

    while(true) {
        string wordOne = toLowerCase(getLine("Word 1 (or Enter to quit): "));
        if(wordOne.length() == 0){
            isStillPlaying = false;
            break;
        }
        string wordTwo = toLowerCase(getLine("Word 2 (or Enter to quit): "));
        if(wordTwo.length() == 0) {
            isStillPlaying = false;
            break;
        }

        if(wordTwo.length() != wordOne.length()) {
            cout << "The two words must be the same length." << endl << endl;
        }else if(!isValid(wordOne,dictionary) || ! isValid(wordTwo,dictionary)) {
            cout << "The two words must be found in the dictionary." << endl << endl;
        }else if(wordOne == wordTwo){
             cout << "The two words must be different." << endl << endl;
        }else {
            word1 = wordOne;
            word2 = wordTwo;
            break;
        }
    }
}

/*
 * check if the input word is a vaid English word that is in the dictionary
 *
 * test_string: the string is going to be tested
 * dictionary: the dictionary to check if the string is in it
 *
 * return: check if test_string is in the dictionary
 */
bool isValid(const string test_string,const Lexicon& dictionary) {
    return dictionary.contains(test_string);
}

/*
 * find the ladder from start to end string by implmenting the algorithm
 *
 * end: the last word of the ladder
 * start: the first word of the ladder
 * dictionary: the dictionary to check if string is valid
 */
void findLadder(const string start, const string end,const Lexicon& dictionary){
    Stack<string> initial_stack;// the first stack contains s1
    Queue<Stack<string>> ladder;
    Vector<string> neighbor;//neighbors of the current stack we are checking
    Lexicon used_word;// contains all the used words
    bool ifFindEnd = false;// if find the target

    initial_stack.push(start);
    ladder.enqueue(initial_stack);
    used_word.add(start);

    while(!ladder.isEmpty() && !ifFindEnd){
        Stack<string> s1 = ladder.dequeue();
        Stack<string> s1_copy = s1;
        neighbor=findNeighbor(s1.peek(),dictionary,used_word);

        for(int i = 0; i < neighbor.size();i++) {
            Stack<string> s2;
            s1.add(neighbor[i]);
            s2 = s1;
            ladder.enqueue(s2);
            s1 = s1_copy;
        }
        ifFindEnd = isFindEnd(ladder,end,start);//check if the target is found

    }
    if(!ifFindEnd){
        cout << "No word ladder found from "<< end <<" back to "
             << start << "." <<  endl << endl;
    }
}

/*
 * this medthod will find all the valid findNeighbor of the input string, then return the vector
 *
 * input_string: string that the method finds its neighbors
 * dictionary: the dictionary to check if string is valid
 * used_word: the dictionary to check if string is already used
 *
 * return: the vector that contains all neighbor of the string
 */
Vector<string> findNeighbor(const string input_string, const Lexicon& dictionary, Lexicon& used_word){
    Vector<string> result;// the result that contains all neighbors
    string copy = input_string;// use to reset copy in the end of each loop

    for (unsigned int i = 0;i < input_string.size();i++) {
        for (char ch = 'a'; ch <= 'z'; ch++) {// iterate through the letters
            copy[i] = ch;
            if(copy != input_string && dictionary.contains(copy) && !used_word.contains(copy)) {
                result.add(copy);
                used_word.add(copy);
            }
        }
        copy = input_string;
    }
    return result;
}


/*
 * test if the Queue has one valid ladder to target,
 * it also print out the stack that has the target in the end
 *
 * input_quene: the quene is tested
 * target: the target word that indicates the end of the ladder
 * start: the start of the ladder. To help print out the statement
 *
 * return: Queue has one valid ladder to target
 */
bool isFindEnd(Queue<Stack<string>> input_quene, const string target, const string start) {
    while(!input_quene.isEmpty()) {
        Stack<string> current_test_stack = input_quene.dequeue();
        if(current_test_stack.peek() == target) {

            cout << "A ladder from "<<target<<" back to "
                 << start << ":"<<  endl;

            while(!current_test_stack.isEmpty()) {
                //cout << current_test_stack.pop()+" ";
                //result_print.enqueueBack(current_test_stack.pop());
                cout << current_test_stack.pop() <<" ";
            }

            cout << endl << endl;
            return true;
        }
    }

    return false;
}

