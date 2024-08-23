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
    void pieceMoved(QPoint from, QPoint to, bool isWhiteTurn);
    void EatPiece(QPoint eatingPos);
    void SetSquareColor(const QPoint& pos, const QVector<QPoint>& legalMoves, bool highlighting);
    void gameOver(bool isWhiteWinner);
    void UpdateUiToTurn(bool isWhiteTurn);
    void UpdateUiForCheck(bool isChecked);
    void UpdateUiForCheckMate(bool isWhiteWinner);

public slots:
    void handlePieceSelection(QPoint pos);
    void EndOfTurn();

private:
    bool IsPieceOnSelectedSquare(QPoint square) const;
    bool ValidMovement(BasePiece* piece, QVector<QPoint> legalMoves, QPoint& pos) const;
    bool IsEatingMovement(const QPoint pos) const;
    bool IsPassantMovement(BasePiece* piece, const QPoint& pos) const;
    BasePiece* GetSelectedPiece(const QPoint pos) const;
    bool IsKingChecked(const QVector<QPoint> &dangerAreas) const;
    bool IsCheckMate() const;
    BasePiece* IsCastlingMove(BasePiece* piece, const QPoint& pos) const;
    QVector<QPoint> GetCastlingMoves() const;

    QVector<QPoint> FilterKingMovements(QVector<QPoint>& moves) const;
    QVector<QPoint> FilterAvailableMovements(const QVector<QPoint>& moves) const;

    void SetAreaFields();
    QVector<QPoint> GetGhostArea(const QVector<QPoint> &field, const QPoint &pos) const;

private:
    bool isWhiteTurn;
    bool isPieceSelected;
    QPoint selectedPiecePos;
    ChessBoard *board;
    QVector<QPoint> friendly;
    QVector<QPoint> enemy;
};

#endif // GAME_H
