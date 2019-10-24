/* Robin Gan | Tommy Young
 * the program that implements triangle and flood fill recursive program.
 * triangle: uses recursion to draw Sierpinski Triangle based on the first coordinate and side
 * length and order
 * flood fill: uses recursion to select the "fill" color and selecting a color grid to change all
 * pixels in that grid
 */

#include "fractals.h"
#include "error.h"
using namespace std;

void drawTriangle(GWindow& window, double top_left_x, double top_left_y, double size);

int floodFillHelper(GWindow &window, int x, int y, int color, int last_color);

/*
 * uses recursion to draw Sierpinski Triangle based on the first coordinate and side length
 *
 * window: the canvas we are drawing the result on
 * x: x of the first coordinate
 * y: y of the first coordinate
 * size: side length of the first triangle
 * order: how many orders we are generating
 */
void drawSierpinskiTriangle(GWindow& window, double x, double y, double size, int order) {
    if (order >= 1) {
        drawTriangle(window, x, y, size);

        // "/ 2" is to get half length of the side
        drawSierpinskiTriangle(window, x, y,size / 2, order - 1);

        // "/ 4" is to get quarter length of the side
        // "/ 2" is to get half length of the side
        drawSierpinskiTriangle(window, x + size / 4,
                               y + (size / 2) * sinDegrees(60),
                               size / 2, order - 1);

        // "/ 2" is to get half length of the side
        drawSierpinskiTriangle(window, x + size / 2,
                               y,size / 2, order - 1);// "/ 2" is to get half length of the side

    } else if (x < 0 || y < 0 || size < 0 || order < 0) {
        throw("value can't be negative");
    }

}

/*
 * draw a triangle from size, x, y corrdinate as top/left
 *
 * top_left_x: x coordinate
 * top_left_y: y coordinate
 * size: length of that triangle
 * window: graphics window
 */
void drawTriangle(GWindow& window, double top_left_x, double top_left_y, double size) {
    // top right x coordinate
    double top_right_x = top_left_x + size;

    // bottom x coordinate "/ 2" is to get half length of the side
    double bottom_x = top_left_x + size / 2;

    //bottom y coordinate
    double bottom_y = top_left_y + size * sinDegrees(60);

    //draw the triangle with those coordinates
    window.drawLine(top_left_x, top_left_y, top_right_x,top_left_y);
    window.drawLine(top_left_x, top_left_y, bottom_x,bottom_y);
    window.drawLine(top_right_x, top_left_y, bottom_x,bottom_y);
}

/*
 * uses recursion to select the "fill" color and selecting a color grid to change all
 * pixels in that grid
 *
 * window: the window we are drawing on
 * x: x of teh first pixel
 * y: y of teh first pixel
 * color: the fill color
 *
 * return: how many pixel are painted
 */
int floodFill(GWindow& window, int x, int y, int color) {
    if (window.getPixel(x, y) == color) {
        return 0;
    } else if(x < 0 || y < 0 || !window.inCanvasBounds(x, y)) {
        throw "invalid in input";
    } else { //the pixel is not equal to color paint the pixel and result is 1, or return 0
        int result = 1;//how many pixel paint
        int current_color = window.getPixel(x, y);// hold the color of current pixel

        window.setPixel(x, y, color);

        //check if we should explore the four direction
        result += floodFillHelper(window, x + 1, y, color, current_color);
        result += floodFillHelper(window, x - 1, y, color, current_color);
        result += floodFillHelper(window, x, y + 1, color, current_color);
        result += floodFillHelper(window, x, y - 1, color, current_color);

        return result;
    }
}

/*
 * the helper method take an additional paraementer isGoing
 *
 * window, x, y,color: parameter that are passed from fillFlood, they has the same functions
 * last_color: indicates the original color of last pixel
 *
 * return: if the value we are going to explore is out of bounds or it has a different color
 * then we return 0, if not we are returning the recursive case
 */
int floodFillHelper(GWindow &window, int x, int y, int color, int last_color) {
    if (!window.inCanvasBounds(x, y) || window.getPixel(x, y)  != last_color) {
        return 0;
    } else {
        return floodFill(window, x, y, color);
    }
}
