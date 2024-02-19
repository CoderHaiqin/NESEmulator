#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "machine.h"
#include <QPainter>
#include <QGraphicsScene>

#include <QGraphicsPixmapItem>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void paintEvent(QPaintEvent *event) override;
    std::map<int, int> keyTable;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

private:
    Ui::MainWindow *ui;

    Machine machine;
    QTimer* timer;

    QGraphicsScene* scene_game;
    QGraphicsPixmapItem *pixmap_lp;
    QRgb pixels[240 * 256];

private slots:
    void update();
};
#endif // MAINWINDOW_H
