//Robin Gan | Tommy Yang
//This program simulates the n-gram program

#include <iostream>
#include "console.h"
#include "random.h"
#include "lexicon.h"
#include "filelib.h"
#include "simpio.h"
#include "map.h"
#include "queue.h"
using namespace std;

void printIntro();

void getFile(Vector<string>& words);

void generateMap(Map<Vector<string>, Vector<string>>& map,
                 const Vector<string>& words,
                 Vector< Vector<string> >& prefix_list);

void generateRandomText(Map<Vector<string>, Vector<string>>& map,
                        Vector< Vector<string> >& prefix_list);

int main() {
    Vector<string> all_words;//contains all words from input
    Map<Vector<string>, Vector<string>> word_map;//word map that is used to generate random words
    Vector< Vector<string> > all_prefix;//contains all the prefix in word_map

    printIntro();
    getFile(all_words);
    generateMap(word_map, all_words, all_prefix);
    generateRandomText(word_map, all_prefix);

    cout << "Exiting." << endl;
    return 0;
}

/*
 * print out the introduction message
 */
void printIntro() {
    cout << "Welcome to CS 106B/X Random Writer ('N-Grams')!" << endl;
    cout << "This program generates random text based on a document." << endl;
    cout << "Give me an input file and an 'N' value for groups" << endl;
    cout << "of words, and I'll create random text for you." << endl << endl;
}


/*
 * Fill Queue with all words from input file
 *
 * words: the Queue is going to be filled with words
 */
void getFile(Vector<string>& words) {
    ifstream infile;
    promptUserForFile(infile, "Input file name?");
    string word;

    while (infile >> word) {
        words.add(word);
    }

    infile.close();
}

/*
 * ask user for value of N, then genrate the map
 *
 * map: the map is going to be generated.
 * words: contains all the words from text
 * prefix_list: contains all the prefix in map
 */
void generateMap(Map<Vector<string>, Vector<string>>& map,
                 const Vector<string>& words,
                 Vector< Vector<string> >& prefix_list) {
    while(true) {
        int gram = getInteger("Value of N?");
        if(gram < 2) {
            cout << "N must be 2 or greater." << endl;
        }else if(gram <= words.size()) {
            int currentIndex;
            Vector<string> prefix;
            string suffix;

            //Add the map element that are in normal range
            for(int i = 0; i < words.size() - gram + 1;i++) {
                for(int j =0 ; j < gram-1 ; j++) {
                    prefix.add(words[i+j]);
                }
                suffix = (words[i + gram - 1]);
                map[prefix].add(suffix);

                prefix_list.add(prefix);

                prefix.clear();
                suffix = "";
            }

            //Add the map that are out of normal range(wrap around)
            for(int i = 0; i < gram - 1 ; i++) {
                for(int j = 0 ; j < gram-1 ; j++) {
                    //Keep track of the current index we are adding to the map
                    currentIndex = words.size() - gram + 1 + j + i;

                    if(currentIndex < words.size()){
                         prefix.add(words[currentIndex]);
                    }else{
                         prefix.add(words[currentIndex - words.size() ]);
                    }

                }

                suffix = words[0 + i];
                map[prefix].add(suffix);

                prefix_list.add(prefix);

                prefix.clear();
                suffix = "";
            }

            break;
        }else{
            cout << "N cannot be greater than the number of words in the file" << endl;
        }
    }
    cout << endl;
}

/*
 * Ask user for how many words to generate, generate the random text
 *
 * map: the map is used to generate the map.
 * prefix_list: contains all the prefix in map
 */
void generateRandomText(Map<Vector<string>, Vector<string>>& map,Vector< Vector<string> >& prefix_list) {
    while(true) {
        int numOfWords = getInteger("# of random words to generate (0 to quit)?");

        // this is the size of each prefix of map.Indicates the minium random words to generate
        int startNumOfWord;
        int randomIndexMax; //Maxium random value

        if(numOfWords != 0) {
             // currently random vector element
            Vector<string>current_prefix = prefix_list[randomInteger(0, prefix_list.size() - 1)];
            // start index of number of word for program to generate the next word
            startNumOfWord = current_prefix.size();

            if(numOfWords >= startNumOfWord + 1){
                cout << "... ";
                for(string element : current_prefix) {
                    cout << element<<" ";
                }

                for(int i = 0; i < numOfWords - startNumOfWord; i++) {
                    randomIndexMax = map[current_prefix].size() - 1;

                    string nextWord = map[current_prefix][ randomInteger(0 , randomIndexMax) ];//random choose the next word

                    cout << nextWord << " ";

                    current_prefix.remove(0);
                    current_prefix.add(nextWord);
                }
                cout << "..." << endl << endl;

            }else {
                cout << "Must be at least " << startNumOfWord + 1 << " words." << endl << endl;
            }

        }else {
            break;
        }
    }

}

