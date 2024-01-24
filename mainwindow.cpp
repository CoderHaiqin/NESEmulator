#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>
#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    timer = new QTimer(this);
    timer->setTimerType(Qt::PreciseTimer);
    timer->start(20);
    QObject::connect(timer, &QTimer::timeout, this, &MainWindow::update);

    machine.load("./nestest.nes");

}

void MainWindow::update() {
    this->machine.update();
    this->repaint();
}

void MainWindow::paintEvent(QPaintEvent *event) {
    QPainter p(this);
    this->machine.get(this->pixels);
    QImage img((uchar*)pixels, Constant::screenWidth, Constant::screenHeight, QImage::Format_ARGB32);
    img = img.scaled(Constant::screenWidth * 2, Constant::screenHeight * 2);
    p.drawImage(0, 0, img);

    std::cout << "repaint" << std::endl;
}


MainWindow::~MainWindow()
{
    delete ui;
}

