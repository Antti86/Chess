#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QMouseEvent>
#include <QVector>
#include "Pieces/basepiece.h"
#include "qgraphicsitem.h"
#include "qgraphicsview.h"
#include "boardsquare.h"

class ChessBoard : public QGraphicsView
{
    Q_OBJECT
public:
    explicit ChessBoard(QWidget *widget = 0);
    ~ChessBoard();

    QGraphicsScene *scene;
    QVector<BasePiece *> pieces;

    const QVector<QPoint> GetPopulatedAreas(QBrush color) const;

    const QVector<QPoint> GetDangerAreas(bool isWhiteturn, const QVector<QPoint> &friendly, const QVector<QPoint>& enemy,
                                         const QPoint ignoredPiecePos = QPoint(-1, -1) ) const;

    void ResetBoard();

public slots:
    void MovePiece(QPoint from, QPoint to, bool isWhiteTurn);
    void SettingSquareColor(const QPoint& pos, const QVector<QPoint>& legalMoves, bool highlighting);
    void EatingPiece(QPoint eatingPos);

signals:
    void pieceSelected(QPoint pos);
    void endTurn();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override; //TODO

private:
    void DrawBoard();
    void DrawPieces();
    void SetPiecesOnBoard();
    void AddPiece(BasePiece *piece);
    void UpdateBoard();

    BoardSquare* GetSelectedSquare(const QPoint& pos) const;

    void CheckPassantStatus(BasePiece* piece, const QPoint& from, const QPoint& to, bool isWhiteTurn);



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
