#include "game.h"

Game::Game(ChessBoard *board, QObject *parent)
    : QObject(parent)
    , board(board)
{
    isWhiteTurn = true;
    SetAreaFields();
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
            QVector<QPoint> moves = piece->GetLegalMoves(friendly, enemy);

            if (!IsKingChecked(GetGhostArea()))
            {
                if (piece->getType() == PieceType::King)
                {
                    QVector<QPoint> filteredMoves = FilterCheckedAreas(moves);
                    emit SetSquareColor(selectedPiecePos, filteredMoves, true);
                }
                else
                {
                    emit SetSquareColor(selectedPiecePos, moves, true);
                }
            }
            else
            {
                QVector<QPoint> filteredMoves = Filter(moves);
                emit SetSquareColor(selectedPiecePos, filteredMoves, true);

            }



        }
    }
    else
    {
        if (pos == selectedPiecePos)
        {
            isPieceSelected = false;
            BasePiece* piece = GetSelectedPiece(pos);
            QVector<QPoint> moves = piece->GetLegalMoves(friendly, enemy);

            if (!IsKingChecked(GetGhostArea()))
            {
                if (piece->getType() == PieceType::King)
                {
                    QVector<QPoint> filteredMoves = FilterCheckedAreas(moves);
                    emit SetSquareColor(selectedPiecePos, filteredMoves, false);
                }
                else
                {
                    emit SetSquareColor(selectedPiecePos, moves, false);
                }
            }
            else
            {


                QVector<QPoint> filteredMoves = Filter(moves);
                emit SetSquareColor(selectedPiecePos, filteredMoves, false);
            }



        }
        else
        {
            BasePiece* piece = GetSelectedPiece(pos);
            QVector<QPoint> moves = piece->GetLegalMoves(friendly, enemy);
            QVector<QPoint> filteredMoves;
            if (piece->getType() == PieceType::King)
            {
                filteredMoves = FilterCheckedAreas(moves);
            }
            else
            {
                filteredMoves = moves;
            }

            if (ValidMovement(piece, filteredMoves, pos))
            {
                emit SetSquareColor(selectedPiecePos, filteredMoves, false);
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
    bool isChecked = IsKingChecked(friendly);
    emit UpdateUiForCheck(isChecked);
    emit UpdateUiToTurn(isWhiteTurn);
    SetAreaFields();
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

bool Game::ValidMovement(BasePiece* piece, QVector<QPoint> legalMoves, QPoint& pos) const //TODO
{


    if (piece)
    {



        return legalMoves.contains(pos);
    }
    else
    {
        return false;
    }
}

bool Game::IsEatingMovement(const QPoint pos) const
{
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

bool Game::IsKingChecked(const QVector<QPoint> &f)
{
    QPoint kingPos;
    QBrush turn = isWhiteTurn ? Qt::white : Qt::black;
    for (auto& p : board->pieces)
    {
        if (p->getType() == PieceType::King && p->getColor() == turn)
        {
            kingPos = p->getPos();
        }
    }

    QVector<QPoint> dangerAreas = board->GetDangerAreas(isWhiteTurn, f, enemy);

    if (dangerAreas.contains(kingPos))
    {
        return true;
    }
    else
    {
        return false;
    }

}



QVector<QPoint> Game::FilterCheckedAreas(QVector<QPoint> &moves)
{
    QVector<QPoint> filteredMoves;

    ghostFriendly = GetGhostArea();


    QVector<QPoint> dangerAreas = board->GetDangerAreas(isWhiteTurn, ghostFriendly, enemy);

    for (auto& m : moves)
    {
        if (!dangerAreas.contains(m))
        {
            filteredMoves.append(m);
        }
    }
    return filteredMoves;

}

void Game::SetAreaFields()
{
    friendly = isWhiteTurn ? board->GetWhitePieceAreas() : board->GetBlackPieceAreas();
    enemy = isWhiteTurn ? board->GetBlackPieceAreas() : board->GetWhitePieceAreas();
}

QVector<QPoint> Game::GetGhostArea()
{
    QVector<QPoint> ret;
    for (auto& p : friendly)
    {
        if (p != selectedPiecePos)
        {
            ret.append(p);
        }
    }
    return ret;

}

QVector<QPoint> Game::Filter(const QVector<QPoint> &moves)
{
    QVector<QPoint> ret;
    QVector<QPoint> ghost = GetGhostArea();


    for (auto& p : moves)
    {

        ghost.append(p);
        if (!IsKingChecked(ghost))
        {
            ret.append(p);
        }
    }


    return ret;
}
