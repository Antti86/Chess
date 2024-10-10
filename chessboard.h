#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#pragma once
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QMouseEvent>
#include <QVector>
#include "Pieces/basepiece.h"
#include "qgraphicsitem.h"
#include "qgraphicsview.h"
#include "boardsquare.h"
#include "Pieces/PieceStateRecord.h"
#include <memory>

class ChessBoard : public QGraphicsView
{
    Q_OBJECT
public:
    explicit ChessBoard(QWidget *widget = 0);
    ~ChessBoard();
    void ResetBoard();
    bool IsInsideBoard(const QPoint& pos) const;
    const QVector<QPoint> GetPopulatedAreas(QBrush color) const;
    const QVector<QPoint> GetDangerAreas(bool isWhiteturn, const QVector<QPoint> &friendly, const QVector<QPoint>& enemy,
                                         const QPoint ignoredPiecePos = QPoint(-1, -1) ) const;

    int GetMoveCount() const;

    QGraphicsScene *scene;

    std::vector<std::unique_ptr<BasePiece>> pieces; // Cant use QVector, wont play nice with unique_ptr
    void AddPiece(std::unique_ptr<BasePiece> piece); //Bot needs to add and delete pieces in simulation, so public

    //Encap these??
    QVector<QVector<PieceStateRecord>> repetitionTrack; //Wont work with bot right now
    QVector<QVector<PieceStateRecord>> posRecords;

public slots:
    void MovePiece(QPoint from, QPoint to, bool isWhiteTurn);
    void SettingSquareColor(const QPoint& pos, const QVector<QPoint>& legalMoves, bool highlighting);
    void EatingPiece(QPoint eatingPos);
    void PromotingPawn(const QPoint& pos, PieceType newPieceType, bool isWhiteTurn);
    void ReverseMoveAndTurn();

signals:
    void pieceSelected(QPoint pos);
    void endTurn();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    void DrawBoard();
    void DrawPieces();
    void SetPiecesOnBoard();
    void UpdateBoard();

    BoardSquare* GetSelectedSquare(const QPoint& pos) const;

    void CheckPassantStatus(BasePiece* piece, const QPoint& from, const QPoint& to, bool isWhiteTurn);

    void RecordPiecePositions();
    void ResetRepetitionTrack();
    void ResetAllRecords();

private:
    int moveCount;

    //Square color
    QBrush fColor;
    QBrush sColor;

    //Coordinate font and color
    QBrush textColor;
    QFont font;

    //Testing for now..
    int squareSize = 70;
    int boardWidth = 8 * squareSize;
    int boardHeight = 8 * squareSize;
    QGraphicsRectItem fullBoardRect;
};

#endif // CHESSBOARD_H
