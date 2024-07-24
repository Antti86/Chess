#ifndef BASEPIECE_H
#define BASEPIECE_H

#include "qbrush.h"
#include "qgraphicsitem.h"

enum class PieceType
{
    King,
    Queen,
    Knight,
    Pawn,
    Bishop,
    Rook

};

class BasePiece : public QGraphicsPixmapItem
{
public:
    BasePiece(QBrush color, QPoint pos, PieceType type, QGraphicsItem *parent = nullptr)
        : QGraphicsPixmapItem(parent), color(color), pos(pos), type(type) {}
    virtual ~BasePiece() {}
    virtual void Draw() = 0;
    virtual void Move(QPoint newPos) = 0;
    virtual QVector<QPoint> GetLegalMoves() const = 0;

    QBrush getColor() const { return color; }
    QPoint getPos() const { return pos; }
    PieceType getType() const { return type; }
protected:
    QBrush color;
    QPoint pos;
    PieceType type;
};

#endif // BASEPIECE_H
