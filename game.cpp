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
            BasePiece* piece = GetSelectedPiece(pos);
            QVector<QPoint> friendly = isWhiteTurn ? board->GetWhitePieceAreas() : board->GetBlackPieceAreas();
            QVector<QPoint> enemy = isWhiteTurn ? board->GetBlackPieceAreas() : board->GetWhitePieceAreas();
            emit SetSquareColor(selectedPiecePos, piece->GetLegalMoves(friendly, enemy), true);
        }
    }
    else
    {
        if (pos == selectedPiecePos)
        {
            isPieceSelected = false;
            BasePiece* piece = GetSelectedPiece(pos);
            QVector<QPoint> friendly = isWhiteTurn ? board->GetWhitePieceAreas() : board->GetBlackPieceAreas();
            QVector<QPoint> enemy = isWhiteTurn ? board->GetBlackPieceAreas() : board->GetWhitePieceAreas();
            emit SetSquareColor(selectedPiecePos, piece->GetLegalMoves(friendly, enemy), false);

        }
        else
        {
            if (ValidMovement(pos))
            {
                BasePiece* piece = GetSelectedPiece(pos);
                QVector<QPoint> friendly = isWhiteTurn ? board->GetWhitePieceAreas() : board->GetBlackPieceAreas();
                QVector<QPoint> enemy = isWhiteTurn ? board->GetBlackPieceAreas() : board->GetWhitePieceAreas();
                emit SetSquareColor(selectedPiecePos, piece->GetLegalMoves(friendly, enemy), false);

                if (IsEatingMovement(pos))
                {
                    emit EatPiece(pos);
                }
                emit pieceMoved(selectedPiecePos, pos);
                isPieceSelected = false;
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
    if (isWhiteTurn)
    {
        for (auto &piece : board->pieces)
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
        for (auto &piece : board->pieces)
        {
            if (piece->getPos() == square && piece->getColor() == Qt::black)
            {
                return true;
            }
        }
        return false;
    }
}

bool Game::ValidMovement(const QPoint pos) const
{
    BasePiece* piece = GetSelectedPiece(pos);

    if (piece)
    {
        QVector<QPoint> friendly = isWhiteTurn ? board->GetWhitePieceAreas() : board->GetBlackPieceAreas();
        QVector<QPoint> enemy = isWhiteTurn ? board->GetBlackPieceAreas() : board->GetWhitePieceAreas();
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
}

bool Game::IsEatingMovement(const QPoint pos) const
{
    QVector<QPoint> enemy = isWhiteTurn ? board->GetBlackPieceAreas() : board->GetWhitePieceAreas();
    return enemy.contains(pos);
}

BasePiece *Game::GetSelectedPiece(const QPoint pos) const
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
    return piece;
}
