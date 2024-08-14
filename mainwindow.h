#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QGraphicsScene>
#include <QMainWindow>
#include <QMouseEvent>
#include "chessboard.h"
#include "game.h"

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
    void ChangeCheckedStatus(bool isChecked);

private:
    Ui::MainWindow *ui;
    Game *game;
    bool isPlaying;

    void kill();
    void ChangePage(QWidget *widget);

    void StartGame();
    void GameOver();
};
#endif // MAINWINDOW_H
