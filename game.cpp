#include "game.h"

Game::Game(ChessBoard *board, QObject *parent)
    :board(board),
    QObject(parent)
{
    isWhiteTurn = true;
    isPieceSelected = false;
}

void Game::handlePieceSelection(QPoint pos)
{

    if (!isPieceSelected)
    {
        if (IsPieceOnSelectedSquare(pos))
        {
            selectedPiecePos = pos;
            isPieceSelected = true;
        }
    }
    else
    {
        if (pos == selectedPiecePos)
        {
            isPieceSelected = false;
        }
        else
        {
            emit pieceMoved(selectedPiecePos, pos);
            isPieceSelected = false;
        }
    }
}

void Game::EndOfTurn()
{
    isWhiteTurn = !isWhiteTurn;
}

bool Game::IsPieceOnSelectedSquare(QPoint square) const
{
    if (isWhiteTurn)
    {
        for (auto& piece : board->pieces)
        {
            if (piece->getPos() == square && piece->getColor() == Qt::white)
            {
                return true;
            }
        }
        return false;
    }
    else
    {
        for (auto& piece : board->pieces)
        {
            if (piece->getPos() == square && piece->getColor() == Qt::black)
            {
                 return true;
            }
        }
        return false;
    }
}
