#include "bot.h"

Bot::Bot(ChessBoard *board, GameInfo& gameInfo, MoveFilter &filter)
    : board(board)
    , gameInfo(gameInfo)
    , filter(filter)
{

}

