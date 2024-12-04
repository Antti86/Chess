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
    MovementScore() = default;
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
    MovementScore GetMinMaxMove();

private:
    bool IsCheckMateMovement(bool isWhiteTurn) const;
    bool IsKingCheck(bool isWhiteTurn) const;

    QVector<MovementScore> GetAllTheMovements(bool isWhiteTurn);

    int ScoreTheBoard();

    int NegaMaxAlphaBeta(int depth, QVector<MovementScore>& filteredMoves, int turnMPlyier, int alpha, int beta );


    std::unique_ptr<BasePiece> DoMove(BasePiece *p, const QPoint& movePos);
    void UnDoMove(BasePiece *p, std::unique_ptr<BasePiece> capPiece, const QPoint& prePos, bool hasMoved);

private:
    ChessBoard* board;
    GameInfo& gameInfo;
    MoveFilter& filter;
    Rng rng; //??
    MovementScore bestMove;
    int DEPTH = 3;
    int counter = 0;

    static constexpr int CheckMate = 10000;
    static constexpr int Check = 200;

};

#endif // BOT_H
