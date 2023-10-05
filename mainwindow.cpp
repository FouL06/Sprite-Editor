#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QTextStream>
#include <iostream>

/**
 * @brief MainWindow::MainWindow Constructor
 * @param parent
 */
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    setMouseTracking(true);
    this->setFixedSize(QSize(1100, 800));
    menuBar()->setNativeMenuBar(false);
    ui->setupUi(this);

    //write config file and check to see if it exists
    QFile file("config.txt");
    if(!file.exists()){
        if(file.open(QIODevice::ReadWrite | QIODevice::Text)){
            file.write(QByteArray("darkmode:false"));
            file.flush();
            file.close();
        }
    }
    else{
        if(file.open(QIODevice::ReadOnly)){
            QString line = file.readAll();
            if(line == "darkmode:false"){
                lightMode();
            }
            else{
                darkMode();
            }
        }
    }

    //User Interface instance variables.
    togglePlayback = false;
    toggleEraser = false;
    sizeX = 500;
    sizeY = 500;
    currentFrame = 0;
    brushValue = 10;

    Frame initialFrame = Frame(sizeX,sizeY);
    FrameButton initialFrameButton = FrameButton();
    frameArray.push_back(initialFrame);

    //initial frame upon new project
    image = QImage(sizeX, sizeY, QImage::Format_ARGB32);
    for(int width = 0; width < sizeX; width++){
        for(int length = 0; length < sizeY; length++){
            image.setPixel(width, length, qRgba(220, 220, 220, 255));
        }
    }

    update();
    updateColorPreviewFrame();

    //Signals and Slots
    connect(ui->EraserButton, &QPushButton::toggled, this, &MainWindow::on_EraserButton_clicked);
    connect(ui->actionAbout_2, &QAction::triggered, this, &MainWindow::showAboutMessage);
    connect(ui->actionSave,&QAction::triggered,this, &MainWindow::saveProject);
    connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::loadProject);
    connect(ui->actionNew, &QAction::triggered, this, &MainWindow::newProject);
}

/**
 * @brief MainWindow::~MainWindow Destructor
 */
MainWindow::~MainWindow()
{
    delete ui;
}

/**
 * @brief MainWindow::update Updates UI stylesheets and frames.
 */
void MainWindow::update()
{
    QGraphicsScene *graphic = new QGraphicsScene(this);
    graphic -> addPixmap(QPixmap::fromImage(image));
    ui->canvasDisplay->setScene(graphic);
    image.paintingActive();
}

/**
 * @brief MainWindow::ChangeFrame This method will change the currently
 * active frame depending on the frame the user selects in the UI.
 * It should be called when the user either clicks Add Frame or a Frame
 * Button.
 *
 * @param arrayIndex The index in the frame array that is needed
 */
void MainWindow::changeFrame(int arrayIndex)
{
    currentFrame = arrayIndex;
    for(int currentX = 0; currentX < sizeX; currentX++){
        for(int currentY = 0; currentY < sizeY; currentY++){
             image.setPixel(currentX, currentY, qRgba(std::get<0>(frameArray[arrayIndex].pixels[currentX][currentY].color),
                                                      std::get<1>(frameArray[arrayIndex].pixels[currentX][currentY].color),
                                                      std::get<2>(frameArray[arrayIndex].pixels[currentX][currentY].color),
                                                      std::get<3>(frameArray[arrayIndex].pixels[currentX][currentY].color)));
        }
    }
    update();
}

/**
 * @brief MainWindow::on_redSpinBox_valueChanged
 * Update r color value of Pixel class using a model-view signal.
 */
void MainWindow::on_redSpinBox_valueChanged()
{
    updateColorPreviewFrame();
}

/**
 * @brief MainWindow::on_greenSpinBox_valueChanged
 * Update g color value of Pixel class using a model-view signal.
 */
void MainWindow::on_greenSpinBox_valueChanged()
{
    updateColorPreviewFrame();
}

/**
 * @brief MainWindow::on_blueSpinBox_valueChanged
 * Update b color value of Pixel class using a model-view signal.
 */
void MainWindow::on_blueSpinBox_valueChanged()
{
    updateColorPreviewFrame();
}

/**
 * @brief MainWindow::on_opacitySpinBox_valueChanged
 * Update opacity value of Pixel class using a model-view signal.
 */
void MainWindow::on_opacitySpinBox_valueChanged()
{
    updateColorPreviewFrame();
}

