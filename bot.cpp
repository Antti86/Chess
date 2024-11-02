#include "bot.h"
#include "Bishop.h"
#include "King.h"
#include "Knight.h"
#include "Pawn.h"
#include "Queen.h"
#include "Rook.h"

Bot::Bot(ChessBoard *board, GameInfo& gameInfo, MoveFilter &filter)
    : board(board)
    , gameInfo(gameInfo)
    , filter(filter)
{

}

MovementScore Bot::GetABestMove()
{

    QVector<Movement> filteredTestMoves;

    for (const auto& p: board->pieces)
    {
        if (p->getColor() == Qt::black)
        {
            filteredTestMoves.append(Movement(p->getPos(), filter.GetFilteredMoves(p->getPos())));
        }
    }

    QPoint enemyKingPos = filter.GetKingPos(Qt::white);

    QVector<MovementScore> fMove;


    for (auto& selpiece : filteredTestMoves)
    {
        auto tt = selpiece.From; //debugging
        BasePiece* p = filter.GetSelectedPiece(selpiece.From);
        bool hasMoved = p->hasMoved;
        for (auto& movePos : selpiece.To)
        {
            int score = 0;

            // auto enemyDangerArea = board->GetDangerAreas(true, gameInfo.enemy, gameInfo.friendly);

            auto friendlyDangerArea = board->GetDangerAreas(false, gameInfo.friendly, gameInfo.enemy);

            if (!HaveCover(p, p->getPos()) && friendlyDangerArea.contains(p->getPos()) &&
                (!friendlyDangerArea.contains(movePos) || HaveCover(p, movePos)))
            {
                score += p->GetPieceScore();
            }

            if (p->getType() == PieceType::Pawn)
            {
                score += GetPawnScore(selpiece.From, movePos);
            }

            BasePiece* capPiece = nullptr;
            PieceType capType;
            QPoint capPos;
            bool capHasMoved;
            bool capCanPassLeft;
            bool capCanPassRight;
            bool eating = filter.IsEatingMovement(movePos);
            if (eating)
            {
                capPiece = filter.GetSelectedPiece(movePos);
                capType = capPiece->getType();
                capPos = capPiece->getPos();
                capHasMoved = capPiece->hasMoved;
                capCanPassLeft = capPiece->canPassantLeft;
                capCanPassRight = capPiece->canPassantRight;

                score += capPiece->GetPieceScore();
                board->EatingPiece(movePos);
            }

            p->Move(movePos);

            if (IsCheckMateMovement())
            {
                score += 100000;
            }

            auto enemyDangerAreaAM = board->GetDangerAreas(true, gameInfo.enemy, gameInfo.friendly);

            auto friendlyDangerAreaAM = board->GetDangerAreas(false, gameInfo.friendly, gameInfo.enemy);

            if (friendlyDangerAreaAM.contains(p->getPos()) && !HaveCover(p, p->getPos()))
            {
                score -= p->GetPieceScore();
            }

            if (enemyDangerAreaAM.contains(enemyKingPos))
            {
                score += 100;
            }





            fMove.append(MovementScore(selpiece.From, movePos, score));

            //Setting back state that was before movement simulation
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
                p->hasMoved = hasMoved;
            }

        }
        p = nullptr;
    }

    int best = std::max_element(fMove.begin(), fMove.end())->score;

    QVector<MovementScore> bestList;

    for (auto& i : fMove)
    {
        if (i.score == best)
        {
            bestList.append(i);
        }
    }

    return bestList[rng.CreateInt(0, bestList.size() - 1)];

    // return *std::max_element(fMove.begin(), fMove.end());

}

MovementScore Bot::GetMinMaxMove()
{

    auto filteredMoves = GetAllTheMovements(Qt::black);

    int aiScore = 0;
    int playerScore = 0;
    int playerMaxScore = 0;
    // int playerMinMaxScore = 0;

    QVector<MovementScore> fMove;

    for (auto& selpiece : filteredMoves)
    {

        BasePiece* p = filter.GetSelectedPiece(selpiece.From);
        bool hasMoved = p->hasMoved;

        for (auto& movePos : selpiece.To)
        {

            std::unique_ptr<BasePiece> capturedPiece = DoMove(p, movePos);
            // playerMinMaxScore = playerMaxScore;
            playerMaxScore = 0;
            gameInfo.SetAreaFields();
            auto filteredMovesOpponent = GetAllTheMovements(Qt::white);
            gameInfo.SetAreaFields();
            for (auto& selpiece2 : filteredMovesOpponent)
            {

                BasePiece* p2 = filter.GetSelectedPiece(selpiece2.From);
                bool hasMoved2 = p2->hasMoved;

                for (auto& movePos2 : selpiece2.To)
                {

                    std::unique_ptr<BasePiece> capturedPiece2 = DoMove(p2, movePos2);

                    playerScore = GetScoreEval(Qt::white) - GetScoreEval(Qt::black);

                    if (playerScore > playerMaxScore)
                    {
                        playerMaxScore = playerScore;
                    }
                    UnDoMove(p2, std::move(capturedPiece2), selpiece2.From, hasMoved2, Qt::black);
                }
            }

            fMove.append(MovementScore(selpiece.From, movePos, playerMaxScore));

            if (selpiece.From == QPoint(6,1))
            {
                int x = 0;
                int k = 0;
            }

            UnDoMove(p, std::move(capturedPiece), selpiece.From, hasMoved, Qt::white);


        }


    }

    gameInfo.SetAreaFields();
    return *std::min_element(fMove.begin(), fMove.end());

}

