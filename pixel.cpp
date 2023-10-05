#include "pixel.h"
#include "iostream"

/**
 * @brief Pixel::Pixel creates a standard pixel that is by default a soft white.
 */
Pixel::Pixel()
{    
    this->color = std::make_tuple(220,220,220,255);
}

/**
 * @brief Pixel::Pixel creates a colored pixel according to the rgba values passed in.
 * @param redValue
 * @param greenValue
 * @param blueValue
 * @param opacityValue
 */
Pixel::Pixel(int redValue, int greenValue, int blueValue, int opacityValue)
{
    this->color = std::make_tuple(redValue, greenValue, blueValue, opacityValue);
}

