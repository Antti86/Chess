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
    void EatPiece(QPoint eatingPos);
    void SetSquareColor(const QPoint& pos, const QVector<QPoint>& legalMoves, bool highlighting);
    void gameOver(bool isWhiteWinner);
    void UpdateUiToTurn(bool isWhiteTurn);
    void UpdateUiForCheck(bool isChecked);

public slots:
    void handlePieceSelection(QPoint pos);
    void EndOfTurn();

private:
    bool IsPieceOnSelectedSquare(QPoint square) const;
    bool ValidMovement(BasePiece* piece, QVector<QPoint> legalMoves, QPoint& pos) const;
    bool IsEatingMovement(const QPoint pos) const;
    BasePiece* GetSelectedPiece(const QPoint pos) const;
    bool IsKingChecked(const QVector<QPoint> &dangerAreas);

    QVector<QPoint> FilterKingMovements(QVector<QPoint>& moves);
    QVector<QPoint> FilterAvailableMovements(const QVector<QPoint>& moves);

    void SetAreaFields();
    QVector<QPoint> GetGhostArea(const QVector<QPoint> &field, const QPoint &pos);

private:
    bool isWhiteTurn;
    bool isPieceSelected;
    QPoint selectedPiecePos;
    ChessBoard *board;
    QVector<QPoint> friendly;
    QVector<QPoint> enemy;
};

#endif // GAME_H
