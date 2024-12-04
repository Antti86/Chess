#include "movefilter.h"



MoveFilter::MoveFilter(ChessBoard *board, GameInfo& gameInfo)
    : board(board)
    , gameInfo(gameInfo)
{
}

QVector<QPoint> MoveFilter::GetFilteredMoves(const QPoint &selectedPiecePos)
{
    BasePiece* piece = GetSelectedPiece(selectedPiecePos);
    QVector<QPoint> moves = piece->GetLegalMoves(gameInfo.friendly, gameInfo.enemy);
    QVector<QPoint> filteredMoves;
    QVector<QPoint> dangerAreas = board->GetDangerAreas(gameInfo.isWhiteTurn, GetGhostArea(gameInfo.friendly, selectedPiecePos), gameInfo.enemy);

    if (!IsKingChecked(dangerAreas))
    {
        if (piece->getType() == PieceType::King)
        {
            filteredMoves = FilterKingMovements(moves, selectedPiecePos, dangerAreas);
        }
        else
        {
            filteredMoves = moves;
        }
    }
    else
    {
        if (piece->getType() == PieceType::King)
        {
            filteredMoves = FilterKingMovements(moves, selectedPiecePos, dangerAreas);
        }
        else
        {
            filteredMoves = FilterAvailableMovements(moves, selectedPiecePos);
        }
    }
    return filteredMoves;

}

QVector<QPoint> MoveFilter::GetFilteredMoves(BasePiece *piece, const QPoint &selectedPiecePos)
{
    QVector<QPoint> moves = piece->GetLegalMoves(gameInfo.friendly, gameInfo.enemy);
    QVector<QPoint> filteredMoves;
    if (!IsKingChecked(gameInfo.dangerAreas))
    {
        if (piece->getType() == PieceType::King)
        {
            filteredMoves = FilterKingMovements(moves, selectedPiecePos, gameInfo.dangerAreas);
        }
        else
        {
            filteredMoves = moves;
        }
    }
    else
    {
        if (piece->getType() == PieceType::King)
        {
            filteredMoves = FilterKingMovements(moves, selectedPiecePos, gameInfo.dangerAreas);
        }
        else
        {
            filteredMoves = FilterAvailableMovements(moves, selectedPiecePos);
        }
    }
    return filteredMoves;
}

QVector<QPoint> MoveFilter::FilterKingMovements(const QVector<QPoint> &moves, const QPoint &kingPos, const QVector<QPoint> &dangerAreas) const
{
    QVector<QPoint> filteredMoves;
    for (auto& m : moves)
    {
        if (!dangerAreas.contains(m))
        {
            filteredMoves.append(m);
        }
    }
    filteredMoves.append(GetCastlingMoves());
    return filteredMoves;
}

QVector<QPoint> MoveFilter::FilterAvailableMovements(const QVector<QPoint> &moves, const QPoint &selectedPiecePos) const
{
    //Filters and returns all the pieces (expect kings) available movements, so that king wont end up in check
    QVector<QPoint> filteredMoves;
    QVector<QPoint> ghost = GetGhostArea(gameInfo.friendly, selectedPiecePos);

    for (auto& p : moves)
    {
        if (IsEatingMovement(p))
        {
            ghost.append(p);
            QVector<QPoint> ghostEnemy = GetGhostArea(gameInfo.enemy, p);
            QVector<QPoint> dangerAreas2 = board->GetDangerAreas(gameInfo.isWhiteTurn, ghost, ghostEnemy, p);

            if (!IsKingChecked(dangerAreas2))
            {
                filteredMoves.append(p);
            }
            ghost.pop_back();
        }
        else
        {
            ghost.append(p);
            QVector<QPoint> dangerAreas = board->GetDangerAreas(gameInfo.isWhiteTurn, ghost, gameInfo.enemy);

            if (!IsKingChecked(dangerAreas))
            {
                filteredMoves.append(p);
            }
            ghost.pop_back();
        }
    }
    return filteredMoves;
}

QVector<QPoint> MoveFilter::GetGhostArea(const QVector<QPoint> &field, const QPoint &pos) const
{
    QVector<QPoint> ghostArea;
    for (auto& p : field)
    {
        if (p != pos)
        {
            ghostArea.append(p);
        }
    }
    return ghostArea;
}

BasePiece *MoveFilter::GetSelectedPiece(const QPoint &pos) const
{
    BasePiece* piece = nullptr;
    for (auto& p : board->pieces)
    {

        if (p->getPos() == pos)
        {
            piece = p.get();
            break;
        }
    }
    return piece;

}

