#ifndef QUEEN_H
#define QUEEN_H

#include "basepiece.h"
#include "ConstantValues.h"

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
    }

    void Draw() override
    {
        setPixmap(scaledPixmap);
        setPos(pos.x() * Constants::SQUARE_SIZE + Constants::PIECE_OFFSET, pos.y() * Constants::SQUARE_SIZE + Constants::PIECE_OFFSET);
    }

    void Move(QPoint newPos) override
    {
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
    QPixmap pixmap;
    QPixmap scaledPixmap;


};

#endif // QUEEN_H

