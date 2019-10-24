//Robin Gan
//This assignment simulates the game of life to a user

#include <iostream>
#include "console.h"
#include <fstream>
#include "filelib.h"
#include "grid.h"
#include "simpio.h"
#include "lifegui.h"
using namespace std;

void printIntro();

void getGridFromUser(Grid<string>& input_grid);

void printGrid(const Grid<string>& input_grid);

void tick(Grid<string>& input_grid);

string liveOrDie(const Grid<string>& input_grid,int row,int column);

int numOfCell(Vector<string>& input_vector);

Vector<string> getAroundValue(const Grid<string>& input_grid,int row,int column);

void startGame(Grid<string>& input_grid, bool& isPlayingTheGame);

void fillGrid(const Grid<string>& input_grid);

void updateGrid(const Grid<string>& input_grid);


int main() {
    LifeGUI::initialize();

    bool isPlaying = true;//if the player is still playing the game
    Grid<string> game;//the game board grid

    printIntro();

    while (isPlaying) {
        getGridFromUser(game);
        startGame(game,isPlaying);
    }

    cout << "Have a nice Life!" << endl;
    return 0;
}

/*
 *print out the introduction of the game
*/
void printIntro() {
    cout << "Welcome to the CS 106B/X Game of Life!" << endl;
    cout << "This program simulates the lifecycle of a bacterial colony." << endl;
    cout << "Cells (X) live and die by the following rules:" << endl;
    cout << "* A cell with 1 or fewer neighbors dies." << endl;
    cout << "* Locations with 2 neighbors remain stable." << endl;
    cout << "* Locations with 3 neighbors will create life." << endl;
    cout << "* A cell with 4 or more neighbors dies." << endl << endl;
}

/*
 * takes in a input grid, print out the input grid
 *
 * input_grid: the grid is going to be printed
 */
void printGrid(const Grid<string>& input_grid) {
    //loop through each cell in grid and print it out
    for (int i = 0; i < input_grid.numRows(); i++) {
        for (int j = 0; j < input_grid.numCols(); j++) {
            cout << input_grid[i][j];
        }
        cout << endl;
    }
}

/*
 *input_grid is the input grid, get grid fromtxt from user and assign it to the grid
 *
 * input_grid: this variable reference the grid in main method. This method will
 * fill that grid with input
 */
void getGridFromUser(Grid<string>& input_grid) {
    ifstream infile;
    promptUserForFile(infile, "Grid input file name?");

    string line;
    int index_of_line = 1,r = 0,c = 0;//index_of_line keeps track of the current row

    while (getline(infile,line)) {
        if (index_of_line == 1){// if index_of_line == 1, means that is the number of rows
            r = stringToInteger(line);
        }else if (index_of_line == 2) {// if index_of_line == 2, means that is the number of columns
            c = stringToInteger(line);
            input_grid.resize(r,c);
        }else if (index_of_line < r + 3) {// r+3: range of actual grid input
            for (int i = 0;i < line.length(); i++) {
                input_grid[index_of_line-3][i] = line[i];//index_of_line-3 is the current row number
            }
        }
        index_of_line++;
    }
    updateGrid(input_grid);
}

/*
 *takes in a input grid, tick the grid for one generation, uses in startGame method
 *
 * input_grid:this grid is updated by its next generation
 */
void tick(Grid<string>& input_grid) {
    Grid<string> copy = input_grid;
    for (int i = 0; i < input_grid.numRows(); i++) {
        for (int j = 0; j < input_grid.numCols(); j++) {
            input_grid[i][j] = liveOrDie(copy,i,j);
        }
    }
}

/*
 * takes in a input grid, return the string should be atthe position of the grid in
 * the next generation
 *
 * input_grid: the grid that has the cell been tested
 * row: the row of the cell of the cell is going to be tested
 * column: the column of the cell of the cell is going to be tested
 *
 * return: if the cell has a cell in the generation, it returns X, else returns -
 */
string liveOrDie(const Grid<string>& input_grid,int row,int column) {
    Vector<string> around_value = getAroundValue(input_grid,row, column);

    if (numOfCell(around_value) < 2) {
        return "-";
    }else if (numOfCell(around_value) == 2) {
        return input_grid[row][column];
    }else if (numOfCell(around_value) == 3) {
        return "X";
    }else{
        return "-";
    }
}

