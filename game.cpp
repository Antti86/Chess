#include "game.h"
#include "Pawn.h"

Game::Game(ChessBoard *board, QObject *parent)
    : QObject(parent)
    , board(board)
{
    isWhiteTurn = true;
    SetAreaFields();
    isPieceSelected = false;

}

void Game::handlePieceSelection(QPoint pos)
{
    if (!isPieceSelected)
    {
        if (IsPieceOnSelectedSquare(pos))
        {
            selectedPiecePos = pos;
            isPieceSelected = true;
            BasePiece* piece = GetSelectedPiece(pos);
            QVector<QPoint> moves = piece->GetLegalMoves(friendly, enemy);
            QVector<QPoint> dangerAreas = board->GetDangerAreas(isWhiteTurn, GetGhostArea(friendly, selectedPiecePos), enemy);

            if (!IsKingChecked(dangerAreas))
            {
                if (piece->getType() == PieceType::King)
                {
                    QVector<QPoint> filteredMoves = FilterKingMovements(moves);
                    emit SetSquareColor(selectedPiecePos, filteredMoves, true);
                }
                else
                {
                    emit SetSquareColor(selectedPiecePos, moves, true);
                }
            }
            else
            {
                if (piece->getType() == PieceType::King)
                {
                    QVector<QPoint> filteredMoves = FilterKingMovements(moves);
                    emit SetSquareColor(selectedPiecePos, filteredMoves, true);
                }
                else
                {
                    QVector<QPoint> filteredMoves = FilterAvailableMovements(moves);
                    emit SetSquareColor(selectedPiecePos, filteredMoves, true);
                }
            }
        }
    }
    else
    {
        if (pos == selectedPiecePos)
        {
            isPieceSelected = false;
            BasePiece* piece = GetSelectedPiece(pos);
            QVector<QPoint> moves = piece->GetLegalMoves(friendly, enemy);

            if (piece->getType() == PieceType::King)
            {
                QVector<QPoint> filteredMoves = FilterKingMovements(moves);
                emit SetSquareColor(selectedPiecePos, filteredMoves, false);
            }
            else
            {
                emit SetSquareColor(selectedPiecePos, moves, false);
            }
        }
        else
        {
            BasePiece* piece = GetSelectedPiece(selectedPiecePos);
            QVector<QPoint> moves = piece->GetLegalMoves(friendly, enemy);
            QVector<QPoint> filteredMoves;
            QVector<QPoint> dangerAreas = board->GetDangerAreas(isWhiteTurn, GetGhostArea(friendly, selectedPiecePos), enemy);

            if (!IsKingChecked(dangerAreas))
            {
                if (piece->getType() == PieceType::King)
                {
                    filteredMoves = FilterKingMovements(moves);
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
                    filteredMoves = FilterKingMovements(moves);
                }
                else
                {
                    filteredMoves = FilterAvailableMovements(moves);
                }
            }
            if (ValidMovement(piece, filteredMoves, pos))
            {
                emit SetSquareColor(selectedPiecePos, filteredMoves, false);
                if (IsPassantMovement(piece, selectedPiecePos, pos))
                {
                    int fix = isWhiteTurn ? 1 : -1;
                    QPoint fixed = QPoint(pos.x(), pos.y() + fix);
                    emit EatPiece(fixed);
                }

                if (IsEatingMovement(pos))
                {
                    emit EatPiece(pos);
                }

                BasePiece* castlingRook = IsCastlingMove(piece, pos);
                if (castlingRook)
                {
                    QPoint kingNewPos;
                    QPoint RookNewPos;
                    if (pos.x() < 3)
                    {
                        kingNewPos = QPoint(2, pos.y());
                        RookNewPos = QPoint(3, pos.y());
                    }
                    else
                    {
                        kingNewPos = QPoint(6, pos.y());
                        RookNewPos = QPoint(5, pos.y());
                    }
                    castlingRook->Move(RookNewPos);
                    emit pieceMoved(selectedPiecePos, kingNewPos, isWhiteTurn);
                }
                else
                {
                    emit pieceMoved(selectedPiecePos, pos, isWhiteTurn);
                }
                isPieceSelected = false;
            }
        }
    }
}

