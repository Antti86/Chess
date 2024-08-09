#ifndef PAWN_H
#define PAWN_H

#include "basepiece.h"
#include "../ConstantValues.h"

class Pawn : public BasePiece
{
public:
    Pawn(QBrush color, QPoint pos, PieceType type, QGraphicsItem *parent = nullptr)
        : BasePiece(color, pos, type, parent)
    {
        if (color == Qt::white)
        {
            pixmap = QPixmap(":/Images/w_pawn.png");
        }
        else
        {
            pixmap = QPixmap(":/Images/b_pawn.png");
        }

        scaledPixmap = pixmap.scaled(Constants::SQUARE_SIZE - 10, Constants::SQUARE_SIZE - 10, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        setPixmap(scaledPixmap);
    }

    void Move(QPoint newPos) override
    {
        //TODO
        pos = newPos;
    }

    QVector<QPoint> GetLegalMoves(const QVector<QPoint>& friendlyPieces, const QVector<QPoint>& enemyPieces) const override
    {
        //TODO

        QVector<QPoint> moves;

        return moves;
    }

private:


};

#endif // PAWN_H
