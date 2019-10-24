/*
 * Robin Gan | Tommy Young
 * This program implements the methods and private variable that are declares in the interface
 * It contains the actual algorithm of the method I suppose to write with link list
 */

#include "tilelist.h"
#include <algorithm>
#include "strlib.h"
using namespace std;

const string HighLightColor = "yellow";

/**
 * @brief the constructor
 * O(1)
 */
TileList::TileList() {
    front = nullptr;//set the front and back to null pointer
    back = nullptr;//set the back to null pointer

}

/**
 * @brief the destructor
 * O(N)
 */
TileList::~TileList() {
    deleteList(front);//delete the whole list
}

/**
 * @brief add a new node in the back
 * @param x: the x coordinate of the new node
 * @param y: the x coordinate of the new node
 * @param width: the width of the new node
 * @param height: the height of the new node
 * @param color: the color of the new node
 * O(1)
 */
void TileList::addBack(int x, int y, int width, int height, string color) {
    TileNode* tem = new TileNode(x, y, width, height, color, back, nullptr);//the new tile
    back = tem;//set back to the new back tile

    TileNode* ptr = back->prev;//change back prev
    if (ptr != nullptr) {
        ptr->next = back;
    }

    if (front == nullptr) {
        front = tem;//if front is null then the back is also the front
    }
}

/**
 * @brief add a new node in the front
 * @param x: the x coordinate of the new node
 * @param y: the x coordinate of the new node
 * @param width: the width of the new node
 * @param height: the height of the new node
 * @param color: the color of the new node
 * O(1)
 */
void TileList::addFront(int x, int y, int width, int height, string color) {
    TileNode* newTile = new TileNode(x, y, width, height, color, nullptr, front);//the front tile
    front = newTile;//set front to the new front tile

    TileNode* ptr = front->next;//change front next
    if(ptr != nullptr) {
        ptr->prev = front;
    }

    if (back == nullptr) {
        back = newTile;//if back is null then the front is also the back
    }
}

/**
 * @brief delete the list, and reset front and back to null pointer
 * O(N)
 */
void TileList::clear() {
   deleteList(front);//clear the whole list

   //set front and back to null pointer
   front = nullptr;
   back = nullptr;
}
/**
 * @brief print out the whole from the front
 * O(N)
 */
void TileList::debug() {
    for (TileNode* ptr = front; ptr != nullptr; ptr = ptr->next) {
        cout << ptr->prev << "/" << ptr << "/"<<
             ptr->next<< endl;
    }
}

/**
 * @brief iteration through the list backward, then draws the each tile
 * @param window: the graphics window that draws tiles.
 * O(N)
 */
void TileList::drawAll(GWindow& window) const {
    for (TileNode* ptr = back; ptr != nullptr; ptr = ptr->prev) {
        ptr->draw(window);
    }
}

/**
 * @brief get the back node of the list
 * @return the back node of the list
 * O(1)
 */
TileNode* TileList::getBack() const {
    return back;
}

/**
 * @brief get the back node of the list
 * @return the back node of the list
 * O(1)
 */
TileNode* TileList::getFront() const {
    return front;
}

/**
 * @brief highlight the first tile touch x and y with yellow
 * @param x: x coordinate where the user click
 * @param y: y coordinate where the user click
 * @return if high light at least one tile or not
 * O(N)
 */
bool TileList::highlight(int x, int y) {
    TileNode* result = search(x,y);//search for the first result

    if(result != nullptr) {
        result->color = HighLightColor;//high light it with yellow
        return true;
    }else {
        return false;
    }
}

/**
 * @brief move the target tile in the backmost of list
 * @param x: x coordinate where the user clicks
 * @param y: y coordinate where the user clicks
 * @return if lower at least one tile or not
 * O(N)
 */
bool TileList::lower(int x, int y) {
    TileNode* low = search(x, y);//find the first result

    if(low != nullptr && low != back){//check if the result is null or already is back
        if (low->prev != nullptr) {//check if low is front

            //rewiring list
            low->prev->next = low->next;
            low->next->prev = low->prev;
            back->next = low;
            low->prev = back;
            low->next = nullptr;
            back = low;
        }else {
            front = low->next;
            back->next = low;
            low->next->prev = nullptr;
            low->prev = back;
            low->next = nullptr;
            back = low;
        }
        return true;
    } else {
        return false;
    }
}

/**
 * @brief merge the target tiles to one big tile, and moves it to the top most of list
 * @param x: x coordinate where the user clicks
 * @param y: y coordinate where the user clicks
 * @return if merge at least one tile or not
 * O(N^2)
 */
void TileList::merge(int x, int y) {
    int count = 0;//how many element found
    TileNode* tile = front;//the current result tile
    TileNode* tileFront = front;//the frontmost result

    int min_x = 0;//minium x value
    int min_y = 0;//minium y value

    int max_x = 0;//maxium x value
    int max_y = 0;//maxium y value
    string color = "";

    //called mergeHelper to updates values we want
    mergeHelper(tile, tileFront, count, x, y, min_x, min_y, max_x, max_y, color);

    if (count > 0) {//check if we find tile to merge
        //then updates value
        tileFront->height = max_y - min_y;
        tileFront->width = max_x - min_x;
        tileFront->x = min_x;
        tileFront->y = min_y;
        tileFront->color = color;
        if(tileFront != front) {//check the tile is already front
            moveFront(tileFront);
        }
    }
}

