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
        //Uses own movements for now, change this later...

        //Left
        if (pos.x() > 0)
        {
            for (int x = pos.x() - 1; x >= 0; x--)
            {
                QPoint checkArea = QPoint(x, pos.y());
                if (friendlyPieces.contains(checkArea))
                {
                    break;
                }
                if (enemyPieces.contains(checkArea))
                {
                    moves.append(checkArea);
                    break;
                }
                moves.append(checkArea);
            }
        }


        //Right
        if (pos.x() < 7)
        {
            for (int x = pos.x() + 1; x < 8; x++)
            {
                QPoint checkArea = QPoint(x, pos.y());
                if (friendlyPieces.contains(checkArea))
                {
                    break;
                }
                if (enemyPieces.contains(checkArea))
                {
                    moves.append(checkArea);
                    break;
                }
                moves.append(checkArea);
            }
        }


        //Up
        if (pos.y() > 0)
        {
            for (int y = pos.y() - 1; y >= 0; y--)
            {
                QPoint checkArea = QPoint(pos.x(), y);
                if (friendlyPieces.contains(checkArea))
                {
                    break;
                }
                if (enemyPieces.contains(checkArea))
                {
                    moves.append(checkArea);
                    break;
                }
                moves.append(checkArea);
            }
        }


        //Down
        if (pos.y() < 7)
        {
            for (int y = pos.y() + 1; y < 8; y++)
            {
                QPoint checkArea = QPoint(pos.x(), y);
                if (friendlyPieces.contains(checkArea))
                {
                    break;
                }
                if (enemyPieces.contains(checkArea))
                {
                    moves.append(checkArea);
                    break;
                }
                moves.append(checkArea);
            }
        }


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