/**
 * @brief MainWindow::updateColorPreviewFrame
 * Updates preview pixel color widget to current pixel color selected by the users input
 * to the rgba value spinboxes.
 */
void MainWindow::updateColorPreviewFrame(){
    redValue = ui->redSpinBox->value();
    greenValue = ui->greenSpinBox->value();
    blueValue = ui->blueSpinBox->value();
    opacityValue = ui->opacitySpinBox->value();

    QString backgroundColor = "QFrame {background-color: rgba(" +
            QString::number(redValue) +","+ QString::number(greenValue)+
            ","+QString::number(blueValue)+"," + QString::number(opacityValue)+")}";
    ui->colorPreviewFrame->setStyleSheet(backgroundColor);
}

/**
 * @brief MainWindow::on_brushSizeSpinBox_valueChanged
 * Updates current brush size the user would like to use.
 */
void MainWindow::on_brushSizeSpinBox_valueChanged()
{
    switch(ui->brushSizeSpinBox->value())
    {
    case 1:
        brushValue = 10;
        break;
    case 2:
        brushValue = 20;
        break;
    case 3:
        brushValue = 30;
        break;
    case 4:
        brushValue = 40;
        break;
    default:
        break;
    }

}

/**
 * @brief MainWindow::on_fpsSpinBox_valueChanged
 * Updates playback fps value for use to edit and sets the fps value
 */
void MainWindow::on_fpsSpinBox_valueChanged()
{
    fps = ui->fpsSpinBox->value();
}

/**
 * @brief MainWindow::mousePressEvent
 *
 * @param event
 */
void MainWindow::mousePressEvent(QMouseEvent *event)
{
    int x = event->pos().x()-428;
    int y = event->pos().y()-90;

    x = x/10;
    y = y/10;
    x = x*10;
    y = y*10;


    if (x >= 0 && y >= 0){
        if(!toggleEraser){
            for(int width = 0; width < brushValue; width++){
                for(int length = 0; length < brushValue; length++){
                    if(x + width < sizeX && y + length < sizeY){
                        image.setPixel(x + width, y + length, qRgba(redValue, greenValue, blueValue, opacityValue * 255));
                        frameArray[currentFrame].pixels[x + width][y + length] = Pixel(redValue, greenValue, blueValue, opacityValue * 255);
                    }
                }
             }
        }
        else{
            for(int width = 0; width < brushValue; width++){
                for(int length = 0; length < brushValue ; length++){
                    if(x + width < sizeX && y + length < sizeY){
                        image.setPixel(x + width, y + length, qRgba(220, 220, 220, 255));
                        frameArray[currentFrame].pixels[x + width][y + length] = Pixel(220, 220, 220, 255);
                    }
                }
            }
        }

    }

    QMainWindow::mousePressEvent(event);

    update();
}

/**
 * @brief MainWindow::on_EraserButton_clicked
 * Allows the user to erase pixels via a toggable button.
 */
void MainWindow::on_EraserButton_clicked()
{
    if(!toggleEraser)
    {
        ui->EraserButton->setText("Eraser: ON");
        toggleEraser = true;
    }
    else
    {
        ui->EraserButton->setText("Eraser: OFF");
        toggleEraser = false;
    }
}

/**
 * @brief MainWindow::on_addFrameButton_clicked adds a new frame to the end of frameArray.
 * It also adds a new FrameButton to end of frameButtonArray. To get the naming of the
 * new Frame and FrameButton, this method checks the size of the arrays and adds the
 * size + 1 to the end of the name string.
 */
void MainWindow::on_addFrameButton_clicked()
{
    currentFrame = frameArray.length();
    Frame newFrame = Frame(sizeX, sizeY);
    frameArray.push_back(newFrame);
    changeFrame(currentFrame);

    ui->frameSpinBox->setMaximum(frameArray.length());
    ui->frameSpinBox->setValue(currentFrame + 1);
}

/**
 * @brief MainWindow::on_deleteFrameButton_clicked
 * Updates frame array datastructure to keep track of editable frames for the user
 * along with updating any UI components that make use of the frame array.
 */
void MainWindow::on_deleteFrameButton_clicked()
{
    if (currentFrame == 0){
        QMessageBox::information(
                    this,
                    tr("Error"),
                    tr("You cannot delete the first frame."));
    }
    else {
        frameArray.erase(frameArray.begin() + currentFrame);
        currentFrame = frameArray.length() - 1;
        changeFrame(currentFrame);
        update();
        ui->frameSpinBox->setMaximum(frameArray.length());
        ui->frameSpinBox->setValue(currentFrame);
    }
}