/**
 * @brief move the target tile in the topmost of list
 * @param x: x coordinate where the user clicks
 * @param y: y coordinate where the user clicks
 * @return if lower at least one tile or not
 * O(N)
 */
bool TileList::raise(int x, int y) {
    TileNode* high = search(x, y);

    if(high != nullptr && high != front){
        moveFront(high);
        return true;
    } else {
        return false;
    }
}

/**
 * @brief remove the first element that x, y touches
 * @param x: x coordinate that user click
 * @param y: y coordinate that user click
 * @return if remove at least one element or not
 * O(N)
 */
bool TileList::remove(int x, int y) {
    TileNode* result = search(x,y);//found the result, return null
    //if we don't need to remove any node

    if(result != nullptr) {
        detach(result);
        return true;
    }else {
        return false;
    }
}

/**
 * @brief remove all element that x, y touches
 * @param x: x coordinate that user click
 * @param y: y coordinate that user click
 * @return how many elements we removed
 * O(N^2)
 */
int TileList::removeAll(int x, int y) {
    int count = 0;//result counter
    TileNode* tile = front;

    while(tile != nullptr) {//see if we remove all elementa
        tile = search(x, y);
        if (tile != nullptr) {
            count++;
            detach(tile);
        }
    }
    return count;
}

/**
 * @brief delete the whole list using recursion
 * @param frontNode: the front node of the list that supposed to be removed
 */
void TileList::deleteList(TileNode *frontNode){
    if (frontNode == nullptr) {
        return;
    }
    deleteList(frontNode->next);
    delete frontNode;
}

/**
 * @brief detach the node from link list
 * @param node: the node that supposed to be removed
 */
void TileList::detach(TileNode *& node) {
    TileNode* trash = node;

    if (node->next != nullptr) {//check if node is back node
        if(node->prev != nullptr) {//check if node is front node
            node->prev->next = node->next;
            node->next->prev = node->prev;
        } else{
            node->next->prev = nullptr;
            front =  node->next;
        }
    } else {
        if(node->prev != nullptr) {//check if node is front node
            node->prev->next = nullptr;
            back = node->prev;

            if(node->prev->prev == nullptr) {//check if it is the node next to front node
                front = node->prev;
            }
        } else {
            front = nullptr;
            back = nullptr;
        }
    }
    delete trash;//deletes the memory
}

/**
 * @brief return the first node that contains x, y, else return nullptr
 * @param x:x coordinate of target node
 * @param y:y coordinate of target node
 * @return the node we found or nullptr
 */
TileNode* TileList::search(int x, int y){
    for (TileNode* ptr = front; ptr != nullptr; ptr = ptr->next) {
        if (ptr->contains(x, y)) {//check if the pointer is touch by x,y
            return ptr;
        }
    }
    return nullptr;
}

/**
 * @brief this method will move the target node to the front
 * @param node: the target node
 */
void TileList::moveFront(TileNode *& node){
    if (node->next != nullptr) {//check if the node is back node

        //rewiring the list
        node->next->prev = node->prev;
        node->prev->next = node->next;
        node->next = front;
        front->prev = node;
        node->prev = nullptr;
        front = node;
    }else {
        back = node->prev;
        node->prev->next = nullptr;
        front->prev = node;
        node->next = front->next;
        node->prev = nullptr;
        front = node;
    }
}

/**
 * @brief the this a special forn merge method. It avoids to search for front again and again
 * @param x: the x coordinate of touch point
 * @param y: the y coordinate of touch point
 * @param count: how many element we found so far
 * @param front: the front element we should avoid
 * @return the tile we found
 */
TileNode* TileList::searchMerge(int x, int y, int count,TileNode* front_) {
    for (TileNode* ptr = front_; ptr != nullptr; ptr = ptr->next) {
        if (ptr->contains(x, y)) {

            //if it is other than the front most element if count is greater than 1
            if((count > 0 && ptr != front_) || count == 0) {
                return ptr;
            }
        }
    }
    return nullptr;
}

/**
 * @brief found all the merge, and increase count
 * @param tile: the tile that is touched by  x, y
 * @param front_: the frontmost element
 * @param count: how many element are found
 * @param x: x coordinate of touch point
 * @param y: y coordinate of touch point
 * @param min_x: the min value of x
 * @param min_y: the min value of y
 * @param max_x: the max value of x
 * @param max_y: the max value of y
 * @param color: the color of the frontmost tile
 */
void TileList::mergeHelper(TileNode*& tile, TileNode*& front_, int& count, int x, int y,
                           int& min_x, int& min_y, int& max_x, int& max_y, string& color) {
    while (tile != nullptr) {
        tile = searchMerge(x, y,count,front_);//search result other than front most result

        if (tile != nullptr) {
            count++;
            if (count == 1) {
                //for the first result we set max, min values to this temperoary
                front_ = tile;
                min_x = front_->x;
                min_y = front_->y;

                max_x = front_->x + front_->width;
                max_y = front_->y + front_->height;
                color = front_->color;
            } else {
                count++;

                //check if the result can update min or max x and y values
                if (tile->x < min_x) {
                    min_x = tile->x;
                }
                if (tile->y < min_y) {
                    min_y = tile->y;
                }
                if (tile->x + tile->width > max_x) {
                    max_x = tile->x + tile->width;
                }
                if (tile->y + tile->height > max_y) {
                    max_y = tile->y + tile->height;
                }
                detach(tile);//remove this node
            }
        }
    }
}
