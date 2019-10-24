/*
 * Robin Gan | Tommy Young
 * This is the class interface that states the instance variable and method head of member
 * functions and private helper functions.
 */

#ifndef _tilelist_h
#define _tilelist_h

#include <iostream>
#include <string>
#include "gwindow.h"
#include "tilenode.h"
using namespace std;

class TileList {
public:
    TileList();
    ~TileList();
    void addBack(int x, int y, int width, int height, string color);
    void addFront(int x, int y, int width, int height, string color);
    void clear();
    void debug();
    void drawAll(GWindow& window) const;
    TileNode* getBack() const;
    TileNode* getFront() const;
    bool highlight(int x, int y);
    bool lower(int x, int y);
    void merge(int x, int y);
    bool raise(int x, int y);
    bool remove(int x, int y);
    int removeAll(int x, int y);

private:
    TileNode* front;//the list node points to the most front element
    TileNode* back;//the list node points to the last element

    void deleteList(TileNode *frontNode);//delete the list memory used in destructor
    void attachFront(TileNode *& node);//attach a new node in the front
    void attachBack(TileNode *& node);//attach a new node in the back

    void detach(TileNode *& node);//remove the tile node
    TileNode* search(int x, int y);//return a node interested or return null pointer
    void moveFront(TileNode *& node);//move one tile node to the front

    //found the merge element and increase count.
    void mergeHelper(TileNode *& tile, TileNode *& front_, int& count,int x, int y,
                     int& min_x, int& min_y, int& max_x, int& max_y, string& color);
    TileNode* searchMerge(int x, int y, int count,TileNode * front_);//special search algorithm for merge method
};

#endif // _tilelist_h
