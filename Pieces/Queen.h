#ifndef QUEEN_H
#define QUEEN_H


#include "../ConstantValues.h"
#include "basepiece.h"

class Queen : public BasePiece
{
public:
    Queen(QBrush color, QPoint pos, PieceType type, QGraphicsItem *parent = nullptr)
        : BasePiece(color, pos, type, parent)
    {
        if (color == Qt::white)
        {
            pixmap = QPixmap(":/Images/w_queen.png");
        }
        else
        {
            pixmap = QPixmap(":/Images/b_queen.png");
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


#endif // QUEEN_H
