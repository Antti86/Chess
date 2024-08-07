#include "game.h"

Game::Game(ChessBoard *board, QObject *parent)
    : QObject(parent)
    , board(board)
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
            emit SetSquareColor(selectedPiecePos, true);
        }
    }
    else
    {
        if (pos == selectedPiecePos)
        {
            isPieceSelected = false;
            emit SetSquareColor(selectedPiecePos, false);
        }
        else
        {
            emit pieceMoved(selectedPiecePos, pos);
            isPieceSelected = false;
            emit SetSquareColor(selectedPiecePos, false);
        }
    }
}

void Game::EndOfTurn()
{
    isWhiteTurn = !isWhiteTurn;
    emit UpdateUiToTurn(isWhiteTurn);
}

bool Game::IsPieceOnSelectedSquare(QPoint square) const
{
    if (isWhiteTurn) {
        for (auto &piece : board->pieces) {
            if (piece->getPos() == square && piece->getColor() == Qt::white) {
                return true;
            }
        }
        return false;
    } else {
        for (auto &piece : board->pieces) {
            if (piece->getPos() == square && piece->getColor() == Qt::black) {
                return true;
            }
        }
        return false;
    }
}
