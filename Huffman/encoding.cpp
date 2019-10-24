/*
 * Robin Gan | Tommy Young
 * This is the file that contains all the functions that runs compress and decompress the trees
 * as well as all the help functions
 */

#include "encoding.h"
#include "priorityqueue.h"
#include "filelib.h"
#include "strlib.h"

void buildEncodingMapHelper(Map<char, string>& result, HuffmanNode* node, string str);

void flattenTreeToHeaderHelper(string& result, HuffmanNode* t);

bool containInMap(Map<char, string>& map, string input, char& write);

int findNextParathenss(const string str);

/**
 * @brief take in a input file and count the frequency of each character put them in a map
 * @param input: the input that contains all the character input
 * @return the map that contains all the frequency
 */
Map<char, int> buildFrequencyTable(istream& input)
{
    Map<char, int>result;
    char ch;
    while (input.get(ch)) {
        result[ch]++;
    }
    return result;
}

/**
 * @brief build the encoding tree with frequency table
 * @param freqTable: the map that contains all the frequency of characters
 * @return the root of the tree
 */
HuffmanNode* buildEncodingTree(Map<char, int>& freqTable)
{
    PriorityQueue<HuffmanNode*> quene;//the priority quene that sort the frequency

    //create the intial table with node and its frequency as its priority
    for(char letter : freqTable){
        HuffmanNode* node = new HuffmanNode(letter);
        quene.enqueue(node, freqTable[letter]);
    }

    //the dequene process
    while(quene.size() > 1){
        int total_fre = 0;//the total frequency of interior

        //peek the first node
        total_fre+=quene.peekPriority();
        HuffmanNode* new_node = new HuffmanNode(nullptr, nullptr);
        new_node->zero = quene.dequeue();

        //peek the second node
        total_fre+=quene.peekPriority();
        new_node->one = quene.dequeue();

        quene.enqueue(new_node, total_fre);//enquene the new node
    }
    return quene.dequeue();
}

/**
 * @brief flatten the tree to a string header
 * @param t: the root of tree
 * @return the header of tree
 */
string flattenTreeToHeader(HuffmanNode* t)
{
    string result = "";//the result(header)
    flattenTreeToHeaderHelper(result, t);

    return result;
}

/**
 * @brief encode a new tree with a header
 * @param str: the header
 * @return the root of return tree
 */
HuffmanNode* recreateTreeFromHeader(string str)
{
    if(str[0] == '.') {//base case --check if the string starts with dot
        return new HuffmanNode(str[1]);
    } else{
        HuffmanNode* zero_node;//the zero node of interior
        HuffmanNode* one_node;//the one node of interior

        if(str[1] == '.'){
            zero_node = recreateTreeFromHeader(str.substr(1, 2));//get the character value

            //(1 + 2) the first index of the rest of string, we need to plus 1 to 2 to get the index
            //since it is the length after the index above we need to minus 1 more to the index
            one_node = recreateTreeFromHeader(str.substr(1 + 2, str.size() - (1 + 2 + 1)));//get the rest of the string
        } else {
            int pivot = findNextParathenss(str);//find the point that seperate the string

            //find the first interior that seperates
            zero_node = recreateTreeFromHeader(str.substr(1, pivot - 1));

            //find the second interior that seperates
            one_node = recreateTreeFromHeader(str.substr(pivot, str.size() - pivot - 1));
        }
        return new HuffmanNode(zero_node, one_node);
    }
}

/**
 * @brief build a map that contains character and its code in the tree
 * @param encodingTree: the root of the encoding tree
 * @return the map that contains character and its code in the tree
 */
Map<char, string> buildEncodingMap(HuffmanNode* encodingTree)
{
    Map<char, string> result;//the result
    string str = "";//the current encode value
    buildEncodingMapHelper(result, encodingTree, str);
    return result;
}

/**
 * @brief freeTree: delete all tree node
 * @param t: the root of the tree
 */