int Bot::GetPawnScore(const QPoint& From, const QPoint& To) const
{
    int score = 0;

    // if (To.y() - From.y() == 2)
    // {
    //     score += 5;
    // }

    if (To.x() > 1 && To.x() < 6)
    {
        score += 10;
    }

    score += To.y();

    return score;
}

bool Bot::HaveCover(BasePiece* p, const QPoint& checkPos) const
{
    if (p->getType() == PieceType::Queen || p->getType() == PieceType::King)
    {
        return false;
    }

    auto thretPieces = board->GetSquarethreateningPiece(checkPos, gameInfo.friendly, gameInfo.enemy);

    for (auto& tp : thretPieces)
    {
        if (tp->getType() == PieceType::Pawn)
        {
            return false;
        }
    }

    // auto friendlyDangerArea = board->GetDangerAreas(false, gameInfo.friendly, gameInfo.enemy);

    auto enemyDangerArea = board->GetDangerAreas(true, gameInfo.enemy, gameInfo.friendly);


    if (enemyDangerArea.contains(checkPos))
    {
        return true;
    }

    return false;
}

bool Bot::IsCheckMateMovement() const
{
    QVector<QPoint> availableMoves;
    for (auto& p : board->pieces)
    {
        if (p->getColor() == Qt::white)
        {
            QVector<QPoint> legalMoves = p->GetLegalMoves(gameInfo.enemy, gameInfo.friendly);
            if (p->getType() == PieceType::King)
            {
                availableMoves.append(filter.FilterKingMovements(legalMoves, filter.GetKingPos(Qt::white)));
            }
            else
            {
                availableMoves.append(filter.FilterAvailableMovements(legalMoves, p->getPos()));
            }
        }
    }
    return availableMoves.size() == 0;
}

QVector<Movement> Bot::GetAllTheMovements(QBrush turn)
{
    QVector<Movement> filteredTestMoves;

    for (const auto& p: board->pieces)
    {
        if (p->getColor() == turn)
        {
            filteredTestMoves.append(Movement(p->getPos(), filter.GetFilteredMoves(p->getPos())));
        }
    }
    return filteredTestMoves;
}

int Bot::GetScoreEval(QBrush turn)
{
    int score = 0;

    for (auto& p : board->pieces)
    {
        if (p->getColor() == turn)
        {
            score += p->GetPieceScore();
        }
    }
    return score;
}

std::unique_ptr<BasePiece> Bot::DoMove(BasePiece* p, const QPoint &movePos)
{
    BasePiece* capPiece = filter.GetSelectedPiece(movePos);


    PieceStateRecord psr;

    std::unique_ptr<BasePiece> capturePiece = nullptr;

    if (capPiece)
    {

        // psr = PieceStateRecord(capPiece->getType(), capPiece->getPos(), capPiece->hasMoved, capPiece->getColor(),
        //                        capPiece->canPassantLeft, capPiece->canPassantRight);
        // capturedMove = true;
        // board->EatingPiece(movePos);
        std::unique_ptr<BasePiece> capturePiece = filter.GivePiecePointer(movePos);

        auto it = std::remove_if(board->pieces.begin(), board->pieces.end(), [](const std::unique_ptr<BasePiece>& piece)
                                 {
                                     if (!piece)
                                     {
                                         return true;
                                     }
                                     return false;
                                 });
        board->pieces.erase(it, board->pieces.end());

    }

    p->Move(movePos);

    return std::move(capturePiece);

}

void Bot::UnDoMove(BasePiece *p, std::unique_ptr<BasePiece> capPiece, const QPoint &prePos, bool hasMoved, QBrush enemy)
{

    if (capPiece)
    {
        // std::unique_ptr<BasePiece> newPiece;
        // switch (capPiece->getType())
        // {
        // case PieceType::Bishop:
        //     newPiece = std::make_unique<Bishop>(enemy, capPiece->getPos(), capPiece->getType());
        //     break;
        // case PieceType::Pawn:
        //     newPiece = std::make_unique<Pawn>(enemy, capPiece->getPos(), capPiece->getType());
        //     break;
        // case PieceType::Queen:
        //     newPiece = std::make_unique<Queen>(enemy, capPiece->getPos(), capPiece->getType());
        //     break;
        // case PieceType::King:
        //     newPiece = std::make_unique<King>(enemy, capPiece->getPos(), capPiece->getType());
        //     break;
        // case PieceType::Knight:
        //     newPiece = std::make_unique<Knight>(enemy, capPiece->getPos(), capPiece->getType());
        //     break;
        // case PieceType::Rook:
        //     newPiece = std::make_unique<Rook>(enemy, capPiece->getPos(), capPiece->getType());
        //     break;
        // }
        // newPiece->hasMoved = capPiece->hasMoved;
        // newPiece->canPassantLeft = capPiece->canPassantLeft;
        // newPiece->canPassantRight = capPiece->canPassantRight;

        board->AddPiece(std::move(capPiece));
    }

    p->Move(prePos);
    p->hasMoved = hasMoved;
}


