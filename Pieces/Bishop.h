#ifndef BISHOP_H
#define BISHOP_H


#include "basepiece.h"
#include "../ConstantValues.h"

class Bishop : public BasePiece
{
public:
    Bishop(QBrush color, QPoint pos, PieceType type, QGraphicsItem *parent = nullptr)
        : BasePiece(color, pos, type, parent)
    {
        if (color == Qt::white)
        {
            pixmap = QPixmap(":/Images/w_bishop.png");
        }
        else
        {
            pixmap = QPixmap(":/Images/b_bishop.png");
        }

        scaledPixmap = pixmap.scaled(Constants::SQUARE_SIZE - 10, Constants::SQUARE_SIZE - 10, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        setPixmap(scaledPixmap);
    }

    QVector<QPoint> GetLegalMoves(const QVector<QPoint>& friendlyPieces, const QVector<QPoint>& enemyPieces) const override
    {
        QVector<QPoint> moves;
        moves.append(CheckMoves(friendlyPieces, enemyPieces, false, CheckDirection::SouthEast));
        moves.append(CheckMoves(friendlyPieces, enemyPieces, false, CheckDirection::NorthEast));
        moves.append(CheckMoves(friendlyPieces, enemyPieces, false, CheckDirection::NorthWest));
        moves.append(CheckMoves(friendlyPieces, enemyPieces, false, CheckDirection::SouthWest));
        return moves;
    }

    QVector<QPoint> GetThreateningMoves(const QVector<QPoint>& friendlyPieces, const QVector<QPoint>& enemyPieces) const override
    {
        QVector<QPoint> moves;
        moves.append(CheckMoves(friendlyPieces, enemyPieces, false, CheckDirection::SouthEast, true));
        moves.append(CheckMoves(friendlyPieces, enemyPieces, false, CheckDirection::NorthEast, true));
        moves.append(CheckMoves(friendlyPieces, enemyPieces, false, CheckDirection::NorthWest, true));
        moves.append(CheckMoves(friendlyPieces, enemyPieces, false, CheckDirection::SouthWest, true));
        return moves;
    }

private:

};

#endif // BISHOP_H
