#ifndef FRAME_H
#define FRAME_H

#include <QImage>
#include <pixel.h>

/**
 * @brief The Frame class uses a 2D array
 */
class Frame: public QImage
{
public:
    Frame(int sizeX, int sizeY);
    QString frameName;
    QVector<QVector<Pixel>> pixels;
};

#endif // FRAME_H
