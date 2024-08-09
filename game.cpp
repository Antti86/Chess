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
            if (ValidMovement(pos))
            {
                if (IsEatingMovement(pos))
                {
                    emit EatPiece(pos);

                }
                emit pieceMoved(selectedPiecePos, pos);
                isPieceSelected = false;
                emit SetSquareColor(selectedPiecePos, false);
            }

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

bool Game::ValidMovement(const QPoint pos) const
{
    BasePiece* piece = nullptr;
    for (auto& p : board->pieces)
    {
        if (p->getPos() == selectedPiecePos)
        {
            piece = p;
            break;
        }
    }

    if (piece)
    {

        QVector<QPoint> friendly = isWhiteTurn ? board->GetWhitePieceAreas() : board->GetBlackPieceAreas();
        QVector<QPoint> enemy = isWhiteTurn ? board->GetBlackPieceAreas() : board->GetWhitePieceAreas();

        // QVector<QPoint> populatedAreas = board->CheckPopulatedAreas();
        QVector<QPoint> legalMoves = piece->GetLegalMoves(friendly, enemy);

        if (legalMoves.contains(pos))
        {
            return true;
        }
        else
        {
            return false;
        }

    }
    else
    {
        return false;
    }






    return true;

}

bool Game::IsEatingMovement(const QPoint pos) const
{
    QVector<QPoint> enemy = isWhiteTurn ? board->GetBlackPieceAreas() : board->GetWhitePieceAreas();

    return enemy.contains(pos);

}
