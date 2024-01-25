#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>
#include <iostream>
#include <QKeyEvent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    timer = new QTimer(this);
    timer->setTimerType(Qt::PreciseTimer);
    timer->start(20);
    QObject::connect(timer, &QTimer::timeout, this, &MainWindow::update);

    keyTable[Qt::Key_K] = 0;
    keyTable[Qt::Key_J] = 1;
    keyTable[Qt::Key_G] = 2;
    keyTable[Qt::Key_H] = 3;
    keyTable[Qt::Key_W] = 4;
    keyTable[Qt::Key_S] = 5;
    keyTable[Qt::Key_A] = 6;
    keyTable[Qt::Key_D] = 7;

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
}

void MainWindow::keyPressEvent(QKeyEvent* event) {
    machine.input(keyTable[event->key()], 1);
}

void MainWindow::keyReleaseEvent(QKeyEvent* event) {
    machine.input(keyTable[event->key()], 0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