/**
 * @brief MainWindow::on_frameSpinBox_valueChanged
 * loads the current frame to the canvas to either viewed or edited.
 * @param selectFrame
 */
void MainWindow::on_frameSpinBox_valueChanged(int selectFrame)
{
    changeFrame(selectFrame - 1);
}

/**
 * @brief MainWindow::showAboutMessage shows the about message when the toolbar menu
 * About button is pressed.
 */
void MainWindow::showAboutMessage(){
    QMessageBox::information(
                this,
                tr("About"),
                tr("This is our Sprite Editor. It allows you to create sprites by"
                   "clicking pixels onto frames. Coded by Tenace Crane, Ashton Foulger,"
                   " Jacob Pelegrin, and Mason Januszewsk"));
}

/**
 * @brief MainWindow::on_lightMode_triggered changes ui design to light color scheme
 * and saves current ui setting choice to a config file.
 */
void MainWindow::on_lightMode_triggered()
{
    QFile file("config.txt");

    if(file.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text)){
        file.write(QByteArray("darkmode:false"));
        file.flush();
        file.close();
    }
    lightMode();
}

/**
 * @brief MainWindow::on_darkMode_triggered changes ui design to dark color scheme
 * and saves current ui setting choice to a config file.
 */
void MainWindow::on_darkMode_triggered()
{
    QFile file("config.txt");

    if(file.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text)){
        file.write(QByteArray("darkmode:true"));
        file.flush();
        file.close();
    }
    darkMode();
}

/**
 * @brief MainWindow::lightMode changes all ui elements to light mode.
 */
void MainWindow::lightMode(){
    this->setStyleSheet("");
    ui->mainScrollArea->setStyleSheet("");
    ui->mainScrollArea->setStyleSheet("");
    ui->playPauseButton->setStyleSheet("");
    ui->addFrameButton->setStyleSheet("");
    ui->deleteFrameButton->setStyleSheet("");
    ui->fpsSpinBox->setStyleSheet("");
    ui->fpsLabel->setStyleSheet("");
    ui->EraserButton->setStyleSheet("");
    ui->brushSizeLabel->setStyleSheet("");
    ui->brushSizeSpinBox->setStyleSheet("");
    ui->opacityLabel->setStyleSheet("");
    ui->opacitySpinBox->setStyleSheet("");
    ui->blueSpinBox->setStyleSheet("");
    ui->greenSpinBox->setStyleSheet("");
    ui->redSpinBox->setStyleSheet("");
    ui->frameScrollArea->setStyleSheet("");
    ui->menubar->setStyleSheet("");
    ui->frameSpinBox->setStyleSheet("");
    ui->frameLabel->setStyleSheet("");
    ui->PlaybackButton->setStyleSheet("");
    ui->menubar->setStyleSheet("QMenuBar::item:selected { background: rbg(100,100,100); } QMenuBar::item:pressed {  background: grey; }");
}

/**
 * @brief MainWindow::darkMode changes all ui elements to dark mode.
 */
void MainWindow::darkMode(){
    this->setStyleSheet("background-color: rgb(65, 65, 65);");
    ui->mainScrollArea->setStyleSheet("background-color: rgb(65, 65, 65);");

    ui->playPauseButton->setStyleSheet("background-color: rgb(100, 100, 100); color: white");
    ui->addFrameButton->setStyleSheet("background-color: rgb(100, 100, 100); color: white");
    ui->deleteFrameButton->setStyleSheet("background-color: rgb(100, 100, 100); color: white");
    ui->fpsSpinBox->setStyleSheet("background-color: rgb(100, 100, 100); color: white");
    ui->fpsLabel->setStyleSheet("color: white;");
    ui->EraserButton->setStyleSheet("background-color: rgb(100, 100, 100); color: white");
    ui->brushSizeLabel->setStyleSheet("color: white;");
    ui->brushSizeSpinBox->setStyleSheet("background-color: rgb(100, 100, 100); color: white");
    ui->opacityLabel->setStyleSheet("color: white;");
    ui->opacitySpinBox->setStyleSheet("background-color: rgb(100, 100, 100); color: white");
    ui->blueSpinBox->setStyleSheet("background-color: rgb(100, 100, 100); color: white");
    ui->greenSpinBox->setStyleSheet("background-color: rgb(100, 100, 100); color: white");
    ui->redSpinBox->setStyleSheet("background-color: rgb(100, 100, 100); color: white");
    ui->frameScrollArea->setStyleSheet("background-color: rgb(100, 100, 100); color: white");
    ui->menubar->setStyleSheet("background-color: rgb(100, 100, 100); color: white");
    ui->frameSpinBox->setStyleSheet("background-color: rgb(100, 100, 100); color: white");
    ui->frameLabel->setStyleSheet("color: white;");
    ui->PlaybackButton->setStyleSheet("background-color: rgb(100, 100, 100); color: white");
    ui->menubar->setStyleSheet("QMenuBar::item:selected { background: rgb(100,100,100); } QMenuBar::item:pressed {  background: rgb(100,100,100); } QMenuBar::item { background: rgb(100,100,100); color: white }");
}

