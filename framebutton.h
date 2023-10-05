#ifndef FRAMEBUTTON_H
#define FRAMEBUTTON_H

#include "frame.h"
#include <QPushButton>

/**
 * @brief The FrameButton class represents a push button that will
 * appear at the bottom of MainWindow. When this button is pressed,
 * the frame associated with it will be displayed. As frames are deleted
 * and added, these buttons must be renamed accordingly. They are stored
 * in an array in MainWindow and every time update is called, the buttons
 * order should also update, if necessary after aforementioned add/deletes.
 */
class FrameButton: public QPushButton
{
public:
    FrameButton();
    QString frameButtonName;
};

#endif // FRAMEBUTTON_H
