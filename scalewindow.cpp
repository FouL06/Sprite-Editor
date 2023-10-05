#include "scalewindow.h"

scaleWindow::scaleWindow(QWidget *parent) : QGraphicsView (parent)
{
    setMouseTracking(true);

    //this will live here or else build errors will occur due to how qt creator
    //is building works and its inheritence properties in ui components.
    //be my guest to try and delete this and not spend 4 hours fixing errors
    //that are out of your control due to this editor.
}
