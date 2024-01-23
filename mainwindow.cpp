#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    timer = new QTimer(this);
    timer->setTimerType(Qt::PreciseTimer);
    timer->start(50);
    QObject::connect(timer, &QTimer::timeout, this, &MainWindow::update);

    machine.load("./nestest.nes");

}

void MainWindow::update() {
    this->machine.update();


}

MainWindow::~MainWindow()
{
    delete ui;
}

