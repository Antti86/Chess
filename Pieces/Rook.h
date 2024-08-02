#ifndef ROOK_H
#define ROOK_H


#include "basepiece.h"
#include "../ConstantValues.h"

class Rook : public BasePiece
{
public:
    Rook(QBrush color, QPoint pos, PieceType type, QGraphicsItem *parent = nullptr)
        : BasePiece(color, pos, type, parent)
    {
        if (color == Qt::white)
        {
            pixmap = QPixmap(":/Images/w_rook.png");
        }
        else
        {
            pixmap = QPixmap(":/Images/b_rook.png");
        }

        scaledPixmap = pixmap.scaled(Constants::SQUARE_SIZE - 10, Constants::SQUARE_SIZE - 10, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        setPixmap(scaledPixmap);
    }

    void Move(QPoint newPos) override
    {
        //TODO
        pos = newPos;
    }

    QVector<QPoint> GetLegalMoves() const override
    {
        //TODO

        QVector<QPoint> moves;

        return moves;
    }

private:


};

#endif // ROOK_H