void Game::EndOfTurn()
{
    isWhiteTurn = !isWhiteTurn;
    SetAreaFields();
    QVector<QPoint> dangerAreas = board->GetDangerAreas(isWhiteTurn, friendly, enemy);
    bool isChecked = IsKingChecked(dangerAreas);

    if (isChecked)
    {
        if (IsCheckMate())
        {
            EndStatus status = isWhiteTurn ? EndStatus::WhiteWins : EndStatus::BlackWins;
            emit UpdateUiForGameOver(status);
            return;
        }
    }
    else
    {
        if (IsStaleMate())
        {
            emit UpdateUiForGameOver(EndStatus::StaleMate);
            return;
        }
    }
    emit UpdateUiForCheck(isChecked);
    emit UpdateUiToTurn(isWhiteTurn);


}


bool Game::IsPieceOnSelectedSquare(QPoint square) const
{
    if (isWhiteTurn)
    {
        for (auto &piece : board->pieces)
        {
            if (piece->getPos() == square && piece->getColor() == Qt::white)
            {
                return true;
            }
        }
        return false;
    }
    else
    {
        for (auto &piece : board->pieces)
        {
            if (piece->getPos() == square && piece->getColor() == Qt::black)
            {
                return true;
            }
        }
        return false;
    }
}

bool Game::ValidMovement(BasePiece* piece, QVector<QPoint> legalMoves, QPoint& pos) const
{
    return piece && legalMoves.contains(pos);
}

bool Game::IsEatingMovement(const QPoint pos) const
{
    return enemy.contains(pos);
}

bool Game::IsPassantMovement(BasePiece* piece, const QPoint &selectedPiecePos, const QPoint &newpos) const
{
    QPoint checkPosLeft = QPoint(selectedPiecePos.x() - 1, selectedPiecePos.y() );
    QPoint checkPosRight = QPoint(selectedPiecePos.x() + 1, selectedPiecePos.y() );

    if (piece->getType() == PieceType::Pawn)
    {
        Pawn *pawn = dynamic_cast<Pawn*>(piece);
        if (newpos.x() != selectedPiecePos.x())
        {
            if (pawn->canPassantLeft)
            {
                return enemy.contains(checkPosLeft);
            }

            if (pawn->canPassantRight)
            {
                return enemy.contains(checkPosRight);
            }
        }
        return false;
    }
    return false;
}

BasePiece *Game::GetSelectedPiece(const QPoint pos) const
{
    BasePiece* piece = nullptr;
    for (auto& p : board->pieces)
    {
        if (p->getPos() == selectedPiecePos)
        {
            piece = p;
            break;
        }
    }
    return piece;
}

bool Game::IsKingChecked(const QVector<QPoint> &dangerAreas) const
{
    QPoint kingPos;
    QBrush turn = isWhiteTurn ? Qt::white : Qt::black;
    for (auto& p : board->pieces)
    {
        if (p->getType() == PieceType::King && p->getColor() == turn)
        {
            kingPos = p->getPos();
            break;
        }
    }
    return dangerAreas.contains(kingPos);
}

bool Game::IsCheckMate() const
{
    QVector<QPoint> availableMoves;
    QBrush turn = isWhiteTurn ? Qt::white : Qt::black;
    for (auto& p : board->pieces)
    {
        if (p->getColor() == turn)
        {
            QVector<QPoint> legalMoves = p->GetLegalMoves(friendly, enemy);
            if (p->getType() == PieceType::King)
            {
                availableMoves.append(FilterKingMovements(legalMoves));
            }
            else
            {
                availableMoves.append(FilterAvailableMovements(legalMoves));
            }
        }
    }
    return availableMoves.size() == 0;
}

bool Game::IsStaleMate() const
{

    QVector<QPoint> availableMoves;
    QBrush turn = isWhiteTurn ? Qt::white : Qt::black;

    for (auto& p : board->pieces)
    {
        if (p->getColor() == turn)
        {
            if (p->getType() == PieceType::King)
            {
                availableMoves.append(FilterKingMovements(p->GetLegalMoves(friendly, enemy)));
            }
            else
            {
                availableMoves.append(FilterAvailableMovements(p->GetLegalMoves(friendly, enemy)));
            }
            if (!availableMoves.isEmpty())
            {
                return false;
            }
        }
    }
    return true;
}

