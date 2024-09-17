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

                if (IsPawnPromotionMove(piece, pos))
                {
                    PromotionDialog dialog;
                    if (dialog.exec() == QDialog::Accepted)
                    {
                        PieceType promotedPiece = dialog.getSelectedPiece();
                        emit PawnPromotion(selectedPiecePos, promotedPiece, isWhiteTurn);
                    }

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
    if (InsufficientMaterialDraw())
    {
        emit UpdateUiForGameOver(EndStatus::InsufficientMaterialDraw);
        return;
    }

    if (IsThreeRepetitionDraw())
    {
        emit UpdateUiForGameOver(EndStatus::ThreefoldDraw);
        return;
    }

    if (IsDeadPositionDraw())
    {
        emit UpdateUiForGameOver(EndStatus::DeadDraw);
        return;
    }

    if (Is50MoveDraw())
    {
        emit UpdateUiForGameOver(EndStatus::FiftyMovesDraw);
        return;
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

bool Game::InsufficientMaterialDraw() const
{
    int whiteCount = 0;
    int blackCount = 0;
    for (auto& p : board->pieces)
    {
        if (p->getType() == PieceType::Queen || p->getType() == PieceType::Rook || p->getType() == PieceType::Pawn)
        {
            return false;
        }
        else if (p->getType() == PieceType::King)
        {
            continue;
        }
        else
        {
            if (p->getColor() == Qt::white)
            {
                whiteCount++;
            }
            else
            {
                blackCount++;
            }
            if (whiteCount >= 2 || blackCount >= 2)
            {
                return false;
            }
        }
    }
    return true;
}

bool Game::IsThreeRepetitionDraw() const
{
    if (board->oldPositions.size() < 3)
    {
        return false;
    }
    int repetitions = 0;

    for (int i = 0; i < board->oldPositions.size(); ++i)
    {
        int currentRepetitions = 1;
        for (int j = i + 1; j < board->oldPositions.size(); ++j)
        {
            if (ArePositionsEqual(board->oldPositions[i], board->oldPositions[j]))
            {
                currentRepetitions++;
                if (currentRepetitions == 3)
                {
                    return true;
                }
            }
        }
    }
    return false;

}

bool Game::IsDeadPositionDraw() const
{
    QVector<QPoint> threatAreas;
    QVector<QPoint> turnPawns;
    QVector<QPoint> enemyPawns;
    QPoint whiteKingPos;
    QPoint blackKingPos;
    QBrush turn = isWhiteTurn ? Qt::white : Qt::black;
    int pawnCount = 0;
    for (auto& p : board->pieces)
    {
        if (p->getType() != PieceType::King && p->getType() != PieceType::Pawn)
        {
            return false;
        }
        if (p->getType() == PieceType::Pawn)
        {
            pawnCount++;
            if (p->getColor() == turn)
            {
                if (p->GetLegalMoves(friendly, enemy).size() != 0)
                {
                    return false;
                }
                turnPawns.append(p->getPos());

            }
            else
            {
                if (p->GetLegalMoves(enemy, friendly).size() != 0)
                {
                    return false;
                }
                else
                {
                    threatAreas.append(p->GetThreateningMoves(friendly, enemy));
                    enemyPawns.append(p->getPos());
                }
            }
        }
        else
        {
            if (p->getColor() == Qt::white)
            {
                whiteKingPos = p->getPos();
            }
            else
            {
                blackKingPos = p->getPos();
            }
        }
    }

    if (pawnCount < 6)
    {
        return false;
    }
    std::sort(turnPawns.begin(), turnPawns.end(), [] (QPoint l, QPoint r) {return l.x() < r.x();});
    for (int i = 0, j = i + 1; i < turnPawns.size() - 1; i++, j++)
    {
        int xLen = turnPawns[j].x() - turnPawns[i].x();
        int yLen = turnPawns[j].y() - turnPawns[i].y();
        int totalLen = xLen + abs(yLen);
        if (totalLen > 3)
        {
            return false;
        }
    }

    if (whiteKingPos.y() - 2 <= blackKingPos.y())
    {
        return false;
    }

    for (auto& p : board->pieces)
    {
        if (p->getType() == PieceType::Pawn && p->getColor() == turn)
        {
            QPoint pPos = p->getPos();

            QPoint left = QPoint(pPos.x() - 1, pPos.y());
            QPoint right = QPoint(pPos.x() + 1, pPos.y());

            if (pPos.x() == 0)
            {
                if (!threatAreas.contains(right))
                {
                    return false;
                }
            }
            else if (pPos.x() == 7)
            {
                if (!threatAreas.contains(left))
                {
                    return false;
                }
            }
            else
            {
                if (!threatAreas.contains(left) || !threatAreas.contains(right))
                {
                    return false;
                }
                if (pPos.x() == 2)
                {
                    QPoint left2 = QPoint(pPos.x() - 2, pPos.y());
                    if (board->IsInsideBoard(left2) && (!threatAreas.contains(left2) && !turnPawns.contains(left2) && enemyPawns.contains(left2)))
                    {
                        return false;
                    }
                }
                if (pPos.x() == 5)
                {
                    QPoint right2 = QPoint(pPos.x() + 2, pPos.y());
                    if (board->IsInsideBoard(right2) && (!threatAreas.contains(right2) && !turnPawns.contains(right2) && enemyPawns.contains(right2)))
                    {
                        return false;
                    }
                }
            }
        }
    }
    return true;
}

bool Game::Is50MoveDraw() const
{
    return board->GetMoveCount() > 100;
}

bool Game::IsPawnPromotionMove(BasePiece *piece, const QPoint pos) const
{
    if (piece->getType() == PieceType::Pawn)
    {
        int edge = isWhiteTurn ? 0 : 7;
        if (pos.y() == edge)
        {
            return true;
        }
        return false;
    }
    return false;
}

bool Game::ArePositionsEqual(const QVector<PieceStateRecord>& pos1, const QVector<PieceStateRecord>& pos2) const
{
    if (pos1.size() != pos2.size())
    {
        return false;
    }

    for (int i = 0; i < pos1.size(); ++i)
    {
        if (pos1[i] != pos2[i])
        {
            return false;
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

    if (!leftRook && !rightRook)
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

    if (leftRook)
    {
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
    }


    if (rightRook)
    {
        if (std::none_of(checkAreaRight.begin(), checkAreaRight.end(), [&] (QPoint& p)
                         {
                             return std::any_of(pieceArea.begin(), pieceArea.end(), [p] (QPoint& a) { return a == p;}) ||
                                    std::any_of(dangerAreas.begin(), dangerAreas.end(), [p] (QPoint& a) {return a == p;});

                         }))
        {
            int y = isWhiteTurn ? 7 : 0;
            castlingMoves.append(QPoint(7, y));
        }
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
