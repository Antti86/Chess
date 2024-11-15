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

    int GetPieceScore() const override
    {
        return 1000;
    }

    QVector<QPoint> GetLegalMoves(const QVector<QPoint>& friendlyPieces, const QVector<QPoint>& enemyPieces) const override
    {
        QVector<QPoint> moves;
        moves.append(CheckMoves(friendlyPieces, enemyPieces, false, CheckDirection::East));
        moves.append(CheckMoves(friendlyPieces, enemyPieces, false, CheckDirection::West));
        moves.append(CheckMoves(friendlyPieces, enemyPieces, false, CheckDirection::North));
        moves.append(CheckMoves(friendlyPieces, enemyPieces, false, CheckDirection::South));
        moves.append(CheckMoves(friendlyPieces, enemyPieces, false, CheckDirection::SouthEast));
        moves.append(CheckMoves(friendlyPieces, enemyPieces, false, CheckDirection::NorthEast));
        moves.append(CheckMoves(friendlyPieces, enemyPieces, false, CheckDirection::NorthWest));
        moves.append(CheckMoves(friendlyPieces, enemyPieces, false, CheckDirection::SouthWest));
        return moves;
    }

    QVector<QPoint> GetThreateningMoves(const QVector<QPoint>& friendlyPieces, const QVector<QPoint>& enemyPieces) const override
    {
        QVector<QPoint> moves;
        moves.append(CheckMoves(friendlyPieces, enemyPieces, false, CheckDirection::East, true));
        moves.append(CheckMoves(friendlyPieces, enemyPieces, false, CheckDirection::West, true));
        moves.append(CheckMoves(friendlyPieces, enemyPieces, false, CheckDirection::North, true));
        moves.append(CheckMoves(friendlyPieces, enemyPieces, false, CheckDirection::South, true));
        moves.append(CheckMoves(friendlyPieces, enemyPieces, false, CheckDirection::SouthEast, true));
        moves.append(CheckMoves(friendlyPieces, enemyPieces, false, CheckDirection::NorthEast, true));
        moves.append(CheckMoves(friendlyPieces, enemyPieces, false, CheckDirection::NorthWest, true));
        moves.append(CheckMoves(friendlyPieces, enemyPieces, false, CheckDirection::SouthWest, true));
        return moves;
    }

private:

};


#endif // QUEEN_H
