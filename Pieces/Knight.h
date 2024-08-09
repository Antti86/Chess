#ifndef KNIGHT_H
#define KNIGHT_H


#include "basepiece.h"
#include "../ConstantValues.h"

class Knight : public BasePiece
{
public:
    Knight(QBrush color, QPoint pos, PieceType type, QGraphicsItem *parent = nullptr)
        : BasePiece(color, pos, type, parent)
    {
        if (color == Qt::white)
        {
            pixmap = QPixmap(":/Images/w_knight.png");
        }
        else
        {
            pixmap = QPixmap(":/Images/b_knight.png");
        }

        scaledPixmap = pixmap.scaled(Constants::SQUARE_SIZE - 10, Constants::SQUARE_SIZE - 10, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        setPixmap(scaledPixmap);
    }

    QVector<QPoint> GetLegalMoves(const QVector<QPoint>& friendlyPieces, const QVector<QPoint>& enemyPieces) const override
    {
        //TODO

        QVector<QPoint> moves;

        return moves;
    }

private:

};

#endif // KNIGHT_H
