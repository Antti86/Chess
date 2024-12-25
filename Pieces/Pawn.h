#ifndef PAWN_H
#define PAWN_H

#include "basepiece.h"
#include "../ConstantValues.h"

class Pawn : public BasePiece
{
public:
    Pawn(QBrush color, QPoint logicalPosition, PieceType type, QGraphicsItem *parent = nullptr)
        : BasePiece(color, logicalPosition, type, parent)
    {
        if (color == Qt::white)
        {
            pixmap = QPixmap(":/Images/w_pawn.png");
        }
        else
        {
            pixmap = QPixmap(":/Images/b_pawn.png");
        }

        scaledPixmap = pixmap.scaled(Constants::SQUARE_SIZE - 10, Constants::SQUARE_SIZE - 10, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        setPixmap(scaledPixmap);

        canPassantLeft = false;
        canPassantRight = false;
    }

    int GetPieceScore() const override
    {
        return 100;
    }

    void Move(QPoint newLogicalPosition) override
    {
        BasePiece::Move(newLogicalPosition);
        // logicalPosition = newPos;
        // hasMoved = true;

        canPassantLeft = false;
        canPassantRight = false;
    }





    QVector<QPoint> GetLegalMoves(const QVector<QPoint>& friendlyPieces, const QVector<QPoint>& enemyPieces) const override
    {
        QVector<QPoint> moves;

        if (this->getColor() == Qt::white)
        {
            QPoint basicMove = QPoint(logicalPosition.x(), logicalPosition.y() - 1);
            bool canBasicMove = false;
            if (!friendlyPieces.contains(basicMove) && !enemyPieces.contains(basicMove))
            {
                moves.append(basicMove);
                canBasicMove = true;
            }

            QPoint eatingMoveR = QPoint(logicalPosition.x() + 1, logicalPosition.y() - 1);
            QPoint eatingMoveL = QPoint(logicalPosition.x() - 1, logicalPosition.y() - 1);

            if (canPassantLeft)
            {
                QPoint passantMoveL = QPoint(logicalPosition.x() - 1, logicalPosition.y());
                if (enemyPieces.contains(passantMoveL))
                {
                    moves.append(eatingMoveL);
                }
            }

            if (canPassantRight)
            {
                QPoint passantMoveR = QPoint(logicalPosition.x() + 1, logicalPosition.y());
                if (enemyPieces.contains(passantMoveR))
                {
                    moves.append(eatingMoveR);
                }
            }

            if (enemyPieces.contains(eatingMoveL))
            {
                moves.append(eatingMoveL);
            }
            if (enemyPieces.contains(eatingMoveR))
            {
                moves.append(eatingMoveR);
            }

            if (!hasMoved && canBasicMove)
            {
                QPoint twoStepMove = QPoint(logicalPosition.x(), logicalPosition.y() - 2);
                if (!friendlyPieces.contains(twoStepMove) && !enemyPieces.contains(twoStepMove))
                {
                    moves.append(twoStepMove);
                }
            }
        }
        else
        {
            QPoint basicMove = QPoint(logicalPosition.x(), logicalPosition.y() + 1);
            bool canBasicMove = false;
            if (!friendlyPieces.contains(basicMove) && !enemyPieces.contains(basicMove))
            {
                moves.append(basicMove);
                canBasicMove = true;
            }
            QPoint eatingMoveR = QPoint(logicalPosition.x() + 1, logicalPosition.y() + 1);
            QPoint eatingMoveL = QPoint(logicalPosition.x() - 1, logicalPosition.y() + 1);

            if (canPassantLeft)
            {
                QPoint passantMoveL = QPoint(logicalPosition.x() - 1, logicalPosition.y());
                if (enemyPieces.contains(passantMoveL))
                {
                    moves.append(eatingMoveL);
                }
            }

            if (canPassantRight)
            {
                QPoint passantMoveR = QPoint(logicalPosition.x() + 1, logicalPosition.y());
                if (enemyPieces.contains(passantMoveR))
                {
                    moves.append(eatingMoveR);
                }
            }

            if (enemyPieces.contains(eatingMoveL))
            {
                moves.append(eatingMoveL);
            }
            if (enemyPieces.contains(eatingMoveR))
            {
                moves.append(eatingMoveR);
            }

            if (!hasMoved && canBasicMove)
            {
                QPoint twoStepMove = QPoint(logicalPosition.x(), logicalPosition.y() + 2);
                if (!friendlyPieces.contains(twoStepMove) && !enemyPieces.contains(twoStepMove))
                {
                    moves.append(twoStepMove);
                }
            }
        }

        return moves;
    }

    QVector<QPoint> GetThreateningMoves(const QVector<QPoint>& friendlyPieces, const QVector<QPoint>& enemyPieces) const override
    {
        QVector<QPoint> moves;
        if (this->getColor() == Qt::white)
        {
            moves.append(QPoint(logicalPosition.x() - 1, logicalPosition.y() - 1));
            moves.append(QPoint(logicalPosition.x() + 1, logicalPosition.y() - 1));
        }
        else
        {
            moves.append(QPoint(logicalPosition.x() + 1, logicalPosition.y() + 1));
            moves.append(QPoint(logicalPosition.x() - 1, logicalPosition.y() + 1));
        }
        return moves;
    }

private:


};

#endif // PAWN_H
