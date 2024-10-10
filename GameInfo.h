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
    }

    QVector<QPoint> friendly; //Holds all the piece positions whos turn it is
    QVector<QPoint> enemy;    //Holds all the enemy piece positions
    bool isWhiteTurn;
    QBrush turn;

    void SetAreaFields()
    {
        isWhiteTurn = !isWhiteTurn;
        turn = isWhiteTurn ? Qt::white : Qt::black;
        friendly = isWhiteTurn ? board->GetPopulatedAreas(Qt::white) : board->GetPopulatedAreas(Qt::black);
        enemy = isWhiteTurn ? board->GetPopulatedAreas(Qt::black) : board->GetPopulatedAreas(Qt::white);
    }
private:

    ChessBoard* board;

};

#endif // GAMEINFO_H
