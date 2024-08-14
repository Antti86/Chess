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

        QVector<QPoint> moves;

        QVector<QPoint> East = CheckMoves(friendlyPieces, enemyPieces, false, CheckDirection::East);
        QVector<QPoint> West = CheckMoves(friendlyPieces, enemyPieces, false, CheckDirection::West);
        QVector<QPoint> North = CheckMoves(friendlyPieces, enemyPieces, false, CheckDirection::North);
        QVector<QPoint> South = CheckMoves(friendlyPieces, enemyPieces, false, CheckDirection::South);
        QVector<QPoint> SouthEast = CheckMoves(friendlyPieces, enemyPieces, false, CheckDirection::SouthEast);
        QVector<QPoint> NorthEast = CheckMoves(friendlyPieces, enemyPieces, false, CheckDirection::NorthEast);
        QVector<QPoint> NorthWest = CheckMoves(friendlyPieces, enemyPieces, false, CheckDirection::NorthWest);
        QVector<QPoint> SouthWest = CheckMoves(friendlyPieces, enemyPieces, false, CheckDirection::SouthWest);

        moves.append(East);
        moves.append(West);
        moves.append(North);
        moves.append(South);
        moves.append(SouthEast);
        moves.append(NorthEast);
        moves.append(NorthWest);
        moves.append(SouthWest);

        return moves;
    }

private:

};


#endif // QUEEN_H
