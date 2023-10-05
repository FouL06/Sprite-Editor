#ifndef PIXEL_H
#define PIXEL_H

#include <tuple>

/**
 * @brief The Pixel class represents a single pixel to be drawn to the UI.
 */
class Pixel
{
public:
    Pixel();
    Pixel(int redValue, int greenValue, int blueValue, int opacityValue);
    std::tuple<int,int,int,int> color;
};

#endif // PIXEL_H
