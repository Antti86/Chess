#ifndef BOARDSQUARE_H
#define BOARDSQUARE_H

#include "qgraphicsitem.h"
#include <qbrush.h>

class BoardSquare : public QGraphicsRectItem
{

public:
    BoardSquare(QBrush Color, QPoint pos, QGraphicsItem *parent = nullptr);
    ~BoardSquare() = default;

    enum { Type = UserType + 1 };
    int type() const override;

public:
    const QPoint& GetPos() const;
    const QBrush& GetColor() const;
    void SetColor(const QBrush& color);

private:
    static constexpr int length = 70;
    QBrush Color;
    QPoint pos;

};

#endif // BOARDSQUARE_H