BasePiece* Game::IsCastlingMove(BasePiece *piece, const QPoint &pos) const
{
    if (piece->getType() == PieceType::King)
    {
        QBrush turn = isWhiteTurn ? Qt::white : Qt::black;

        for (auto& p : board->pieces)
        {
            if (p->getColor() == turn && p->getType() == PieceType::Rook && p->getPos() == pos)
            {
                return p;
            }
        }
    }
    return nullptr;
}


QVector<QPoint> Game::GetCastlingMoves() const
{
    QVector<QPoint> castlingMoves;

    QVector<QPoint> dangerAreas = board->GetDangerAreas(isWhiteTurn, friendly, enemy);

    if (IsKingChecked(dangerAreas))
    {
        return castlingMoves;
    }

    QBrush turn = isWhiteTurn ? Qt::white : Qt::black;

    QVector<QPoint> pieceArea;

    int rookCount;

    bool leftRook = false;
    bool rightRook = false;

    for (auto& p : board->pieces)
    {
        if (p->getColor() == turn)
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

    if (!leftRook || !rightRook)
    {
        return castlingMoves;
    }

    QVector<QPoint> checkAreaLeft;
    QVector<QPoint> checkAreaRight;

    if (isWhiteTurn)
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

    if (std::none_of(checkAreaLeft.begin(), checkAreaLeft.end(), [&] (QPoint& p)
    {
            return std::any_of(pieceArea.begin(), pieceArea.end(), [p] (QPoint& a) { return a == p;}) ||
                   std::any_of(dangerAreas.begin(), dangerAreas.end(), [p] (QPoint& a)
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
        int y = isWhiteTurn ? 7 : 0;
        castlingMoves.append(QPoint(0, y));
    }

    if (std::none_of(checkAreaRight.begin(), checkAreaRight.end(), [&] (QPoint& p)
    {
            return std::any_of(pieceArea.begin(), pieceArea.end(), [p] (QPoint& a) { return a == p;}) ||
                   std::any_of(dangerAreas.begin(), dangerAreas.end(), [p] (QPoint& a) {return a == p;});

    }))
    {
        int y = isWhiteTurn ? 7 : 0;
        castlingMoves.append(QPoint(7, y));
    }

    return castlingMoves;
}

QVector<QPoint> Game::FilterKingMovements(const QVector<QPoint> &moves) const
{
    QVector<QPoint> filteredMoves;
    QVector<QPoint> ghostFriendly = GetGhostArea(friendly, selectedPiecePos);

    QVector<QPoint> dangerAreas = board->GetDangerAreas(isWhiteTurn, ghostFriendly, enemy);

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

void Game::SetAreaFields()
{
    friendly = isWhiteTurn ? board->GetPopulatedAreas(Qt::white) : board->GetPopulatedAreas(Qt::black);
    enemy = isWhiteTurn ? board->GetPopulatedAreas(Qt::black) : board->GetPopulatedAreas(Qt::white);
}

QVector<QPoint> Game::GetGhostArea(const QVector<QPoint>& field, const QPoint& pos) const
{
    QVector<QPoint> ret;
    for (auto& p : field)
    {
        if (p != pos)
        {
            ret.append(p);
        }
    }
    return ret;
}

QVector<QPoint> Game::FilterAvailableMovements(const QVector<QPoint> &moves) const
{
    QVector<QPoint> filteredMoves;
    QVector<QPoint> ghost = GetGhostArea(friendly, selectedPiecePos);

    for (auto& p : moves)
    {
        if (IsEatingMovement(p))
        {
            ghost.append(p);
            QVector<QPoint> ghostEnemy = GetGhostArea(enemy, p);
            QVector<QPoint> dangerAreas2 = board->GetDangerAreas(isWhiteTurn, ghost, ghostEnemy, p);

            if (!IsKingChecked(dangerAreas2))
            {
                filteredMoves.append(p);
            }
            ghost.pop_back();
        }
        else
        {
            ghost.append(p);
            QVector<QPoint> dangerAreas = board->GetDangerAreas(isWhiteTurn, ghost, enemy);

            if (!IsKingChecked(dangerAreas))
            {
                filteredMoves.append(p);
            }
            ghost.pop_back();
        }
    }
    return filteredMoves;
}