void freeTree(HuffmanNode* t)
{
    if (t == nullptr) {
        return;
    }
    freeTree(t->zero);
    freeTree(t->one);
    delete t;
}

/**
 * @brief compress: compress a input to bits and header form
 * @param input: input file usually a paragraph of text
 * @param output: writes the result of this function
 */
void compress(istream& input, HuffmanOutputFile& output)
{
    //uses helper function above to assign frequency table and tree root. header
    Map<char, int> result = buildFrequencyTable(input);
    HuffmanNode* node = buildEncodingTree(result);
    Map<char, string> map = buildEncodingMap(node);
    string header = flattenTreeToHeader(node);

    freeTree(node);//free the tree we are no longer need

    output.writeHeader(header);//writes to output
    rewindStream(input);

    //interation through the input and writes bit to output
    char ch;
    while (input.get(ch)) {
        string tem = map[ch];//get the bits for this string
        for(int i = 0; i < tem.length(); i++){
            //seperate each bits to add to output
            output.writeBit(stringToInteger(tem.substr(i, 1)));
        }
    }
}

/**
 * @brief decompress: inverse of compress
 * @param input: the file that contains result from compress
 * @param output: the result of output(input of compress)
 */
void decompress(HuffmanInputFile& input, ostream& output)
{
    string header = input.readHeader();//input header
    HuffmanNode* tree = recreateTreeFromHeader(header);
    Map<char, string> map = buildEncodingMap(tree);

    freeTree(tree);//free the tree we're no longer use

    string tem = "";//bit we combine so far
    int currentBit = input.readBit();//current bit we have(used to determine the end loop)
    char writeChar = 'd';//char to write

    while(currentBit != -1) {
        tem += integerToString(currentBit);//add the new bit to
        if(containInMap(map, tem, writeChar)) {//check if the bits are indicated to a character
            output << writeChar;
            tem = "";//reset the tempoarary string
        }
        currentBit = input.readBit();//update the next bits
    }
}

/**
 * @brief use recursion to find the header
 * @param result: the string that is the result
 * @param t: the root of tree
 */
void flattenTreeToHeaderHelper(string& result, HuffmanNode* t) {
    if (t->isLeaf()) {//base case
        result += ".";
        result += t->ch;
        return;
    }
    result += "(";
    flattenTreeToHeaderHelper(result, t->zero);
    flattenTreeToHeaderHelper(result, t->one);
    result += ")";
}

/**
 * @brief use recursion to find the code for each character
 * @param result: the result map
 * @param node: current node we are at
 * @param str: bit code we have so far
 */
void buildEncodingMapHelper(Map<char, string>& result, HuffmanNode* node, string str) {
    if(node->isLeaf()){//base case
        result[node->ch] = str;
        return;
    }
    str += "0";
    buildEncodingMapHelper(result, node->zero, str);

    str  = str.substr(0, str.length() - 1);//delete the last character
    str += "1";
    buildEncodingMapHelper(result, node->one, str);
}

/**
 * @brief check if the input is lead to a character
 * @param map: contains charcacter with its bit code
 * @param input: the input bit
 * @param write: character we find to write to output
 * @return if the input bits is a avaliable
 */
bool containInMap(Map<char, string>& map, string input, char& write) {
    for (char ch: map) {//iterate through the map
        if(map[ch] == input) {//check if the character has input same as its bit code
            write = ch;//update the character we find
            return true;
        }
    }
    return false;
}

/**
 * @brief find the index of next parathness(the pivot that seperates the nodes of interior)
 * @param str: the input str that contains a node that includes two interiors
 * @return the pivot that seperates the nodes of interior
 */
int findNextParathenss(const string str) {
    int pivot = 2;//the pivot now that since I didn't substring the origin parethess
    int level = 1;//when it goes to zero that means we find the other ')'

    while(level != 0){
        if(str[pivot] == '(') {
            level++;
        } else if (str[pivot] == ')') {
            level--;
        }
        pivot++;
    }

    return pivot;
}
