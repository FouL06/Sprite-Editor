#include "frame.h"

/**
 * @brief Frame::Frame Default Constructor
 * @param sizeX
 * @param sizeY
 */
Frame::Frame(int sizeX, int sizeY)
{
    //Create the frames Pixel Array Columns and Rows and populate with default pixels.
    for(int i = 0; i < sizeX;i++){
        QVector<Pixel> pixelColumn(sizeY);
        pixels.push_back(pixelColumn);
    }
}
