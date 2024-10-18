#ifndef BOT_H
#define BOT_H

#pragma once
#include "chessboard.h"
#include "GameInfo.h"
#include "movefilter.h"
#include "Rng.h"

struct Movement //Model for storing piece and all its movement options
{
    Movement(QPoint From, QVector<QPoint> To)
        : From(From)
        , To(To)
    {}
    QPoint From;
    QVector<QPoint> To;
};

struct MovementScore    //Model for storing individual movement and its score
{
    MovementScore(QPoint From, QPoint To, int score)
        : From(From)
        , To(To)
        , score(score)
    {}
    QPoint From;
    QPoint To;
    int score;

    bool operator<(MovementScore& rhs)
    {
        return this->score < rhs.score;
    }
    bool operator>(MovementScore& rhs)
    {
        return this->score > rhs.score;
    }
    bool operator ==(MovementScore& rhs)
    {
        return this->score == rhs.score;
    }
};

class Bot
{
public:
    Bot(ChessBoard* board, GameInfo& gameInfo, MoveFilter& filter);
    MovementScore GetABestMove();
    MovementScore GetMinMaxMove();

private:
    int GetPawnScore(const QPoint &From, const QPoint &To) const;
    bool HaveCover(BasePiece *p, const QPoint &checkPos) const;
    bool IsCheckMateMovement() const;

    QVector<Movement> GetAllTheMovements(QBrush turn);

    int GetScoreEval(QBrush turn);


    std::unique_ptr<BasePiece> DoMove(BasePiece *p, const QPoint& movePos);
    void UnDoMove(BasePiece *p, std::unique_ptr<BasePiece> capPiece, const QPoint& prePos, bool hasMoved, QBrush enemy);

private:
    ChessBoard* board;
    GameInfo& gameInfo;
    MoveFilter& filter;
    Rng rng;

};

#endif // BOT_H
