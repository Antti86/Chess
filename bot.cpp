#include "bot.h"


Bot::Bot(ChessBoard *board, GameInfo& gameInfo, MoveFilter &filter)
    : board(board)
    , gameInfo(gameInfo)
    , filter(filter)
{

}

MovementScore Bot::GetMinMaxMove()
{
    auto filteredMoves = GetAllTheMovements(false);
    bestMove = filteredMoves[rng.CreateInt(0, filteredMoves.size() - 1)];
    counter = 0;
    NegaMaxAlphaBeta(DEPTH, filteredMoves, -1, -CheckMate, CheckMate);
    return bestMove;
}


int Bot::NegaMaxAlphaBeta(int depth, QVector<MovementScore> &filteredMoves, int turnMPlyier, int alpha, int beta)
{

    counter++;
    if (depth == 0)
    {
        return turnMPlyier * ScoreTheBoard() + filteredMoves.size();
    }
    int maxScore = -CheckMate;
    for (auto& move : filteredMoves)
    {

        BasePiece* p = filter.GetSelectedPiece(move.From);
        bool hasMoved = p->hasMoved;

        std::unique_ptr<BasePiece> capturedPiece = DoMove(p, move.To);

        gameInfo.SetAreaFields();

        auto filteredMoves2 = GetAllTheMovements(gameInfo.isWhiteTurn);


        int score = -NegaMaxAlphaBeta(depth - 1, filteredMoves2, -turnMPlyier, -beta, -alpha);

        if (score > maxScore)
        {
            maxScore = score;
            if (depth == DEPTH)
            {
                bestMove = move;
            }

        }

        gameInfo.SetAreaFields();
        UnDoMove(p, std::move(capturedPiece), move.From, hasMoved);

        if (maxScore > alpha)
        {
            alpha = maxScore;
        }

        if (alpha >= beta)
        {
            break;
        }

    }
    return maxScore;
}


bool Bot::IsCheckMateMovement(bool isWhiteTurn) const
{
    bool end = gameInfo.isWhiteTurn;

    gameInfo.SetTurn(isWhiteTurn);

    QVector<QPoint> availableMoves;
    for (auto& p : board->pieces)
    {
        if (p->getColor() == gameInfo.turn)
        {
            QVector<QPoint> legalMoves = p->GetLegalMoves(gameInfo.enemy, gameInfo.friendly);
            if (p->getType() == PieceType::King)
            {
                availableMoves.append(filter.FilterKingMovements(legalMoves, filter.GetKingPos(gameInfo.turn), gameInfo.dangerAreas));
            }
            else
            {
                availableMoves.append(filter.FilterAvailableMovements(legalMoves, p->getPos()));
            }
        }
    }
    gameInfo.SetTurn(end);
    return availableMoves.size() == 0;
}

bool Bot::IsKingCheck(bool isWhiteTurn) const
{
    bool end = gameInfo.isWhiteTurn;

    gameInfo.SetTurn(isWhiteTurn);
    QPoint enemyKingPos = filter.GetKingPos(gameInfo.turn);
    auto enemyDangerArea = board->GetDangerAreas(isWhiteTurn, gameInfo.enemy, gameInfo.friendly);



    gameInfo.SetTurn(end);

    return enemyDangerArea.contains(enemyKingPos);

}


QVector<MovementScore> Bot::GetAllTheMovements(bool isWhiteTurn)
{
    QVector<MovementScore> filteredTestMoves;

    QBrush turn = isWhiteTurn ? Qt::white : Qt::black;

    for (const auto& p: board->pieces)
    {
        int score = p->GetPieceScore();
        if (p->getColor() == turn)
        {
            auto moves = filter.GetFilteredMoves(p.get(), p->getPos());
            // auto moves = filter.GetFilteredMoves( p->getPos());
            // auto moves = p->GetLegalMoves(gameInfo.friendly, gameInfo.enemy);
            for (auto& m : moves)
            {
                if (filter.IsEatingMovement(m))
                {
                    score += 1000;
                }
                filteredTestMoves.append(MovementScore(p->getPos(), m, score));
            }

        }
    }
    std::sort(filteredTestMoves.begin(), filteredTestMoves.end(), [] (MovementScore& l, MovementScore& r) {return l.score > r.score;});
    return filteredTestMoves;
}