/*
 *takes in a input vector, return the number of live cell in the vector
 *
 * input_vector: the vector that contains value around a cell
 *
 * return: the number of live cell in the vector(cells around the cell)
 */
int numOfCell(Vector<string>& input_vector) {
    int count = 0;
    for (string cell : input_vector) {
        if(cell == "X") {
           count++;
        }
    }
    return count;
}

/*
 *This method return the vector that contains cells that around that cell
 *
 * input_grid: the grid that has the target cell
 * row: the row of the cell of the  target cell
 * column: the column of the cell of the target cell
 *
 * return: return the vector that contains cells that around that cell
 */
Vector<string> getAroundValue(const Grid<string>& input_grid,int row,int column) {
    string left,right,top,bottom,top_left,top_right,bot_left,bot_right;

    //check if the left right top bottom is on the side of grid
    left = (column == 0) ?
            input_grid[row][input_grid.numCols() - 1] :
            input_grid[row][column - 1];

    right = (column == input_grid.numCols() - 1) ?
            input_grid[row][0] : input_grid[row][column + 1];

    top = (row == 0) ?
            input_grid[input_grid.numRows() - 1][column]:
            input_grid[row - 1][column];

    bottom = (row == input_grid.numRows() - 1) ?
            input_grid[0][column] : input_grid[row + 1][column];

    int tem_row,tem_col;//tempertoary variable that change to coordinates of cell

    //check if the left right top bottom is on the side of grid,
    //then put two together to form a direction
    tem_col = (column == 0) ? input_grid.numCols() - 1 : column - 1;
    tem_row = (row == 0) ? input_grid.numRows() - 1 : row - 1;
    top_left = input_grid[tem_row][tem_col];

    tem_row = (row == input_grid.numRows() - 1) ? 0 : row + 1;
    bot_left = input_grid[tem_row][tem_col];

    tem_col = (column == input_grid.numCols() - 1) ? 0 : column+1;
    bot_right = input_grid[tem_row][tem_col];

    tem_row = (row == 0) ? input_grid.numRows() - 1 : row - 1;
    top_right = input_grid[tem_row][tem_col];

    Vector<string> output;
    output.add(left);
    output.add(right);
    output.add(bottom);
    output.add(top);
    output.add(top_left);
    output.add(top_right);
    output.add(bot_left);
    output.add(bot_right);

    return output;
}

/*
 * This method does the communication with user to complete the game.
 *
 * input_grid: the starting grid that user puts in
 * isPlayingTheGame: indicates if the player is still plaing the game
 */
void startGame(Grid<string>& input_grid, bool& isPlayingTheGame) {
    printGrid(input_grid);

    string answer = "";
    bool isFinish = false;

    while (!isFinish) {
        answer = getLine("a)nimate, t)ick, q)uit?");//get answer from user
        if (answer == "t" || answer == "T") {
            tick(input_grid);
            printGrid(input_grid);
            updateGrid(input_grid);
        }else if (answer == "a" || answer == "A") {
            int frames = getInteger("How many frames?");
            LifeGUI::resize(input_grid.numRows(), input_grid.numCols());
            for (int i = 0; i < frames; i++) {
                tick(input_grid);
                fillGrid(input_grid);
                LifeGUI::repaint();
                clearConsole();
                printGrid(input_grid);
                pause(100);
            }
        }else if (answer == "q" || answer == "Q") {
            bool wantAnotherFile = getYesOrNo("Load another file? (y/n)");
            if (wantAnotherFile) {
                break;
            }else {
                isPlayingTheGame = false;
                LifeGUI::shutdown();
                break;
            }
        }else{
            cout << "Invalid choice; please try again." << endl;
        }
    }
}

/*
 * takes in a input grid, uses fillCell() to fill the grid on GUI
 *
 * input_grid: the grid displays on GUI
 */
void fillGrid(const Grid<string>& input_grid) {
    LifeGUI::clear();
    for (int i = 0; i < input_grid.numRows(); i++) {
        for (int j = 0; j < input_grid.numCols(); j++) {
            if (input_grid[i][j] == "X") {
                LifeGUI::fillCell(i,j);
            }
        }
    }
}

/*
 * takes in a input grid, update new grid to GUI
 *
 * input_grid: the new grid should be updated on GUI
 */
void updateGrid(const Grid<string>& input_grid) {
    LifeGUI::resize(input_grid.numRows(), input_grid.numCols());
    fillGrid(input_grid);
    LifeGUI::repaint();
}
