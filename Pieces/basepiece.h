#ifndef BASEPIECE_H
#define BASEPIECE_H


#include "qbrush.h"
#include "qgraphicsitem.h"
#include "../ConstantValues.h"

enum class PieceType {
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
        : QGraphicsPixmapItem(parent)
        , color(color)
        , pos(pos)
        , type(type)
    {}
    virtual ~BasePiece() = default;
    virtual void Draw()
    {
        setPos(pos.x() * Constants::SQUARE_SIZE + Constants::PIECE_OFFSET, pos.y() * Constants::SQUARE_SIZE + Constants::PIECE_OFFSET);
    };
    void Move(QPoint newPos)
    {
        pos = newPos;
    }
    virtual QVector<QPoint> GetLegalMoves(const QVector<QPoint>& friendlyPieces, const QVector<QPoint>& enemyPieces) const = 0;

    QBrush getColor() const { return color; }
    QPoint getPos() const { return pos; }
    PieceType getType() const { return type; }

protected:
    QBrush color;
    QPoint pos;
    PieceType type;
    QPixmap pixmap;
    QPixmap scaledPixmap;
};

#endif // BASEPIECE_H
