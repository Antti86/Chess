#ifndef BOT_H
#define BOT_H

#pragma once
#include "chessboard.h"
#include "GameInfo.h"
#include "movefilter.h"

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

private:
    int GetPawnScore(const QPoint &From, const QPoint &To) const;

private:
    ChessBoard* board;
    GameInfo& gameInfo;
    MoveFilter& filter;


};

#endif // BOT_H
