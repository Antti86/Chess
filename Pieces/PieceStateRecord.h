#ifndef PIECESTATERECORD_H
#define PIECESTATERECORD_H

#include "basepiece.h"

struct PieceStateRecord
{
    PieceType type;
    QPoint position;
    bool hasMoved;

    bool operator==(const PieceStateRecord& other) const
    {
        return type == other.type && position == other.position && hasMoved == other.hasMoved;
    }
};

#endif // PIECESTATERECORD_H
