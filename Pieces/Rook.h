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

    QVector<QPoint> GetLegalMoves(const QVector<QPoint>& friendlyPieces, const QVector<QPoint>& enemyPieces) const override
    {
        QVector<QPoint> moves;

        moves.append(CheckMoves(friendlyPieces, enemyPieces, false, CheckDirection::East));
        moves.append(CheckMoves(friendlyPieces, enemyPieces, false, CheckDirection::West));
        moves.append(CheckMoves(friendlyPieces, enemyPieces, false, CheckDirection::North));
        moves.append(CheckMoves(friendlyPieces, enemyPieces, false, CheckDirection::South));
        return moves;
    }

    QVector<QPoint> GetThreateningMoves(const QVector<QPoint>& friendlyPieces, const QVector<QPoint>& enemyPieces) const override
    {
        QVector<QPoint> moves;
        moves.append(CheckMoves(friendlyPieces, enemyPieces, false, CheckDirection::East, true));
        moves.append(CheckMoves(friendlyPieces, enemyPieces, false, CheckDirection::West, true));
        moves.append(CheckMoves(friendlyPieces, enemyPieces, false, CheckDirection::North, true));
        moves.append(CheckMoves(friendlyPieces, enemyPieces, false, CheckDirection::South, true));
        return moves;
    }

private:


};

#endif // ROOK_H
