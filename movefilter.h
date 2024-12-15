#ifndef MOVEFILTER_H
#define MOVEFILTER_H

#pragma once
#include "chessboard.h"
#include "GameInfo.h"

class MoveFilter
{
public:
    MoveFilter(ChessBoard* board, GameInfo& gameInfo);
    QVector<QPoint> GetFilteredMoves(const QPoint& selectedPiecePos); // More accurate, but slow for negamax
    QVector<QPoint> GetFilteredMoves(BasePiece* piece, const QPoint& selectedPiecePos); // Faster overload, but it gives false moves in some edge cases
    QVector<QPoint> FilterKingMovements(const QVector<QPoint>& moves, const QPoint &kingPos, const QVector<QPoint> &dangerAreas) const;
    QVector<QPoint> FilterAvailableMovements(const QVector<QPoint>& moves, const QPoint &selectedPiecePos) const;

    QVector<QPoint> GetGhostArea(const QVector<QPoint> &field, const QPoint &pos) const; //returns field without pos square

    BasePiece* GetSelectedPiece(const QPoint &pos) const; // Returns raw pointer to piece in pos square, null if no piece

    std::unique_ptr<BasePiece> GivePiecePointer(const QPoint& selectedPiecePos); // Moves pointer ownership, bot simulations uses this
    QPoint GetKingPos(QBrush color) const;

    bool IsKingChecked(const QVector<QPoint> &dangerAreas) const;
    bool IsEatingMovement(const QPoint pos) const;
    QVector<QPoint> GetCastlingMoves() const;

private:
    ChessBoard* board;
    GameInfo& gameInfo;

};



#endif // MOVEFILTER_H
