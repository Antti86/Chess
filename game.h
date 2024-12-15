#ifndef GAME_H
#define GAME_H

#pragma once
#include "chessboard.h"
#include "qobject.h"
#include "bot.h"
#include "movefilter.h"
#include "GameInfo.h"



enum class EndStatus { WhiteWins, BlackWins, InsufficientMaterialDraw, DeadDraw, StaleMate, ThreefoldDraw, FiftyMovesDraw};

class Game : public QObject
{
    Q_OBJECT

public:
    Game(ChessBoard *board, QObject *parent);

signals:
    void pieceMoved(QPoint from, QPoint to, bool isWhiteTurn); //Starts a event chain that will end turn
    void EatPiece(QPoint eatingPos);
    void SetSquareColor(const QPoint& pos, const QVector<QPoint>& legalMoves, bool highlighting);
    void PawnPromotion(const QPoint& pos, PieceType newPieceType, bool isWhiteTurn);
    void gameOver(bool isWhiteWinner);
    void UpdateUiToTurn(bool isWhiteTurn);
    void UpdateUiForCheck(bool isChecked);
    void UpdateUiForGameOver(EndStatus status);

public slots:
    void handlePieceSelection(QPoint pos); // Handles player gaming logic
    void EndOfTurn(); // Check all the game ending states and sets gameInfo values

private:
    bool IsPieceOnSelectedSquare(QPoint square) const;
    bool ValidMovement(BasePiece* piece, QVector<QPoint> legalMoves, QPoint& pos) const;

    // Some special move checkers
    bool IsPassantMovement(BasePiece* piece, const QPoint& selectedPiecePos, const QPoint &newpos) const;
    bool IsPawnPromotionMove(BasePiece* piece, const QPoint pos) const;
    BasePiece* IsCastlingMove(BasePiece* piece, const QPoint& pos) const;

    // Ending checks
    bool IsCheckMate() const;
    bool IsStaleMate() const;
    bool InsufficientMaterialDraw() const;
    bool IsThreeRepetitionDraw() const;
    bool ArePositionsEqual(const QVector<PieceStateRecord>& pos1, const QVector<PieceStateRecord>& pos2) const;
    bool IsDeadPositionDraw() const;
    bool Is50MoveDraw() const;

    // If playing against bot
    void BotMovement();

    float MeanTime(); // for performance tests

private:
    bool isPieceSelected;
    QPoint selectedPiecePos;
    ChessBoard *board;
    GameInfo gameInfo; //Contains gamestate info, like whos turn etc..
    Bot bot;    //Ai opponent
    MoveFilter filter; // Helper class for movement checks and filtering
    QVector<std::pair<float, int>> runTime; // for performance tests
};

#endif // GAME_H
