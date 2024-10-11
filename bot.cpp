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

            if (friendlyDangerArea.contains(p->getPos()) && !friendlyDangerArea.contains(movePos))
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

            auto enemyDangerAreaAM = board->GetDangerAreas(true, gameInfo.enemy, gameInfo.friendly);

            auto friendlyDangerAreaAM = board->GetDangerAreas(false, gameInfo.friendly, gameInfo.enemy);

            if (friendlyDangerAreaAM.contains(p->getPos()))
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

    return *std::max_element(fMove.begin(), fMove.end());

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

