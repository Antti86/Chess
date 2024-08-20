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
        setPixmap(scaledPixmap);
    }

    QVector<QPoint> GetLegalMoves(const QVector<QPoint>& friendlyPieces, const QVector<QPoint>& enemyPieces) const override
    {
        QVector<QPoint> moves;
        moves.append(CheckMoves(friendlyPieces, enemyPieces, true, CheckDirection::South));
        moves.append(CheckMoves(friendlyPieces, enemyPieces, true, CheckDirection::North));
        moves.append(CheckMoves(friendlyPieces, enemyPieces, true, CheckDirection::West));
        moves.append(CheckMoves(friendlyPieces, enemyPieces, true, CheckDirection::East));
        moves.append(CheckMoves(friendlyPieces, enemyPieces, true, CheckDirection::SouthEast));
        moves.append(CheckMoves(friendlyPieces, enemyPieces, true, CheckDirection::SouthWest));
        moves.append(CheckMoves(friendlyPieces, enemyPieces, true, CheckDirection::NorthWest));
        moves.append(CheckMoves(friendlyPieces, enemyPieces, true, CheckDirection::NorthEast));
        return moves;
    }

    QVector<QPoint> GetThreateningMoves(const QVector<QPoint>& friendlyPieces, const QVector<QPoint>& enemyPieces) const override
    {
        QVector<QPoint> moves;
        moves.append(CheckMoves(friendlyPieces, enemyPieces, true, CheckDirection::East, true));
        moves.append(CheckMoves(friendlyPieces, enemyPieces, true, CheckDirection::West, true));
        moves.append(CheckMoves(friendlyPieces, enemyPieces, true, CheckDirection::North, true));
        moves.append(CheckMoves(friendlyPieces, enemyPieces, true, CheckDirection::South, true));
        moves.append(CheckMoves(friendlyPieces, enemyPieces, true, CheckDirection::SouthEast, true));
        moves.append(CheckMoves(friendlyPieces, enemyPieces, true, CheckDirection::NorthEast, true));
        moves.append(CheckMoves(friendlyPieces, enemyPieces, true, CheckDirection::NorthWest, true));
        moves.append(CheckMoves(friendlyPieces, enemyPieces, true, CheckDirection::SouthWest, true));
        return moves;
    }

private:


};

#endif // KING_H
