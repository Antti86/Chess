#ifndef GAME_H
#define GAME_H


#include "qobject.h"
#include "chessboard.h"


enum class PlayerTurn
{
    White,
    Black
};

class Game : public QObject
{
    Q_OBJECT

public:
    Game(ChessBoard *board, QObject *parent);




signals:
    void pieceMoved(QPoint from, QPoint to);
    void gameOver(bool isWhiteWinner);

public slots:
    void handlePieceSelection(QPoint pos);
    // void handleMove(QPoint from, QPoint to);
    void EndOfTurn(); //???

private:
    bool IsPieceOnSelectedSquare(QPoint square) const;

private:
    bool isWhiteTurn;
    bool isPieceSelected;
    QPoint selectedPiecePos;
    ChessBoard* board;
};

#endif // GAME_H
