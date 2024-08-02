#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include "chessboard.h"
#include "game.h"
#include <QMouseEvent>

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

public slots:
    void ChangeTurnMark(bool isWhiteTrun);
    void setMousePoint (QPointF point);

private:
    Ui::MainWindow *ui;
    bool firstTime;
    ChessBoard *board;
    Game *game;
    bool isPlaying;

    void kill();
    void ChangePage(QWidget* widget);

    void StartGame();
    void GameOver();

};
#endif // MAINWINDOW_H
