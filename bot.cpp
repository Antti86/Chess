#include "bot.h"


Bot::Bot(ChessBoard *board, GameInfo& gameInfo, int DEPTH, MoveFilter &filter)
    : board(board)
    , gameInfo(gameInfo)
    , filter(filter)
    , DEPTH(DEPTH)
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

int Bot::GetCounter() const
{
    return counter;
}


int Bot::NegaMaxAlphaBeta(int depth, QVector<MovementScore> &filteredMoves, int turnMPlyier, int alpha, int beta)
{

    counter++;
    if (depth == 0)
    {
        return turnMPlyier * ScoreTheBoard(filteredMoves) + filteredMoves.size();
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
    if (isWhiteTurn != end)
    {
        gameInfo.SetTurn(isWhiteTurn);
    }

    if (!filter.IsKingChecked(gameInfo.dangerAreas))
    {
        if (isWhiteTurn != end)
        {
            gameInfo.SetTurn(end);
        }
        return false;
    }


    QVector<QPoint> availableMoves;
    for (auto& p : board->pieces)
    {
        if (p->getColor() == gameInfo.turn)
        {
            QVector<QPoint> legalMoves = p->GetLegalMoves(gameInfo.friendly, gameInfo.enemy);
            if (p->getType() == PieceType::King)
            {
                availableMoves.append(filter.FilterKingMovements(legalMoves, filter.GetKingPos(gameInfo.turn), gameInfo.dangerAreas));
            }
            else
            {
                availableMoves.append(filter.FilterAvailableMovements(legalMoves, p->getPos()));
            }
            if (availableMoves.size() != 0)
            {
                if (isWhiteTurn != end)
                {
                    gameInfo.SetTurn(end);
                }
                return false;
            }
        }
    }

    if (isWhiteTurn != end)
    {
        gameInfo.SetTurn(end);
    }

    return availableMoves.size() == 0;
}


QVector<MovementScore> Bot::GetAllTheMovements(bool isWhiteTurn)
{
    QVector<MovementScore> filteredTestMoves;

    QBrush turn = isWhiteTurn ? Qt::white : Qt::black;

    for (const auto& p: board->pieces)
    {

        if (p->getColor() == turn)
        {
            int score = p->GetPieceScore();
            auto moves = filter.GetFilteredMoves(p.get(), p->getPos()); // Midlle level with performance and accuarcy
            // auto moves = filter.GetFilteredMoves( p->getPos()); // Best accuarcy, slow
            // auto moves = p->GetLegalMoves(gameInfo.friendly, gameInfo.enemy); // Best performance, low accaurcy
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

int Bot::ScoreTheBoard(QVector<MovementScore> &filteredMoves)
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

    // Material Scoring
    for (auto& p : board->pieces)
    {
        int posScore = 0;
        int threatBonus = p->GetThreateningMoves(gameInfo.friendly, gameInfo.enemy).size() * 2;


        if (p->getType() == PieceType::Pawn)
        {
            if (p->getColor() == Qt::white)
            {
                if (p->getPos().x() > 2 && p->getPos().x() < 6)
                {
                    posScore = (8 - p->getPos().y()) * 5 + 30;
                }
            }
            else
            {
                if (p->getPos().x() > 2 && p->getPos().x() < 6)
                {
                    posScore = p->getPos().y() * 5 + 30;
                }
            }

        }
        if (p->getType() == PieceType::Knight)
        {
            posScore = p->GetLegalMoves(gameInfo.friendly, gameInfo.enemy).size() * 2;
        }

        if (p->getColor() == Qt::white)
        {
            score += p->GetPieceScore() + posScore + threatBonus;
        }
        else
        {
            score -= p->GetPieceScore() + posScore + threatBonus;
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

    }
    gameInfo.UpdateAreaFields(p->getPos(), movePos);
    p->Move(movePos);

    return std::move(capturePiece);

}

void Bot::UnDoMove(BasePiece *p, std::unique_ptr<BasePiece> capPiece, const QPoint &prePos, bool hasMoved)
{

    if (capPiece)
    {
        gameInfo.enemy.append(capPiece->getPos());
        board->pieces.push_back(std::move(capPiece));
    }

    gameInfo.UpdateAreaFields(p->getPos(), prePos);

    p->Move(prePos);
    p->hasMoved = hasMoved;

}


