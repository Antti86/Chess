#ifndef KNIGHT_H
#define KNIGHT_H


#include "basepiece.h"
#include "../ConstantValues.h"

class Knight : public BasePiece
{
public:
    Knight(QBrush color, QPoint logicalPosition, PieceType type, QGraphicsItem *parent = nullptr)
        : BasePiece(color, logicalPosition, type, parent)
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

    int GetPieceScore() const override
    {
        return 500;
    }

    QVector<QPoint> GetLegalMoves(const QVector<QPoint>& friendlyPieces, const QVector<QPoint>& enemyPieces) const override
    {
        QVector<QPoint> moves;
        QVector<QPoint> possibleMoves;

        possibleMoves.append(QPoint(logicalPosition.x() + 2, logicalPosition.y() + 1));
        possibleMoves.append(QPoint(logicalPosition.x() + 2, logicalPosition.y() - 1));

        possibleMoves.append(QPoint(logicalPosition.x() - 2, logicalPosition.y() + 1));
        possibleMoves.append(QPoint(logicalPosition.x() - 2, logicalPosition.y() - 1));

        possibleMoves.append(QPoint(logicalPosition.x() + 1, logicalPosition.y() + 2));
        possibleMoves.append(QPoint(logicalPosition.x() + 1, logicalPosition.y() - 2));

        possibleMoves.append(QPoint(logicalPosition.x() - 1, logicalPosition.y() + 2));
        possibleMoves.append(QPoint(logicalPosition.x() - 1, logicalPosition.y() - 2));

        for (auto& m : possibleMoves)
        {
            if (m.x() >= 0 && m.x() < 8 && m.y() >= 0 && m.y() < 8)
            {
                if (!friendlyPieces.contains(m))
                {
                    moves.append(m);
                }
            }
        }
        return moves;
    }

    QVector<QPoint> GetThreateningMoves(const QVector<QPoint>& friendlyPieces, const QVector<QPoint>& enemyPieces) const override
    {
        QVector<QPoint> moves;
        QVector<QPoint> possibleMoves;

        possibleMoves.append(QPoint(logicalPosition.x() + 2, logicalPosition.y() + 1));
        possibleMoves.append(QPoint(logicalPosition.x() + 2, logicalPosition.y() - 1));

        possibleMoves.append(QPoint(logicalPosition.x() - 2, logicalPosition.y() + 1));
        possibleMoves.append(QPoint(logicalPosition.x() - 2, logicalPosition.y() - 1));

        possibleMoves.append(QPoint(logicalPosition.x() + 1, logicalPosition.y() + 2));
        possibleMoves.append(QPoint(logicalPosition.x() + 1, logicalPosition.y() - 2));

        possibleMoves.append(QPoint(logicalPosition.x() - 1, logicalPosition.y() + 2));
        possibleMoves.append(QPoint(logicalPosition.x() - 1, logicalPosition.y() - 2));

        for (auto& m : possibleMoves)
        {
            if (m.x() >= 0 && m.x() < 8 && m.y() >= 0 && m.y() < 8)
            {
                moves.append(m);
            }
        }
        return moves;
    }

private:

};

#endif // KNIGHT_H
