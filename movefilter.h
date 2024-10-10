#ifndef MOVEFILTER_H
#define MOVEFILTER_H

#pragma once
#include "chessboard.h"
#include "GameInfo.h"

class MoveFilter
{
public:
    MoveFilter(ChessBoard* board, GameInfo& gameInfo);
    QVector<QPoint> GetFilteredMoves(const QPoint& selectedPiecePos);
    QVector<QPoint> FilterKingMovements(const QVector<QPoint>& moves, const QPoint &kingPos) const;
    QVector<QPoint> FilterAvailableMovements(const QVector<QPoint>& moves, const QPoint &selectedPiecePos) const;

    QVector<QPoint> GetGhostArea(const QVector<QPoint> &field, const QPoint &pos) const;

    BasePiece* GetSelectedPiece(const QPoint &pos) const;
    QPoint GetKingPos(QBrush color) const;

    bool IsKingChecked(const QVector<QPoint> &dangerAreas) const;
    bool IsEatingMovement(const QPoint pos) const;


    QVector<QPoint> GetCastlingMoves() const;

private:
    ChessBoard* board;
    GameInfo& gameInfo;

};



#endif // MOVEFILTER_H