int Bot::ScoreTheBoard()
{
    int score = 0;

    // Status Scoring
    // True = white     False = black
    // if (IsCheckMateMovement(true))
    // {
    //     return CheckMate;
    // }
    // if (IsCheckMateMovement(false))
    // {
    //     return -CheckMate;
    // }

    // if (IsKingCheck(true))
    // {
    //     score += Check;
    // }
    // if (IsKingCheck(false))
    // {
    //     score -= Check;
    // }



    if (filter.IsKingChecked(gameInfo.dangerAreas))
    {
        if (gameInfo.isWhiteTurn)
        {
            score -= Check;
        }
        else
        {
            score += Check;
        }
    }

    // Material Scoring
    for (auto& p : board->pieces)
    {
        int posScore = 0;
        int coverBonus = 0;
        int threatBonus = p->GetThreateningMoves(gameInfo.friendly, gameInfo.enemy).size() * 2;


        if (p->getType() == PieceType::Pawn)
        {
            if (p->getPos().x() > 2 && p->getPos().x() < 6 &&
                p->getPos().y() > 2 && p->getPos().y() < 6)
            {
                posScore = 30;
            }
        }
        if (p->getType() == PieceType::Knight)
        {
            posScore = p->GetLegalMoves(gameInfo.friendly, gameInfo.enemy).size() * 4;
        }

        if (p->getColor() == Qt::white)
        {
            score += p->GetPieceScore() + posScore + coverBonus + threatBonus;
        }
        else
        {
            score -= p->GetPieceScore() + posScore + coverBonus + threatBonus;
        }
    }
    return score;
}


std::unique_ptr<BasePiece> Bot::DoMove(BasePiece* p, const QPoint &movePos)
{
    BasePiece* capPiece = filter.GetSelectedPiece(movePos);

    std::unique_ptr<BasePiece> capturePiece;

    if (capPiece)
    {

        capturePiece = filter.GivePiecePointer(movePos);

        // for (auto iter = board->pieces.begin(); iter != board->pieces.end();)
        // {
        //     if (!*iter)
        //     {
        //         std::swap(*iter, board->pieces.back());
        //         board->pieces.pop_back();
        //     }
        //     else
        //     {
        //         ++iter;
        //     }
        // }

        for (int i = 0; i < board->pieces.size();)
        {
            if (board->pieces[i] == nullptr)
            {
                std::swap(board->pieces[i], board->pieces[board->pieces.size() - 1]);
                board->pieces.pop_back();
            }
            else
            {
                ++i;
            }
        }
        gameInfo.DeleteEnemyAreaSpot(movePos);


        // auto it = std::remove_if(board->pieces.begin(), board->pieces.end(), [](const std::unique_ptr<BasePiece>& piece)
        //                          {
        //                              if (!piece)
        //                              {
        //                                  return true;
        //                              }
        //                              return false;
        //                          });

        // board->pieces.erase(it, board->pieces.end());

    }
    gameInfo.UpdateAreaFields(p->getPos(), movePos);
    p->Move(movePos);

    return std::move(capturePiece);

}

void Bot::UnDoMove(BasePiece *p, std::unique_ptr<BasePiece> capPiece, const QPoint &prePos, bool hasMoved)
{

    if (capPiece)
    {
        board->pieces.push_back(std::move(capPiece));
        gameInfo.enemy.append(capPiece->getPos());

    }

    gameInfo.UpdateAreaFields(p->getPos(), prePos);

    p->Move(prePos);
    p->hasMoved = hasMoved;

}


