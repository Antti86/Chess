#include "boardsquare.h"

BoardSquare::BoardSquare(QBrush Color, QPoint pos, QGraphicsItem *parent)
    : QGraphicsRectItem(parent)
    , Color(Color)
    , pos(pos)
{
    this->setRect(pos.x() * length, pos.y() * length, length, length);
    this->setBrush(Color);

}

int BoardSquare::type() const
{
    return Type;
}

const QPoint &BoardSquare::GetPos() const
{
    return pos;
}

const QBrush &BoardSquare::GetColor() const
{
    return Color;
}

void BoardSquare::SetColor(const QBrush& color)
{
    this->setBrush(color);
    this->update();
}

