#ifndef GAMEINFO_H
#define GAMEINFO_H

#pragma once
#include "chessboard.h"



class GameInfo
{
public:
    GameInfo(ChessBoard* board)
        : board(board)
    {
        isWhiteTurn = true;
        turn = isWhiteTurn ? Qt::white : Qt::black;
        friendly = isWhiteTurn ? board->GetPopulatedAreas(Qt::white) : board->GetPopulatedAreas(Qt::black);
        enemy = isWhiteTurn ? board->GetPopulatedAreas(Qt::black) : board->GetPopulatedAreas(Qt::white);
        dangerAreas = board->GetDangerAreas(isWhiteTurn, friendly, enemy);
    }

    QVector<QPoint> friendly; //Holds all the piece positions whos turn it is
    QVector<QPoint> enemy;    //Holds all the enemy piece positions
    QVector<QPoint> dangerAreas;
    bool isWhiteTurn;
    QBrush turn;

    void SetAreaFields()
    {
        isWhiteTurn = !isWhiteTurn;
        turn = isWhiteTurn ? Qt::white : Qt::black;
        friendly = isWhiteTurn ? board->GetPopulatedAreas(Qt::white) : board->GetPopulatedAreas(Qt::black);
        // friendly = enemy;
        enemy = isWhiteTurn ? board->GetPopulatedAreas(Qt::black) : board->GetPopulatedAreas(Qt::white);
        dangerAreas = board->GetDangerAreas(isWhiteTurn, friendly, enemy);
    }

    void SetTurn(bool iswhiteturn)
    {
        isWhiteTurn = iswhiteturn;
        turn = isWhiteTurn ? Qt::white : Qt::black;
        friendly = isWhiteTurn ? board->GetPopulatedAreas(Qt::white) : board->GetPopulatedAreas(Qt::black);
        enemy = isWhiteTurn ? board->GetPopulatedAreas(Qt::black) : board->GetPopulatedAreas(Qt::white);
        dangerAreas = board->GetDangerAreas(isWhiteTurn, friendly, enemy);
    }

    enum class GameStateStatus {Begin, Midlle, End};
    GameStateStatus status = GameStateStatus::Begin;
private:

    ChessBoard* board;

};

#endif // GAMEINFO_H
