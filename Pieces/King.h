#ifndef KING_H
#define KING_H


#include "basepiece.h"
#include "../ConstantValues.h"

class King : public BasePiece
{
public:
    King(QBrush color, QPoint pos, PieceType type, QGraphicsItem *parent = nullptr)
        : BasePiece(color, pos, type, parent)
    {
        if (color == Qt::white)
        {
            pixmap = QPixmap(":/Images/w_king.png");
        }
        else
        {
            pixmap = QPixmap(":/Images/b_king.png");
        }

        scaledPixmap = pixmap.scaled(Constants::SQUARE_SIZE - 10, Constants::SQUARE_SIZE - 10, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    }

    void Move(QPoint newPos) override
    {
        //TODO
        pos = newPos;
        setPos(pos);
    }

    QVector<QPoint> GetLegalMoves() const override
    {
        //TODO

        QVector<QPoint> moves;

        return moves;
    }

private:

};

#endif // KING_H