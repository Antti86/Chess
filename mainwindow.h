#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include "chessboard.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    ChessBoard *board;

    bool isPlaying;

    void kill();
    void ChangePage(QWidget* widget);

    void StartGame();
    void GameOver();
};
#endif // MAINWINDOW_H
