#ifndef BASEPIECE_H
#define BASEPIECE_H


#include "qbrush.h"
#include "qgraphicsitem.h"
#include "../ConstantValues.h"

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

class BasePiece : public QGraphicsPixmapItem
{
public:
    BasePiece(QBrush color, QPoint pos, PieceType type, QGraphicsItem *parent = nullptr)
        : QGraphicsPixmapItem(parent)
        , color(color)
        , pos(pos)
        , type(type)
        , hasMoved(false)
    {}
    virtual ~BasePiece() = default;
    virtual void Draw()
    {
        setPos(pos.x() * Constants::SQUARE_SIZE + Constants::PIECE_OFFSET, pos.y() * Constants::SQUARE_SIZE + Constants::PIECE_OFFSET);
    };
    virtual void Move(QPoint newPos)
    {
        pos = newPos;
        hasMoved = true;
    }
    virtual QVector<QPoint> GetLegalMoves(const QVector<QPoint>& friendlyPieces, const QVector<QPoint>& enemyPieces) const = 0;
    virtual QVector<QPoint> GetThreateningMoves(const QVector<QPoint>& friendlyPieces, const QVector<QPoint>& enemyPieces) const = 0;
    QBrush getColor() const { return color; }
    QPoint getPos() const { return pos; }
    PieceType getType() const { return type; }
    bool GetHashMoved() const {return hasMoved;}

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
            dirCheckVal = pos.x() + 1;
            dirIncrement = 1;
            yOffset = 0;
            dirCap = 7;
            comp = std::less_equal<int>();
            break;
        case CheckDirection::North:
            xOffset = 0;
            dirCheckVal = pos.y() - 1;
            dirIncrement = -1;
            yOffset = -1;
            dirCap = 0;
            comp = std::greater_equal<int>();
            break;
        case CheckDirection::NorthEast:
        {
            bool isXmain = (7 - pos.x()) <= pos.y() ? true : false;
            xOffset = 1;
            dirCheckVal = isXmain ? (pos.x() + 1) : (pos.y() - 1);
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
            dirCheckVal = pos.y() + 1;
            dirIncrement = 1;
            yOffset = 1;
            dirCap = 7;
            comp = std::less_equal<int>();
            break;
        case CheckDirection::SouthWest:
        {
            bool isXmain = pos.x() <= (7 - pos.y()) ? true : false;
            xOffset = -1;
            dirCheckVal = isXmain ? (pos.x() - 1) : (pos.y() + 1);
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
            dirCheckVal = pos.x() - 1;
            dirIncrement = -1;
            yOffset = 0;
            dirCap = 0;
            comp = std::greater_equal<int>();
            break;
        case CheckDirection::NorthWest:
            xOffset = -1;
            dirCheckVal = std::min(pos.x(), pos.y()) - 1;
            dirIncrement = -1;
            yOffset = -1;
            dirCap = 0;
            comp = std::greater_equal<int>();
            break;
        case CheckDirection::SouthEast:
            xOffset = 1;
            dirCheckVal = std::min(pos.x(), pos.y()) + 1;
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
                QPoint checkArea = QPoint(pos.x() + xOffset, pos.y() + yOffset);
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
            else
            {

                for (int x = pos.x() + xOffset, y = pos.y() + yOffset; comp(dirCheckVal, dirCap); dirCheckVal += dirIncrement, x += xOffset, y += yOffset )
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

protected:
    QBrush color;
    QPoint pos;
    PieceType type;
    QPixmap pixmap;
    QPixmap scaledPixmap;
    bool hasMoved;

};

#endif // BASEPIECE_H
