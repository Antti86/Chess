#ifndef BASEPIECE_H
#define BASEPIECE_H


#include "qbrush.h"
#include "qgraphicsitem.h"
#include "../ConstantValues.h"
#include <QPropertyAnimation>

enum class PieceType {
    King,
    Queen,
    Knight,
    Pawn,
    Bishop,
    Rook

};

enum class CheckDirection
{
    South,
    SouthWest,
    West,
    NorthWest,
    North,
    NorthEast,
    East,
    SouthEast
};

class BasePiece : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
    Q_PROPERTY(QPointF pos READ pos WRITE setPos)
public:
    BasePiece(QBrush color, QPoint logicalPosition, PieceType type, QGraphicsItem *parent = nullptr)
        : QGraphicsPixmapItem(parent)
        , hasMoved(false)
        , color(color)
        , logicalPosition(logicalPosition)
        , type(type)
    {
        bool canPassantLeft = false;
        bool canPassantRight = false;

        setPos(logicalPosition.x() * Constants::SQUARE_SIZE + Constants::PIECE_OFFSET,
               logicalPosition.y() * Constants::SQUARE_SIZE + Constants::PIECE_OFFSET);
    }

    virtual int GetPieceScore() const = 0;

    virtual ~BasePiece() = default;
    virtual void Draw()
    {
        setPos(logicalPosition.x() * Constants::SQUARE_SIZE + Constants::PIECE_OFFSET, logicalPosition.y() * Constants::SQUARE_SIZE + Constants::PIECE_OFFSET);
    };
    virtual void Move(QPoint newLogicalPosition)
    {
        QPropertyAnimation *anim = new QPropertyAnimation(this, "pos");
        anim->setDuration(500);
        anim->setStartValue(pos());
        anim->setEndValue(QPointF(newLogicalPosition.x() * Constants::SQUARE_SIZE + Constants::PIECE_OFFSET,
                                  newLogicalPosition.y() * Constants::SQUARE_SIZE + Constants::PIECE_OFFSET));
        anim->start(QAbstractAnimation::DeleteWhenStopped);

        hasMoved = true;
        logicalPosition = newLogicalPosition;
    }
    virtual QVector<QPoint> GetLegalMoves(const QVector<QPoint>& friendlyPieces, const QVector<QPoint>& enemyPieces) const = 0;
    virtual QVector<QPoint> GetThreateningMoves(const QVector<QPoint>& friendlyPieces, const QVector<QPoint>& enemyPieces) const = 0;
    QBrush getColor() const { return color; }
    QPoint getPos() const { return logicalPosition; }
    PieceType getType() const { return type; }
    bool GetHashMoved() const {return hasMoved;}

    bool canPassantLeft;
    bool canPassantRight;
    bool hasMoved;

protected:
    QBrush color;
    QPoint logicalPosition;
    PieceType type;
    QPixmap pixmap;
    QPixmap scaledPixmap;

protected:
    QVector<QPoint> CheckMoves(const QVector<QPoint>& friendlyPieces, const QVector<QPoint>& enemyPieces, bool onlyOneSquare,
                               CheckDirection dir, bool checkingThreat = false) const
    {

        //Did this algorithm, cos wanted to see if i could make generic movement checker. Works nice for the most pieces.
        //But if this was a paid or team project, i would have probably maked simplier version and just copy pastet to
        // inhereting pieces.

        QVector<QPoint> moves;

        int xOffset;
        int yOffset;
        int dirCap;
        int dirCheckVal;
        int dirIncrement;
        std::function<bool(int, int)> comp;

        switch (dir)
        {
        case CheckDirection::East:
            xOffset = 1;
            dirCheckVal = logicalPosition.x() + 1;
            dirIncrement = 1;
            yOffset = 0;
            dirCap = 7;
            comp = std::less_equal<int>();
            break;
        case CheckDirection::North:
            xOffset = 0;
            dirCheckVal = logicalPosition.y() - 1;
            dirIncrement = -1;
            yOffset = -1;
            dirCap = 0;
            comp = std::greater_equal<int>();
            break;
        case CheckDirection::NorthEast:
        {
            bool isXmain = (7 - logicalPosition.x()) <= logicalPosition.y() ? true : false;
            xOffset = 1;
            dirCheckVal = isXmain ? (logicalPosition.x() + 1) : (logicalPosition.y() - 1);
            dirIncrement = isXmain ? 1 : -1;
            yOffset = -1;
            dirCap = isXmain ? 7 : 0;
            if (isXmain)
            {
                comp = std::less_equal<int>();
            }
            else
            {
                comp = std::greater_equal<int>();
            }
        }
        break;
        case CheckDirection::South:
            xOffset = 0;
            dirCheckVal = logicalPosition.y() + 1;
            dirIncrement = 1;
            yOffset = 1;
            dirCap = 7;
            comp = std::less_equal<int>();
            break;
        case CheckDirection::SouthWest:
        {
            bool isXmain = logicalPosition.x() <= (7 - logicalPosition.y()) ? true : false;
            xOffset = -1;
            dirCheckVal = isXmain ? (logicalPosition.x() - 1) : (logicalPosition.y() + 1);
            dirIncrement = isXmain ? -1 : 1;
            yOffset = 1;
            dirCap = isXmain ? 0 : 7;
            if (isXmain)
            {

                comp = std::greater_equal<int>();
            }
            else
            {
                comp = std::less_equal<int>();
            }
        }
            break;
        case CheckDirection::West:
            xOffset = -1;
            dirCheckVal = logicalPosition.x() - 1;
            dirIncrement = -1;
            yOffset = 0;
            dirCap = 0;
            comp = std::greater_equal<int>();
            break;
        case CheckDirection::NorthWest:
            xOffset = -1;
            dirCheckVal = std::min(logicalPosition.x(), logicalPosition.y()) - 1;
            dirIncrement = -1;
            yOffset = -1;
            dirCap = 0;
            comp = std::greater_equal<int>();
            break;
        case CheckDirection::SouthEast:
            xOffset = 1;
            dirCheckVal = std::max(logicalPosition.x(), logicalPosition.y()) + 1;
            dirIncrement = 1;
            yOffset = 1;
            dirCap = 7;
            comp = std::less_equal<int>();
            break;
        }

        if (comp(dirCheckVal, dirCap))
        {
            if (onlyOneSquare)
            {
                QPoint checkArea = QPoint(logicalPosition.x() + xOffset, logicalPosition.y() + yOffset);
                if (checkArea.x() >= 0 && checkArea.x() < 8 && checkArea.y() >= 0 && checkArea.y() < 8)
                {
                    if (!checkingThreat)
                    {
                        if (!friendlyPieces.contains(checkArea))
                        {
                            moves.append(checkArea);
                        }
                    }
                    else
                    {
                        moves.append(checkArea);
                    }
                }
            }
            else
            {

                for (int x = logicalPosition.x() + xOffset, y = logicalPosition.y() + yOffset; comp(dirCheckVal, dirCap); dirCheckVal += dirIncrement, x += xOffset, y += yOffset )
                {
                    QPoint checkArea = QPoint(x, y);
                    if (!checkingThreat)
                    {
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
                    else
                    {
                        if (friendlyPieces.contains(checkArea) || enemyPieces.contains(checkArea))
                        {
                            moves.append(checkArea);
                            break;
                        }
                        moves.append(checkArea);
                    }
                }
            }

        }
        return moves;
    }



};

#endif // BASEPIECE_H
