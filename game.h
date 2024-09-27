#ifndef GAME_H
#define GAME_H

#pragma once
#include "chessboard.h"
#include "qobject.h"
#include "PromotionDialog.h"
#include <cmath>


enum class EndStatus { WhiteWins, BlackWins, InsufficientMaterialDraw, DeadDraw, StaleMate, ThreefoldDraw, FiftyMovesDraw};

class Game : public QObject
{
    Q_OBJECT

public:
    Game(ChessBoard *board, QObject *parent);

signals:
    void pieceMoved(QPoint from, QPoint to, bool isWhiteTurn);
    void EatPiece(QPoint eatingPos);
    void SetSquareColor(const QPoint& pos, const QVector<QPoint>& legalMoves, bool highlighting);
    void PawnPromotion(const QPoint& pos, PieceType newPieceType, bool isWhiteTurn);
    void gameOver(bool isWhiteWinner);
    void UpdateUiToTurn(bool isWhiteTurn);
    void UpdateUiForCheck(bool isChecked);
    void UpdateUiForGameOver(EndStatus status);

public slots:
    void handlePieceSelection(QPoint pos);
    void EndOfTurn();

private:
    bool IsPieceOnSelectedSquare(QPoint square) const;
    bool ValidMovement(BasePiece* piece, QVector<QPoint> legalMoves, QPoint& pos) const;
    bool IsEatingMovement(const QPoint pos) const;
    bool IsPassantMovement(BasePiece* piece, const QPoint& selectedPiecePos, const QPoint &newpos) const;
    BasePiece* GetSelectedPiece(const QPoint pos) const;
    bool IsKingChecked(const QVector<QPoint> &dangerAreas) const;
    bool IsCheckMate() const;
    bool IsStaleMate() const;
    bool InsufficientMaterialDraw() const;

    bool IsThreeRepetitionDraw() const;
    bool ArePositionsEqual(const QVector<PieceStateRecord>& pos1, const QVector<PieceStateRecord>& pos2) const;

    bool IsDeadPositionDraw() const;
    bool Is50MoveDraw() const;

    bool IsPawnPromotionMove(BasePiece* piece, const QPoint pos) const;

    BasePiece* IsCastlingMove(BasePiece* piece, const QPoint& pos) const;
    QVector<QPoint> GetCastlingMoves() const;

    QVector<QPoint> FilterKingMovements(const QVector<QPoint>& moves, const QPoint &kingPos) const;
    QVector<QPoint> FilterAvailableMovements(const QVector<QPoint>& moves) const;
    QPoint GetTurnKingPos() const;

    void SetAreaFields();
    QVector<QPoint> GetGhostArea(const QVector<QPoint> &field, const QPoint &pos) const;

private:
    bool isWhiteTurn;
    QBrush turn;
    bool isPieceSelected;
    QPoint selectedPiecePos;
    ChessBoard *board;
    QVector<QPoint> friendly; //Holds all the piece positions whos turn it is
    QVector<QPoint> enemy;    //Holds all the enemy piece positions
};

#endif // GAME_H
