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
            QVector<QPoint> dangerAreas = board->GetDangerAreas(isWhiteTurn, GetGhostArea(friendly, selectedPiecePos), enemy);

            if (!IsKingChecked(dangerAreas))
            {
                if (piece->getType() == PieceType::King)
                {
                    QVector<QPoint> filteredMoves = FilterKingMovements(moves);
                    emit SetSquareColor(selectedPiecePos, filteredMoves, true);
                }
                else
                {
                    emit SetSquareColor(selectedPiecePos, moves, true);
                }
            }
            else
            {
                if (piece->getType() == PieceType::King)
                {
                    QVector<QPoint> filteredMoves = FilterKingMovements(moves);
                    emit SetSquareColor(selectedPiecePos, filteredMoves, true);
                }
                else
                {
                    QVector<QPoint> filteredMoves = FilterAvailableMovements(moves);
                    emit SetSquareColor(selectedPiecePos, filteredMoves, true);
                }
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
            emit SetSquareColor(selectedPiecePos, moves, false);
        }
        else
        {
            BasePiece* piece = GetSelectedPiece(selectedPiecePos);
            QVector<QPoint> moves = piece->GetLegalMoves(friendly, enemy);
            QVector<QPoint> filteredMoves;
            QVector<QPoint> dangerAreas = board->GetDangerAreas(isWhiteTurn, GetGhostArea(friendly, selectedPiecePos), enemy);

            if (!IsKingChecked(dangerAreas))
            {
                if (piece->getType() == PieceType::King)
                {
                    filteredMoves = FilterKingMovements(moves);
                }
                else
                {
                    filteredMoves = moves;
                }
            }
            else
            {
                if (piece->getType() == PieceType::King)
                {
                    filteredMoves = FilterKingMovements(moves);
                }
                else
                {
                    filteredMoves = FilterAvailableMovements(moves);
                }
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
    SetAreaFields();
    QVector<QPoint> dangerAreas = board->GetDangerAreas(isWhiteTurn, friendly, enemy);
    bool isChecked = IsKingChecked(dangerAreas);
    emit UpdateUiForCheck(isChecked);
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

bool Game::ValidMovement(BasePiece* piece, QVector<QPoint> legalMoves, QPoint& pos) const
{
    return piece && legalMoves.contains(pos);
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

bool Game::IsKingChecked(const QVector<QPoint> &dangerAreas)
{
    QPoint kingPos;
    QBrush turn = isWhiteTurn ? Qt::white : Qt::black;
    for (auto& p : board->pieces)
    {
        if (p->getType() == PieceType::King && p->getColor() == turn)
        {
            kingPos = p->getPos();
            break;
        }
    }
    return dangerAreas.contains(kingPos);
}

QVector<QPoint> Game::FilterKingMovements(QVector<QPoint> &moves)
{
    QVector<QPoint> filteredMoves;
    QVector<QPoint> ghostFriendly = GetGhostArea(friendly, selectedPiecePos);

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
    friendly = isWhiteTurn ? board->GetPopulatedAreas(Qt::white) : board->GetPopulatedAreas(Qt::black);
    enemy = isWhiteTurn ? board->GetPopulatedAreas(Qt::black) : board->GetPopulatedAreas(Qt::white);
}

QVector<QPoint> Game::GetGhostArea(const QVector<QPoint>& field, const QPoint& pos)
{
    QVector<QPoint> ret;
    for (auto& p : field)
    {
        if (p != pos)
        {
            ret.append(p);
        }
    }
    return ret;
}

QVector<QPoint> Game::FilterAvailableMovements(const QVector<QPoint> &moves)
{
    QVector<QPoint> filteredMoves;
    QVector<QPoint> ghost = GetGhostArea(friendly, selectedPiecePos);

    for (auto& p : moves)
    {
        if (IsEatingMovement(p))
        {
            ghost.append(p);
            QVector<QPoint> ghostEnemy = GetGhostArea(enemy, p);
            QVector<QPoint> dangerAreas2 = board->GetDangerAreas(isWhiteTurn, ghost, ghostEnemy, p);

            if (!IsKingChecked(dangerAreas2))
            {
                filteredMoves.append(p);
            }
            ghost.pop_back();
        }
        else
        {
            ghost.append(p);
            QVector<QPoint> dangerAreas = board->GetDangerAreas(isWhiteTurn, ghost, enemy);

            if (!IsKingChecked(dangerAreas))
            {
                filteredMoves.append(p);
            }
            ghost.pop_back();
        }
    }
    return filteredMoves;
}
