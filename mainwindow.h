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
    void GameOver(EndStatus status);

private:
    Ui::MainWindow *ui;
    Game *game;
    bool isPlaying;
    int botDifficulty;

    void kill();
    void ChangePage(QWidget *widget);
    void SetBotDifficulty();

    void StartGame(bool playingAgainstBot);
    void ExitGame();

    // Dynamic connection wrapper
    QList<QMetaObject::Connection> connectionList;

    //Basic connections
    template <typename Sender, typename Signal, typename Receiver, typename Method>
    void trackConnection(Sender sender, Signal signal, Receiver receiver, Method method)
    {
        QMetaObject::Connection connection = connect(sender, signal, receiver, method);
        connectionList.append(connection);
    }

    // Overload for lambda funcs in connection
    template <typename Sender, typename Signal, typename Func>
    void trackConnection(Sender sender, Signal signal, Func func)
    {
        QMetaObject::Connection connection = connect(sender, signal, func);
        connectionList.append(connection);
    }

    // Kills all the dynamic connections. Navigation connections wont be included.
    void ResetConnections();
};
#endif // MAINWINDOW_H
