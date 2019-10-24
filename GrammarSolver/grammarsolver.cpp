/* Robin Gan | Tommy Young
 * this is a program implements the grammar solver. We are storing input in a map.
 * Then, we uses BNF format to generate random sentences based on how many user wants and the BTN rules
 */
#include "grammarsolver.h"
#include "strlib.h"
#include "random.h"
#include "simpio.h"
#include "map.h"
#include "vector.h"
using namespace std;

/*
 *recursive part of the problem--generate the sentence from directory. Change the reference to
 * result string to the result we find
 *
 * map: contains the directory from input
 * symbol: the symbol we are starting
 * result: contains the final result
 */
void recursive(const Map< string, Vector<Vector<string>> >& map, string symbol,string& result) {
    //base case: if the symbol is not matching with any of keys, we return the symbol
    if (!map.keys().contains(symbol)) {
        result += symbol + " ";
    } else {
        //recursive case: choose a random element from map to get the next symbol
        Vector<string> random_vector = randomElement(map[symbol]);

        for (int i = 0 ; i < random_vector.size(); i++) {
            recursive(map, random_vector[i], result);
        }
    }
}

/*
 * stores the input in a map with BNF rules. Then uses "recursive" function to get
 * the result
 *
 * input: raw user input
 * symbol: nonterminal symbol that refer to the first rule
 * times: how many sentences the user wants
 *
 * return: a string vector with all the result ready to print
 */
Vector<string> grammarGenerate(istream& input, string symbol, int times) {
    string line;// line of input file
    Vector<string> result;//the result vector
    Map< string, Vector<Vector<string>> > non_terminal;//the map contains the directory of
    //non_terminal and terminal

    Vector<Vector<string> > current_content;//current line of sub directory
    string current_key;//current key of sub directory

    string result_string;//the string cotains result of vector

    while (getline(input,line)) {
        current_key = stringSplit(line, "::=")[0];

        if (non_terminal.keys().contains(current_key)) {
            throw("Can't have multiple "+current_key+" keys");
        }

        for (string s : stringSplit(stringSplit(line, "::=")[1], "|")) {
            current_content.add(stringSplit(trim(s) , " "));
        }

        non_terminal[current_key] = current_content;
        current_content.clear();
    }

    for (int i = 0; i < times ; i++) {
        recursive(non_terminal, symbol, result_string);
        result.add(result_string);
        result_string = "";
    }

    return result;
}