/**
 * @brief MainWindow::saveProject saves the current project into a file determined by the user.
 * Uses a QFileDialog to ask the user for file name and save location. Uses QTextStream to take
 * information about the current frames and pixels and writes them into the file.
 */
void MainWindow::saveProject(){
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Save Sprite Project"), "",
                                                    tr("Sprite File (*.ssp);;All Files (*)"));

    if (fileName.isEmpty())
        return;
    else {
        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly)) {
            QMessageBox::information(this, tr("Unable to open file"),
                                     file.errorString());
            return;
        }

        QTextStream out(&file);

        out << "{\n";
        out << "\t"<<"height:" << sizeY <<",\n";
        out << "\t"<<"width:" << sizeX <<",\n";
        out << "\t"<<"numberOfFrames:" << frameArray.size() <<",\n";
        out << "\t"<<"frames: {\n";

        for(int k = 0; k < frameArray.size(); k++){
            out << "Frame" << k;
            for (int i = 0; i < sizeY; i++)
            {
                for (int j = 0; j < sizeX; j++)
                {
                    out << "\t";
                    out << "[" <<std::get<0>(frameArray[k].pixels[i][j].color) << ", "
                        << std::get<1>(frameArray[k].pixels[i][j].color) << ", "
                        << std::get<2>(frameArray[k].pixels[i][j].color) << ", "
                        << std::get<3>(frameArray[k].pixels[i][j].color) << "]";
                }
                out << "\n";
            }
        }
        out << "\t}\n";
        out << "}";
    }
}

/**
* @brief MainWindow::loadProject loads a project from a .ssp file. It loads in the
* frames and pixels stored into the project.
*/
void MainWindow::loadProject(){
   QString fileName = QFileDialog::getOpenFileName(this,
                                                   tr("Open Sprite Project"), "",
                                                   tr("Sprite File (*.ssp);;All Files (*)"));

   if (fileName.isEmpty())
       return;
   else {

       QFile file(fileName);

       if (!file.open(QIODevice::ReadOnly)) {
           QMessageBox::information(this, tr("Unable to open file"),
                                    file.errorString());
           return;
       }

       QTextStream in(&file);
       frameArray.clear();   // clear existing frames

       // read in the height
   }
}

void MainWindow::newProject(){
    qApp->quit();
    QProcess::startDetached(qApp->arguments()[0], qApp->arguments());
}

/**
 * @brief MainWindow::on_PlaybackButton_clicked
 *
 */
void MainWindow::on_PlaybackButton_clicked()
{
    if(frameArray.length() == 1){
            ui->PlaybackButton->setText("Playback: OFF");
            togglePlayback = false;
            return;
        }
        if(!togglePlayback)
        {
            ui->PlaybackButton->setText("Playback: ON");
            togglePlayback = true;
            changeFrame(1);
            update();
            ui->frameSpinBox->setValue(currentFrame);
            ui->addFrameButton->setEnabled(false);
            ui->deleteFrameButton->setEnabled(false);
            ui->fpsSpinBox->setEnabled(false);
            spritePlayback();
        }
        else
        {
            ui->PlaybackButton->setText("Playback: OFF");
            togglePlayback = false;
            ui->addFrameButton->setEnabled(true);
            ui->deleteFrameButton->setEnabled(true);
            ui->fpsSpinBox->setEnabled(true);
        }
}

/**
 * @brief MainWindow::spritePlayback
 *
 */
void MainWindow::spritePlayback()
{
    if(!togglePlayback){
        return;
    }

    if(currentFrame < frameArray.length() -1){
        changeFrame(currentFrame + 1);
        update();
        QTimer::singleShot(1000/fps, this, &MainWindow::spritePlayback);
    }
    else{
        changeFrame(0);
        update();
        QTimer::singleShot(1000/fps, this, &MainWindow::spritePlayback);
    }
}
