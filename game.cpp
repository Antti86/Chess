#include "game.h"
#include "Bishop.h"
#include "King.h"
#include "Knight.h"
#include "Pawn.h"
#include "PromotionDialog.h"
#include "Queen.h"
#include "Rook.h"

Game::Game(ChessBoard *board, QObject *parent)
    : QObject(parent)
    , board(board)
    , gameInfo(board)
    , filter(MoveFilter(board, gameInfo))
    , bot(Bot(board, gameInfo, filter))
{
    isPieceSelected = false;
}

void Game::handlePieceSelection(QPoint pos)
{
    if (!isPieceSelected) //Selects a piece
    {
        if (IsPieceOnSelectedSquare(pos))
        {
            selectedPiecePos = pos;
            isPieceSelected = true;

            QVector<QPoint> filteredMoves = filter.GetFilteredMoves(selectedPiecePos);
            emit SetSquareColor(selectedPiecePos, filteredMoves, true);
        }
    }
    else
    {
        if (pos == selectedPiecePos) //Realeses selected piece
        {
            isPieceSelected = false;
            QVector<QPoint> filteredMoves = filter.GetFilteredMoves(pos);
            emit SetSquareColor(selectedPiecePos, filteredMoves, false);
        }
        else //Moving a piece path
        {
            BasePiece* piece = filter.GetSelectedPiece(selectedPiecePos);
            QVector<QPoint> filteredMoves = filter.GetFilteredMoves(selectedPiecePos);

            if (ValidMovement(piece, filteredMoves, pos))
            {
                emit SetSquareColor(selectedPiecePos, filteredMoves, false);
                if (IsPassantMovement(piece, selectedPiecePos, pos))
                {
                    int fix = gameInfo.isWhiteTurn ? 1 : -1;
                    QPoint fixed = QPoint(pos.x(), pos.y() + fix);
                    emit EatPiece(fixed);
                }

                if (filter.IsEatingMovement(pos))
                {
                    emit EatPiece(pos);
                }

                if (IsPawnPromotionMove(piece, pos))
                {
                    PromotionDialog dialog(gameInfo.isWhiteTurn);
                    if (dialog.exec() == QDialog::Accepted)
                    {
                        PieceType promotedPiece = dialog.getSelectedPiece();
                        emit PawnPromotion(selectedPiecePos, promotedPiece, gameInfo.isWhiteTurn);
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
                    emit pieceMoved(selectedPiecePos, kingNewPos, gameInfo.isWhiteTurn);
                }
                else
                {
                    emit pieceMoved(selectedPiecePos, pos, gameInfo.isWhiteTurn);

                }
                isPieceSelected = false;
            }
        }
    }
}


void Game::BotMovement()
{

    QVector<Movement> filteredTestMoves;

    for (const auto& p: board->pieces)
    {
        if (p->getColor() == Qt::black)
        {
            selectedPiecePos = p->getPos();
            filteredTestMoves.append(Movement(selectedPiecePos, filter.GetFilteredMoves(selectedPiecePos)));
        }
    }

    QPoint kingPos; //Refactor this
    for (auto& p : board->pieces)
    {
        if (p->getColor() == Qt::white && p->getType() == PieceType::King)
        {
            kingPos = p->getPos();
            break;
        }
    }

    QVector<MovementScore> fMove;

    int bestScore = 0;

    for (auto& selpiece : filteredTestMoves)
    {
        auto tt = selpiece.From;
        BasePiece* p = filter.GetSelectedPiece(selpiece.From);
        for (auto& m : selpiece.To)
        {
            int score = 0;

            BasePiece* capPiece = nullptr;
            PieceType capType;
            QPoint capPos;
            bool capHasMoved;
            bool capCanPassLeft;
            bool capCanPassRight;
            bool eating = filter.IsEatingMovement(m);
            if (filter.IsEatingMovement(m))
            {
                capPiece = filter.GetSelectedPiece(m);
                score += capPiece->GetPieceScore();
                capType = capPiece->getType();
                capPos = capPiece->getPos();
                capHasMoved = capPiece->hasMoved;
                capCanPassLeft = capPiece->canPassantLeft;
                capCanPassRight = capPiece->canPassantRight;
                emit EatPiece(m);
            }
            p->Move(m);

            auto dangerArea = board->GetDangerAreas(true, gameInfo.enemy, gameInfo.friendly);

            if (dangerArea.contains(kingPos))
            {
                score += 100;
            }




            if (score > bestScore)
            {
                bestScore = score;
            }
            fMove.append(MovementScore(selpiece.From, m, score));
            if (eating)
            {
                p->Move(selpiece.From);
                std::unique_ptr<BasePiece> newPiece;
                switch (capType)
                {
                case PieceType::Bishop:
                    newPiece = std::make_unique<Bishop>(Qt::white, capPos, capType);
                    break;
                case PieceType::Pawn:
                    newPiece = std::make_unique<Pawn>(Qt::white, capPos, capType);
                    break;
                case PieceType::Queen:
                    newPiece = std::make_unique<Queen>(Qt::white, capPos, capType);
                    break;
                case PieceType::King:
                    newPiece = std::make_unique<King>(Qt::white, capPos, capType);
                    break;
                case PieceType::Knight:
                    newPiece = std::make_unique<Knight>(Qt::white, capPos, capType);
                    break;
                case PieceType::Rook:
                    newPiece = std::make_unique<Rook>(Qt::white, capPos, capType);
                    break;
                }
                newPiece->hasMoved = capHasMoved;
                newPiece->canPassantLeft = capCanPassLeft;
                newPiece->canPassantRight = capCanPassRight;

                board->AddPiece(std::move(newPiece));
            }
            else
            {
                p->Move(selpiece.From);
            }

        }
        p = nullptr;
    }

    auto final = std::max_element(fMove.begin(), fMove.end());

    selectedPiecePos = final->From;

    QPoint pos = final->To;

    QVector<QPoint> filteredMoves = filter.GetFilteredMoves(selectedPiecePos);
    isPieceSelected = true;

    // QPoint pos = bot.SelectMove(filteredMoves);

    BasePiece* piece = filter.GetSelectedPiece(selectedPiecePos);

    if (ValidMovement(piece, filteredMoves, pos))
    {
        if (IsPassantMovement(piece, selectedPiecePos, pos))
        {
            int fix = gameInfo.isWhiteTurn ? 1 : -1;
            QPoint fixed = QPoint(pos.x(), pos.y() + fix);
            emit EatPiece(fixed);
        }

        if (filter.IsEatingMovement(pos))
        {
            emit EatPiece(pos);
        }

        if (IsPawnPromotionMove(piece, pos))
        {
            emit PawnPromotion(selectedPiecePos, PieceType::Queen, gameInfo.isWhiteTurn);
            // PromotionDialog dialog(isWhiteTurn);
            // if (dialog.exec() == QDialog::Accepted)
            // {
            //     PieceType promotedPiece = dialog.getSelectedPiece();
            //     emit PawnPromotion(selectedPiecePos, promotedPiece, isWhiteTurn);
            // }

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
            emit pieceMoved(selectedPiecePos, kingNewPos, gameInfo.isWhiteTurn);
        }
        else
        {
            emit pieceMoved(selectedPiecePos, pos, gameInfo.isWhiteTurn);

        }
        isPieceSelected = false;
    }

}


void Game::EndOfTurn()
{
    gameInfo.SetAreaFields();

    QVector<QPoint> dangerAreas = board->GetDangerAreas(gameInfo.isWhiteTurn, gameInfo.friendly, gameInfo.enemy);
    bool isChecked = filter.IsKingChecked(dangerAreas);

    if (isChecked)
    {
        if (IsCheckMate())
        {
            EndStatus status = gameInfo.isWhiteTurn ? EndStatus::BlackWins : EndStatus::WhiteWins;
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
    emit UpdateUiToTurn(gameInfo.isWhiteTurn);

    if (!gameInfo.isWhiteTurn)
    {
        BotMovement();
    }


}


bool Game::IsPieceOnSelectedSquare(QPoint square) const
{
    if (gameInfo.isWhiteTurn)
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
                return gameInfo.enemy.contains(checkPosLeft);
            }

            if (pawn->canPassantRight)
            {
                return gameInfo.enemy.contains(checkPosRight);
            }
        }
        return false;
    }
    return false;
}

