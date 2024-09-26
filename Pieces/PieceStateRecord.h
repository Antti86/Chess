#ifndef PIECESTATERECORD_H
#define PIECESTATERECORD_H

#include "basepiece.h"

struct PieceStateRecord
{
    PieceType type;
    QPoint position;
    bool hasMoved;
    QBrush color;
    bool canPassantLeft;
    bool canPassantRight;

    bool operator==(const PieceStateRecord& other) const
    {
        return type == other.type && position == other.position && hasMoved == other.hasMoved && color == other.color &&
            canPassantLeft == other.canPassantLeft && canPassantRight == other.canPassantRight;
    }

};

#endif // PIECESTATERECORD_H