std::unique_ptr<BasePiece> MoveFilter::GivePiecePointer(const QPoint &selectedPiecePos)
{
    for (auto& p : board->pieces)
    {

        if (p->getPos() == selectedPiecePos)
        {
            return std::move(p);
        }
    }
    return nullptr;
}

QPoint MoveFilter::GetKingPos(QBrush color) const
{
    QPoint kingPos;
    for (auto& p : board->pieces)
    {
        if (p->getColor() == color && p->getType() == PieceType::King)
        {
            kingPos = p->getPos();
            break;
        }
    }
    return kingPos;
}

bool MoveFilter::IsKingChecked(const QVector<QPoint> &dangerAreas) const
{
    return dangerAreas.contains(GetKingPos(gameInfo.turn));
}

bool MoveFilter::IsEatingMovement(const QPoint pos) const
{
    return gameInfo.enemy.contains(pos);
}

QVector<QPoint> MoveFilter::GetCastlingMoves() const
{
    QVector<QPoint> castlingMoves;


    if (IsKingChecked(gameInfo.dangerAreas))
    {
        return castlingMoves;
    }

    QVector<QPoint> pieceArea;
    bool leftRook = false;
    bool rightRook = false;

    for (auto& p : board->pieces)
    {
        if (p->getColor() == gameInfo.turn)
        {
            pieceArea.append(p->getPos());
            if (p->getType() == PieceType::King && p->GetHashMoved())
            {
                return castlingMoves;
            }
            if (p->getType() == PieceType::Rook)
            {
                if (!p->GetHashMoved() && (p->getPos() == QPoint(0, 7) || p->getPos() == QPoint(0, 0)))
                {
                    leftRook = true;
                }
                if (!p->GetHashMoved() && (p->getPos() == QPoint(7, 7) || p->getPos() == QPoint(7, 0)))
                {
                    rightRook = true;
                }
            }
        }
    }

    if (!leftRook && !rightRook)
    {
        return castlingMoves;
    }

    QVector<QPoint> checkAreaLeft;
    QVector<QPoint> checkAreaRight;

    if (gameInfo.isWhiteTurn)
    {
        if (leftRook)
        {
            checkAreaLeft.append(QPoint(1, 7));
            checkAreaLeft.append(QPoint(2, 7));
            checkAreaLeft.append(QPoint(3, 7));
        }
        if (rightRook)
        {
            checkAreaRight.append(QPoint(5, 7));
            checkAreaRight.append(QPoint(6, 7));
        }
    }
    else
    {
        if (leftRook)
        {
            checkAreaLeft.append(QPoint(1, 0));
            checkAreaLeft.append(QPoint(2, 0));
            checkAreaLeft.append(QPoint(3, 0));
        }
        if (rightRook)
        {
            checkAreaRight.append(QPoint(5, 0));
            checkAreaRight.append(QPoint(6, 0));
        }
    }

    if (leftRook)
    {
        if (std::none_of(checkAreaLeft.begin(), checkAreaLeft.end(), [&] (QPoint& p)
                         {
                             return std::any_of(pieceArea.begin(), pieceArea.end(), [p] (QPoint& a) { return a == p;}) ||
                                    std::any_of(gameInfo.dangerAreas.begin(), gameInfo.dangerAreas.end(), [p] (QPoint& a)
                                                {
                                                    if (p == QPoint(1,0) || p == QPoint(1, 7))
                                                    {
                                                        return false;
                                                    }
                                                    else
                                                    {
                                                        return a == p;
                                                    }
                                                });

                         }))
        {
            int y = gameInfo.isWhiteTurn ? 7 : 0;
            castlingMoves.append(QPoint(0, y));
        }
    }

    if (rightRook)
    {
        if (std::none_of(checkAreaRight.begin(), checkAreaRight.end(), [&] (QPoint& p)
                         {
                             return std::any_of(pieceArea.begin(), pieceArea.end(), [p] (QPoint& a) { return a == p;}) ||
                                    std::any_of(gameInfo.dangerAreas.begin(), gameInfo.dangerAreas.end(), [p] (QPoint& a) {return a == p;});

                         }))
        {
            int y = gameInfo.isWhiteTurn ? 7 : 0;
            castlingMoves.append(QPoint(7, y));
        }
    }
    return castlingMoves;
}
