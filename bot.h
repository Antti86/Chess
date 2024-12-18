#ifndef BOT_H
#define BOT_H

#pragma once
#include "chessboard.h"
#include "GameInfo.h"
#include "movefilter.h"
#include "Rng.h"

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
    int GetCounter() const; //For benchmarking purpose

private:
    // Bot "Ai"
    bool IsCheckMateMovement(bool isWhiteTurn) const;
    QVector<MovementScore> GetAllTheMovements(bool isWhiteTurn);
    int ScoreTheBoard(QVector<MovementScore>& filteredMoves);
    int NegaMaxAlphaBeta(int depth, QVector<MovementScore>& filteredMoves, int turnMPlyier, int alpha, int beta );

    //For the movement simulation, if capture movement, the piece will be moved in unique pointer and moved back with undomove
    std::unique_ptr<BasePiece> DoMove(BasePiece *p, const QPoint& movePos);
    void UnDoMove(BasePiece *p, std::unique_ptr<BasePiece> capPiece, const QPoint& prePos, bool hasMoved);

private:
    ChessBoard* board;
    GameInfo& gameInfo;
    MoveFilter& filter;
    Rng rng;
    MovementScore bestMove;

    int counter = 0; //For benchmarking purpose
    int DEPTH = 2;  // 2 == easy, 3 == medium, 4 == hard, Cant handle 5 for now
    static constexpr int CheckMate = 10000;
    static constexpr int Check = 200;

};

#endif // BOT_H