bool Game::IsCheckMate() const
{
    QVector<QPoint> availableMoves;
    for (auto& p : board->pieces)
    {
        if (p->getColor() == gameInfo.turn)
        {
            QVector<QPoint> legalMoves = p->GetLegalMoves(gameInfo.friendly, gameInfo.enemy);
            if (p->getType() == PieceType::King)
            {
                availableMoves.append(filter.FilterKingMovements(legalMoves, filter.GetKingPos(gameInfo.turn)));
            }
            else
            {
                availableMoves.append(filter.FilterAvailableMovements(legalMoves, p->getPos()));
            }
        }
    }
    return availableMoves.size() == 0;
}

bool Game::IsStaleMate() const
{
    QVector<QPoint> availableMoves;
    for (auto& p : board->pieces)
    {
        if (p->getColor() == gameInfo.turn)
        {
            if (p->getType() == PieceType::King)
            {
                availableMoves.append(filter.FilterKingMovements(p->GetLegalMoves(gameInfo.friendly, gameInfo.enemy), filter.GetKingPos(gameInfo.turn)));
            }
            else
            {
                availableMoves.append(filter.FilterAvailableMovements(p->GetLegalMoves(gameInfo.friendly, gameInfo.enemy), p->getPos()));
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
    if (board->repetitionTrack.size() < 3)
    {
        return false;
    }
    int repetitions = 0;

    for (int i = 0; i < board->repetitionTrack.size(); ++i)
    {
        int currentRepetitions = 1;
        for (int j = i + 1; j < board->repetitionTrack.size(); ++j)
        {
            if (ArePositionsEqual(board->repetitionTrack[i], board->repetitionTrack[j]))
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

bool Game::ArePositionsEqual(const QVector<PieceStateRecord>& pos1, const QVector<PieceStateRecord>& pos2) const
{
    //IsThreeRepetitionDraw uses this
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

bool Game::IsDeadPositionDraw() const
{
    QVector<QPoint> threatAreas;
    QVector<QPoint> turnPawns;
    QVector<QPoint> enemyPawns;
    QPoint whiteKingPos;
    QPoint blackKingPos;
    int pawnCount = 0;

    //Checks available movements and count the pieces
    for (auto& p : board->pieces)
    {
        if (p->getType() != PieceType::King && p->getType() != PieceType::Pawn)
        {
            return false;
        }
        if (p->getType() == PieceType::Pawn)
        {
            pawnCount++;
            if (p->getColor() == gameInfo.turn)
            {
                if (p->GetLegalMoves(gameInfo.friendly, gameInfo.enemy).size() != 0)
                {
                    return false;
                }
                turnPawns.append(p->getPos());
            }
            else
            {
                if (p->GetLegalMoves(gameInfo.enemy, gameInfo.friendly).size() != 0)
                {
                    return false;
                }
                else
                {
                    threatAreas.append(p->GetThreateningMoves(gameInfo.friendly, gameInfo.enemy));
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
    // Needs least 6 pawns that this draw is possible
    if (pawnCount < 6)
    {
        return false;
    }
    //Distance calculations. Makes sure that pawns are not to far a part
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
    //This is for checking that kings are their own sides (verticaly)
    if (whiteKingPos.y() - 2 <= blackKingPos.y())
    {
        return false;
    }

    //Checks that kings cant move across the board
    for (auto& p : board->pieces)
    {
        if (p->getType() == PieceType::Pawn && p->getColor() == gameInfo.turn)
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
    // In this draw one Move is consider when both players make a move. Thats why moveCount limit is 100
    return board->GetMoveCount() > 100;
}

bool Game::IsPawnPromotionMove(BasePiece *piece, const QPoint pos) const
{
    if (piece->getType() == PieceType::Pawn)
    {
        int edge = gameInfo.isWhiteTurn ? 0 : 7;
        if (pos.y() == edge)
        {
            return true;
        }
        return false;
    }
    return false;
}


BasePiece* Game::IsCastlingMove(BasePiece *piece, const QPoint &pos) const
{
    if (piece->getType() == PieceType::King)
    {
        for (auto& p : board->pieces)
        {
            if (p->getColor() == gameInfo.turn && p->getType() == PieceType::Rook && p->getPos() == pos)
            {
                return p.get();
            }
        }
    }
    return nullptr;
}
