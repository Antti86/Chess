#ifndef GAME_H
#define GAME_H

#include "chessboard.h"
#include "qobject.h"

enum class PlayerTurn { White, Black };

class Game : public QObject
{
    Q_OBJECT

public:
    Game(ChessBoard *board, QObject *parent);

signals:
    void pieceMoved(QPoint from, QPoint to);
    void SetSquareColor(const QPoint& pos, bool highlighting);
    void gameOver(bool isWhiteWinner);
    void UpdateUiToTurn(bool isWhiteTurn);

public slots:
    void handlePieceSelection(QPoint pos);
    void EndOfTurn(); //???

private:
    bool IsPieceOnSelectedSquare(QPoint square) const;

private:
    bool isWhiteTurn;
    bool isPieceSelected;
    QPoint selectedPiecePos;
    ChessBoard *board;
};

#endif // GAME_H
